#ifndef morse_included
#define morse_included

#define INTERRUPTS 250

// https://en.wikipedia.org/wiki/Morse_code
#define DIT_MS 60
// DAH is always DIT * 3

// Convert dit in milliseconds to scale with our 250 interrupts/second
// The loss of precision from double to int is negligible
int get_dit_interrupts(void);
int get_dah_interrupts(void); // dit * 3


#endif
