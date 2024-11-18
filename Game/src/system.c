#include <system.h>

System *data_hook(System *tosave) {
	static System	*saved;
	if (tosave != NULL)
		saved = tosave;
	return saved;
}

int *readpixel(const Texture *t, PointI point) {
	return (&t->buffer[point.y * t->Width + point.x]);
}

void send_data_toserv(Player *data) {
	System *system = data_hook(NULL);
	if (data != NULL) {
		if (send(system->client_socket, (char *)data, sizeof(Player), 0) < 0) {
			perror("Failed to send message");
			close(system->client_socket);
			exit(EXIT_FAILURE);
		}
	}
}

void get_data_fromserv(Player *data) {
	System *system = data_hook(NULL);
	if (data != NULL) {
		if (recv(system->client_socket, (char *)data, sizeof(Player), 0) < 0) {
			perror("Failed to get message");
			close(system->client_socket);
			exit(EXIT_FAILURE);
		}
	}
}

void init_server(System *system) {
	system->client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (system->client_socket < 0) {
		perror("Socket creation failed");
		exit(EXIT_FAILURE);
	}

	system->server_address.sin_family = AF_INET;
	system->server_address.sin_port = htons(SERVER_PORT);

	 if (inet_pton(AF_INET, SERVER_IP, &system->server_address.sin_addr) <= 0) {
		perror("Invalid address/Address not supported");
		close(system->client_socket);
		exit(EXIT_FAILURE);
	}

	if (connect(system->client_socket, (struct sockaddr *)&system->server_address, sizeof(system->server_address)) < 0) {
		perror("Connection failed");
		close(system->client_socket);
		exit(EXIT_FAILURE);
	}
}

void drawcircle(const Texture *texture, int radius, PointI position, int color) {
	// Draw a circle using the equation >>>>>>>> x^2 + y^2 = r^2
	int y = position.y-radius;
	int x = 0;
	while (y < position.y+radius) {
		x = position.x-radius;
		while (x < position.x+radius) {
			int dx = x - position.x;
			int dy = y - position.y;
			if (!(x < 0 || x >= texture->Width || y < 0 || y >= texture->Height))
				if (dx * dx + dy * dy <= radius * radius)
					*readpixel(texture, (PointI){x, y}) = color;
			x++;
		}
		y++;
	}
}

float PointFdistance(PointI p1, PointI p2) {
	(void)p1;
	(void)p2;
	return (sqrt(pow(abs(p2.x - p1.x), 2) + pow(abs(p2.y - p1.y), 2)));
}

void drawline(Texture *texture, PointI start, PointI end, int color, int thickness) {
	int dx = abs(end.x - start.x);
	int dy = abs(end.y - start.y);
	int sx = (start.x < end.x) ? 1 : -1;
	int sy = (start.y < end.y) ? 1 : -1;
	int err = dx - dy;

	while (1) {
		// Draw the main pixel and add thickness by drawing a circle around it
		for (int i = -thickness / 2; i <= thickness / 2; i++) {
			for (int j = -thickness / 2; j <= thickness / 2; j++) {
				// Calculate the offset point and check within the texture bounds
				PointI offset = {start.x + i, start.y + j};
				if (offset.x >= 0 && offset.x < texture->Width && offset.y >= 0 && offset.y < texture->Height) {
					*readpixel(texture, offset) = color;
				}
			}
		}

		// Check if we've reached the endpoint
		if (start.x == end.x && start.y == end.y) break;

		// Bresenham's algorithm error handling
		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			start.x += sx;
		}
		if (e2 < dx) {
			err += dx;
			start.y += sy;
		}
	}
}

Player *find_plr(Player *players[1024], int id) {
	Player *res = NULL;
	int i = 0;
	while (players[i] != NULL) {
		if (players[i]->id == id) {
			res = players[i];
			break;
		}
		i++;
	}
	return (res);
}

void	addplayer(Player *players[1024], Player *ptoadd) {
	for (int i = 0; i < 1024; i++) {
		if (players[i] == NULL) {
			players[i] = ptoadd;
			break;
		}
	}
}

void	draw_image(Texture *texture, Texture t, PointI pos) {
	for (int x = 0; x < t.Width; x++) {
		for (int y = 0; y < t.Height; y++) {
			if (*readpixel(&t, (PointI){x , y}) > 0)
				*readpixel(texture, (PointI){x +pos.x, y+pos.y}) = * readpixel(&t, (PointI){x , y});
		}
	}
}

void	ProcessTexture(Texture *texture) {
	System *system = data_hook(NULL);
	{
		for (int y = 0; y < texture->Height; y++) {
			for (int x = 0; x < texture->Width; x++) {
				*readpixel(texture, (PointI){x, y}) = 0x222222;
			}
		}
	}
	int poll_count = poll(system->pfds, 1, 1 /*TIME OUT ms*/ );
	if (poll_count == -1) {
		perror("Poll failed");
		exit(19);
	}
	
	if (system->pfds[0].revents & POLLIN) {
		Player *p = malloc(sizeof(Player));
		get_data_fromserv(p);
		Player *findp = find_plr(system->players, p->id);
		if (findp != NULL) {
			// ft_printf("player moved : %d, x%d, y%d\n",  p->id, p->pos.x, p->pos.y);
			findp->pos = p->pos;
		}
		else {
			addplayer(system->players, p);
			ft_printf("new player added : %d\n",  p->id);
		}
	}
	
	
	draw_image(texture, system->t, (PointI){100, 100});
	int i = 0;
	while (system->players[i] != NULL) {
		drawcircle(texture, system->players[i]->rad, system->players[i]->pos, system->players[i]->color);
		i++;
	}
}
