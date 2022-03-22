#ifndef morse_included
#define morse_included

#define INTERRUPTS 250.0

// https://en.wikipedia.org/wiki/Morse_code
#define DIT_MS 100.0
#define DAH_MULTIPLIER 3
#define SPACE_MULTIPLIER 7
#define SECOND_IN_MS 1000.0
// DAH is always DIT * 3

// Convert dit in milliseconds to scale with our 250 interrupts/second
// The loss of precision from double to int is negligible
int get_dit_interrupts(void);
int get_dah_interrupts(void);
int get_space_interrupts(void);
void morse_translate(char *msg, char *buffer);
void morse_listen(void);
void morse_press_start(void);
void morse_press_stop(void);

#endif
