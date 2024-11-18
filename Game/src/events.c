#include <system.h>

int ev_systemloop(System *system) {
	ProcessTexture(&system->maintexture);
	mlx_clear_window(system->MlxPtr, system->WinPtr);
	mlx_put_image_to_window(system->MlxPtr, system->WinPtr, system->maintexture.ptr, 0, 0);

	PointI mousepos; mlx_mouse_get_pos(system->WinPtr, &mousepos.x, &mousepos.y);
	if (mousepos.x >= 0 && mousepos.x <= system->Win_Width && mousepos.y >= 0 && mousepos.y <= system->Win_Height && system->mousekeypressed == true) {
		Player t;
		t.pos.x = mousepos.x;
		t.pos.y = mousepos.y;
		system->players[0]->pos = mousepos;
		send_data_toserv(system->players[0]);
	} else {
		// system->ppp = (PointI){system->maintexture.Width/2, system->maintexture.Height/2};
	}
	return (0);
}

int ev_keydown(int keycode, System *system) {
	for (int i = 0; system->keys[i].keycode != -1; i++) {
		if (system->keys[i].keycode == keycode) {
			system->keys[i].pressed = true;
			break ;
		}
	}
	return (0);
}

int ev_keyup(int keycode, System *system) {
	for (int i = 0; system->keys[i].keycode != -1; i++) {
		if (system->keys[i].keycode == keycode) {
			system->keys[i].pressed = false;
			break ;
		}
	}
	return (0);
}

int		ev_mousekeydown(int button, int x, int y, System *system) {(void)x;(void)y;
	if (button == 1)
		system->mousekeypressed = true;
	return 0;
}
int		ev_mousekeyup(int button, int x, int y, System *system) {(void)x;(void)y;
	if (button == 1)
		system->mousekeypressed = false;
	return 0;
}