/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mait-elk <mait-elk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 14:29:28 by mait-elk          #+#    #+#             */
/*   Updated: 2024/11/17 17:03:18 by mait-elk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <iostream>
#include <poll.h>
#include <unistd.h>
#include <netinet/in.h>
#include <vector>

typedef struct {
	int	x;
	int	y;
}	PointI;


typedef struct {
	// enum {SET, ADD, EDIT} CMD;
	int				id;
	PointI			pos;
	int				color;
	int				rad;
} Player;

class Server {
private:
	int					ServerFD;
	std::vector<pollfd>	pfds;
	char				buffer[1024];
	Server(const Server &copy);
	Server& operator=(const Server &copy);
public:
				Server();
				~Server();
	void		StartServer();
	template<typename T>
	void	sendMsgToClient(int ClientFD, T *data, size_t size = sizeof(T)) {
		send(ClientFD, (char *)data, size, 0);
	}
	template<typename Type>
	Type ReceiveMsgFromClient(int ClientFD) {
		char buffer[1024];
		ssize_t bytes = 0;
		memset(buffer, 0, sizeof(buffer));
		bytes = recv(ClientFD, buffer, sizeof(buffer), 0);
		if (bytes <= 0) {
			throw std::runtime_error("NULL");
		}
		Type *r = (Type *)buffer;
		return (*r);
	}
};
