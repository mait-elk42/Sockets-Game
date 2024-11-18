#include <system.h>

void	Init_Keys_Recorder(Key *keys) {
	for (int i = 0; i < 1024; i++) {
		keys[i] = (Key){
			.pressed = false,
			.keycode = -1
		};
	}
	RecordNewKey(keys, (Key) {
		.pressed = false,
		.keycode = KEY_A
	});
}

void RecordNewKey(Key *keys, Key torecord) {
	for (int i = 0; i < 1024; i++) {
		if (keys[i].keycode == -1) {
			keys[i] = torecord;
			break ;
		}
	}
}