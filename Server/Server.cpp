/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-elk <mait-elk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 14:29:26 by mait-elk          #+#    #+#             */
/*   Updated: 2024/11/17 17:29:50 by mait-elk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server() {
	pollfd				server_pfd;
	int					opt = 1;
	struct sockaddr_in serverAddress;
	
	ServerFD = socket(AF_INET, SOCK_STREAM, 0);
	if (ServerFD == -1)
		throw std::runtime_error("Socket Creation Failed !");
	std::memset(&serverAddress, 0, sizeof(serverAddress));
	std::memset(&server_pfd, 0, sizeof(server_pfd));
	std::memset(&buffer, 0, sizeof(buffer));
	
	serverAddress.sin_port = htons(6667);
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	setsockopt(ServerFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
	if (bind(ServerFD, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) == -1) {
		close(ServerFD);
		throw std::runtime_error("Bind Failed !");
	}

	if (listen(ServerFD, 1024) == -1) {
		close(ServerFD);
		throw std::runtime_error("Listen Failed !");
	}
	server_pfd.events = POLLIN;
	server_pfd.fd = ServerFD;
	pfds.push_back(server_pfd);

	std::cout << "The Game server has been started..." << std::endl;
	std::cout << "IP: 127.0.0.1 \nPORT: " << 6667 << std::endl;
}

Server::~Server() {
	
}

void Server::StartServer() {

	std::vector<Player> players;

	while (true) {
		int pollEvent = poll(pfds.data(), pfds.size(), -1);
		if (pollEvent == -1)
			throw std::runtime_error("Poll Failed !");
		
		if (pfds[0].revents & POLLIN) {
			pollfd pfd = {.fd = -1, .events = 0, .revents = 0};
			int clientFd = accept(ServerFD, NULL, NULL);
			if (clientFd == -1)
				continue;
			Player newp;
			newp.id = pfds.size();
			sendMsgToClient<Player>(clientFd, &newp);
			if (pfds[0].revents & POLLIN) {
				players.push_back(ReceiveMsgFromClient<Player>(clientFd));
				std::cout << "GOT NEW PLAYER DATA :)" << std::endl;
			}
			pfd.fd = clientFd;
			pfd.events = POLLIN;
			pfds.push_back(pfd);
			
			for (std::vector<pollfd>::iterator i = pfds.begin() + 1; i < pfds.end(); i++) {
				std::cout << "sending to : " << i->fd << std::endl;
				for (std::vector<Player>::iterator j = players.begin(); j < players.end(); j++) {
					sendMsgToClient<Player>(i->fd, &(*j));
					std::cout << "p :" << j->id << std::endl;
				}
			}
		}

		int i = 0;
		std::vector<pollfd>::iterator it = (pfds.begin() + 1);
		std::vector<Player>::iterator itp = players.begin();
		while (it < pfds.end()) {
			if (it->revents & POLLIN) {
				try {
					Player response = ReceiveMsgFromClient<Player>(it->fd);
					for (std::vector<Player>::iterator plr = players.begin(); plr < players.end(); plr++) {
						if (plr->id == response.id){
							plr->pos = response.pos;
							break;
						}
					}

					for (std::vector<pollfd>::iterator i = pfds.begin() + 1; i < pfds.end(); i++) {
						sendMsgToClient<Player>(i->fd, &response);
					}
				} catch (std::exception &e) {
					std::cout << "got :" << e.what() << " => Player Exited." << std::endl;
					close(it->fd);
					pfds.erase(it);
					if (!players.empty())
						players.erase(itp);
				}
			}
			itp++;
			it++;
			i++;
		}
	}
}


Server::Server(const Server &copy) {
	if (&copy != this)
		*this = copy;
}
Server&	Server::operator=(const Server &copy) {
	if (&copy != this)
		;
	return *this;
}
