#include <system.h>

int main() {
	System system = {
		.MlxPtr = mlx_init(),
		.Win_Height = 800,
		.Win_Width = 800,
		.WinPtr = mlx_new_window(system.MlxPtr, system.Win_Width, system.Win_Height, "Procedural Animation"),
		.maintexture = {
			.ptr = mlx_new_image(system.MlxPtr, system.Win_Width, system.Win_Height),
			.Width = system.Win_Width,
			.Height = system.Win_Height,
			.buffer = (int *)mlx_get_data_addr(system.maintexture.ptr, &system.maintexture.bits_per_pixel, &system.maintexture.size_line, &system.maintexture.endian),
		},
		.mousekeypressed = false,
	};
	data_hook(&system);
	system.t.ptr = mlx_xpm_file_to_image(system.MlxPtr, "plr.xpm", &system.t.Width, &system.t.Height);
	system.t.buffer = (int *) mlx_get_data_addr(system.t.ptr, &system.t.bits_per_pixel, &system.t.size_line, &system.t.endian);
	Init_Keys_Recorder(system.keys);
	init_server(&system);
	system.pfds[0].fd = system.client_socket;
    system.pfds[0].events = POLLIN;

	int poll_count = poll(system.pfds, 1, 1); // Timeout set to 100ms
	if (poll_count == -1) {
		perror("Poll failed");
		exit(19);
	}
	for (int i = 0; i < 1024; i++) {
		system.players[i] = NULL;
	}

	Player *me = malloc(sizeof(Player));
	if (system.pfds[0].revents & POLLIN) {
		ft_printf("new data received\n");
		Player myid;
		get_data_fromserv(&myid);
		me->id = myid.id;
	}

	me->pos = (PointI) {.x = me->id * 50, .y = me->id * 50};
	me->color = 0x00ff00;
	me->rad = 10;
	system.players[0] = me;

	send_data_toserv(me);

	printf("MY ID IS : %d\n", me->id);

	mlx_hook(system.WinPtr,  ON_KEYDOWN, 0, ev_keydown, &system);
	mlx_hook(system.WinPtr,  ON_KEYUP, 0, ev_keyup, &system);

	mlx_hook(system.WinPtr,  ON_MOUSEDOWN, 0, ev_mousekeydown, &system);
	mlx_hook(system.WinPtr,  ON_MOUSEUP, 0, ev_mousekeyup, &system);

	mlx_loop_hook(system.MlxPtr, ev_systemloop, &system);

	mlx_loop(system.MlxPtr);
}


