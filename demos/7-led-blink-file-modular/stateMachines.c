#include <msp430.h>
#include "stateMachines.h"
#include "led.h"

char toggle_red()		/* always toggle! */
{
  static char state = 0;

  switch (state) {
  case 0:
    red_on = 1;
    state = 1;
    break;
  case 1:
    red_on = 0;
    state = 0;
    break;
  }
  return 1;			/* always changes an led */
}

char toggle_green()	/* only toggle green if red is on!  */
{
  char changed = 0;
  if (red_on) {
    green_on ^= 1;
    changed = 1;
  }
  return changed;
}


void state_advance()		/* alternate between toggling red & green */
{
  char changed = 0;  

  static char counter = 0;
  counter = (counter + 1) % 4;

  switch (counter) {
    case 0: green_on = 0; red_on = 0; break;
    case 1: green_on = 0; red_on = 1; break;
    case 2: green_on = 1; red_on = 0; break;
    case 3: green_on = 1; red_on = 1; break;
  }

  led_changed = 1;
  led_update();
}



