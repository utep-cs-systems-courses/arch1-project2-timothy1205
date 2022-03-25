#include <string.h>
#include "morse.h"
#define DICTIONARY_OFFSET 97

// * for dit, - for dah, ; for space between letters
static char dictionary[26][5] = {
  {"*-;"}, // a
  {"-***;"}, // b
  {"-*-*;"}, // c
  {"-**;"}, // d
  {"*;"}, // e
  {"**-*;"}, // f
  {"--*;"}, // g
  {"****;"}, // h
  {"**;"}, // i
  {"*---;"}, // j
  {"-*-;"}, // k
  {"*-**;"}, // l
  {"--;"}, // m
  {"-*;"}, // n
  {"---;"}, // o
  {"*--*;"}, // p
  {"--*-;"}, // q
  {"*-*;"}, // r
  {"***;"}, // s
  {"-;"}, // t
  {"**-;"}, // u
  {"***-;"}, // v
  {"*--;"}, // w
  {"-**-;"}, // x
  {"-*--;"}, // y
  {"--**;"}  // z
};

void morse_translate(char *msg, char *buffer)
{
  while (*msg != '\0') {
    // a starts at 97
    unsigned char i = *msg - DICTIONARY_OFFSET;

    if (*msg == ' ') {
      // Preserve spaces
      strcat(buffer, " ");
    } else {
      if (i > 26)
	return; // Shouldn't be possible
      
      // Append dictionary translation to buffer
      strcat(buffer, dictionary[i]);
    }

    msg++;
  }
}

int get_dit_interrupts(void) {
  return (float) (DIT_MS / SECOND_IN_MS * INTERRUPTS);
}

int get_dah_interrupts(void) {
  return get_dit_interrupts() * DAH_MULTIPLIER;
}

int get_space_interrupts(void) {
  return get_dit_interrupts() * SPACE_MULTIPLIER;
}
