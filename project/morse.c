#include "morse.h"

inline int get_dit_interrupts(void)
{
  return DIT_MS / 1000 * INTERRUPTS;
}

inline int get_dah_interrupts(void)
{
  return get_dit_interrupts() * 3;
}



