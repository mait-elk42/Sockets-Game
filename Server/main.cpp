#include "Server.hpp"

int main() {
	try {
		Server 				server;
		server.StartServer();
	}
	catch (std::exception &e) {
		
	}
	return 0;
}