#include <msp430.h>
#include "toy.h"
#include "input.h"
#include "output.h"

static ToyState state = STATE_WAITING;

void toy_reset(void)
{ state = STATE_WAITING; 
//  led_red_off();
//  led_green_on();
  buzzer_stop();
}

void toy_play_preset(unsigned char preset)
{
  state = STATE_OUTPUTTING_MORSE;
//  led_green_off();
 // led_red_blink();
  buzzer_play_preset(preset);
}

void toy_listen(void)
{
  state = STATE_LISTENING_MORSE;
//  led_red_off();
//  led_green_blink();
  //morse_listen();
}

void toy_reject_input(void)
{
  state = STATE_REJECTED_MORSE;
//  led_red_on();
//  led_green_off();
  //buzzer_reject();
}

static char press_acknowledged = 0;
static char unpress_acknowledged = 0;
void button_pressed(char p2val)
{
  if (!(p2val & SW4) && !(press_acknowledged & SW4)) {
    // Button is pressed and not yet acknowledged
    press_acknowledged |= SW4;
    unpress_acknowledged &= ~SW4;
    toy_reset();
  } else if (!(p2val & SW1) && !(press_acknowledged & SW1)) {
    press_acknowledged |= SW1;
    unpress_acknowledged &= ~SW1;

    switch (state) {
      case STATE_WAITING:
        toy_listen();
        break;
      case STATE_LISTENING_MORSE:
        //morse_press_start();
        break;
    }
  } else if (!(p2val & SW2) && !(press_acknowledged & SW2)) {
    press_acknowledged |= SW2;
    unpress_acknowledged &= ~SW2;

    switch (state) {
      case STATE_WAITING:
        toy_play_preset(1);
        break;
    }
  } else if (!(p2val & SW3) && !(press_acknowledged & SW3)) {
    press_acknowledged |= SW3;
    unpress_acknowledged &= ~SW3;

    switch (state) {
      case STATE_WAITING:
        toy_play_preset(2);
        break;
    }
  }
}

void button_unpressed(char p2val)
{
  if (p2val & SW1 && !(unpress_acknowledged & SW1)) {
    // Button is unpressed and not yet acknowledged
    unpress_acknowledged |= SW1;
    press_acknowledged &= ~SW1;
    //morse_press_stop();
  } 
  if (p2val & SW2 && !(unpress_acknowledged & SW2)) {
    unpress_acknowledged |= SW2;
    press_acknowledged &= ~SW2;
  }
  if (p2val & SW3 && !(unpress_acknowledged & SW3)) {
    unpress_acknowledged |= SW3;
    press_acknowledged &= ~SW3;
  }
  if (p2val & SW4 && !(unpress_acknowledged & SW4)) {
    unpress_acknowledged |= SW4;
    press_acknowledged &= ~SW4;
  }
}
