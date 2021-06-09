#ifndef SB16_H
#define SB16_H

int init_sb16();
void play_sound(void* addr, int size, int volume, int samplerate, int channels);

#endif