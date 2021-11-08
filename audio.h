#ifndef audio
#define audio
#define c 261
#define d 294
#define e 329
#define f 349
#define g 392
#define a 440
#define b 493
void audio_thread(void* Argument);
void iniAudio(void);
extern int musical_notes[];
extern int musical_end[];
#endif