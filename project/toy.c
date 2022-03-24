#include <msp430.h>
#include "toy.h"
#include "input.h"
#include "output.h"

static ToyState state = STATE_WAITING;

void toy_reset(void)
{ 
  state = STATE_WAITING; 
  led_red_off();
  led_green_blink();
  buzzer_stop();
}

void toy_play_preset(unsigned char preset)
{
  state = STATE_OUTPUTTING_MORSE;
  led_green_off();
  led_red_off();
  buzzer_play_preset(preset);
}

static char press_acknowledged = 0;
static char unpress_acknowledged = 0;
void button_pressed(char p2val)
{
  press_acknowledged = 0;
  if (!(p2val & SW4)) {
    toy_reset();
  } else if (!(p2val & SW1)) {

    switch (state) {
      case STATE_WAITING:
        toy_play_preset(1);
        break;
    }
  } else if (!(p2val & SW2)) {

    switch (state) {
      case STATE_WAITING:
        toy_play_preset(2);
        break;
    }
  } else if (!(p2val & SW3)) {

    switch (state) {
      case STATE_WAITING:
        toy_play_preset(3);
        break;
    }
  }
}

void button_unpressed(char p2val)
{
  /*if (p2val & SW1 && !(unpress_acknowledged & SW1)) {
    // Button is unpressed and not yet acknowledged
    unpress_acknowledged |= SW1;
    press_acknowledged &= ~SW1;
  } else if (p2val & SW2 && !(unpress_acknowledged & SW2)) {
    unpress_acknowledged |= SW2;
    press_acknowledged &= ~SW2;
  } else if (p2val & SW3 && !(unpress_acknowledged & SW3)) {
    unpress_acknowledged |= SW3;
    press_acknowledged &= ~SW3;
  } else if (p2val & SW4 && !(unpress_acknowledged & SW4)) {
    unpress_acknowledged |= SW4;
    press_acknowledged &= ~SW4;
  }*/
}
