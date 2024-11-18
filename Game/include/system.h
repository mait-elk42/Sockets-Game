#ifndef SYSTEM_H
#define SYSTEM_H
#include <stdio.h>
#include <libft.h>
#include <mlx.h>
#include <stdbool.h>
#include <math.h>
#include <macros.h>
#include <arpa/inet.h>
#include <sys/poll.h>


#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 6667

#define LENGTH 50

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


typedef struct {
	int		keycode;
	bool	pressed;
}	Key;

typedef struct {
	void	*ptr;
	int		*buffer;
	int		Width;
	int		Height;
	int 	bits_per_pixel;
	int 	size_line;
	int 	endian;
}	Texture;

typedef struct {
	PointI	position;
	int		color;
}	Object;

typedef struct {
	void	*MlxPtr;
	int		Win_Width;
	int		Win_Height;
	void	*WinPtr;
	Texture maintexture;
	Player	*player;
	Player	*players[1024];
	Key		keys[1024];
	bool mousekeypressed;

	int 				client_socket;
	struct sockaddr_in	server_address;
	char				buffer[BUFFER_SIZE];
	struct pollfd pfds[1];

	Texture t;
}	System;

System	*data_hook(System *tosave) ;

int		*readpixel(const Texture *t, PointI point) ;

void	init_server(System *system);
void	send_data_toserv(Player *data);
void	get_data_fromserv(Player *data);


int		ev_systemloop(System *system) ;
int		ev_keydown(int keycode, System *system) ;
int		ev_keyup(int keycode, System *system) ;
int		ev_mousekeydown(int button, int x, int y, System *system) ;
int		ev_mousekeyup(int button, int x, int y, System *system) ;


void	ProcessTexture(Texture *texture) ;
void	Init_Keys_Recorder(Key *keys) ;
void	RecordNewKey(Key *keys, Key torecord) ;

Key	read_key(int keycode) ;


#endif