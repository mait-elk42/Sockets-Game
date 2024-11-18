#include <system.h>

Key	read_key(int keycode) {
	System *system = data_hook(NULL);
	for (int i = 0; system->keys[i].keycode != -1; i++) {
		if (system->keys[i].keycode == keycode) {
			return system->keys[i];
		}
	}
	return (Key){-1,false};
}