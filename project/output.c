#include <msp430.h>
#include "libTimer.h"
#include "output.h"
#include "morse.h"
#include "toy.h"

#define MORSE_BUFFER_LENGTH 200
#define BUZZER_DIT_CYCLES 1000
#define BUZZER_DAH_CYCLES 300

#define LED_ON_INTERRUPTS 250
#define LED_OFF_INTERRUPTS 100

static char morse_buffer[MORSE_BUFFER_LENGTH];
static char *buffer_pos = 0;
static BuzzerState buzzer_state = BUZZER_OFF;
static unsigned int buffer_counter = 0;

void buzzer_init(void)
{
    /* 
       Direct timer A output "TA0.1" to P2.6.  
        According to table 21 from data sheet:
          P2SEL2.6, P2SEL2.7, anmd P2SEL.7 must be zero
          P2SEL.6 must be 1
        Also: P2.6 direction must be output
    */
    timerAUpmode();		/* used to drive speaker */
    P2SEL2 &= ~(BIT6 | BIT7);
    P2SEL &= ~BIT7; 
    P2SEL |= BIT6;
    P2DIR = BIT6;		/* enable output to speaker (P2.6) */
}

void buzzer_set_period(short cycles)
{
  CCR0 = cycles; 
  CCR1 = cycles >> 1;		/* one half cycle */
}

void buzzer_stop(void)
{
  buzzer_set_period(0);
  buzzer_state = BUZZER_OFF;
}

void buzzer_play_preset(unsigned char preset)
{
  switch (preset) {
    case 1:
      buzzer_play_message("why hello there");
      break;
    case 2:
      buzzer_play_message("enjoy this random message");
      break;
    case 3:
      buzzer_play_message("given enough eyeballs all bugs are shallow");
  }
}

void buzzer_play_message(char *msg)
{
  // Reset buffer
  morse_buffer[0] = '\0';
  buffer_pos = morse_buffer;
  buzzer_state = BUZZER_START;
  
  // Fill our buffer with the trasnlation
  morse_translate(msg, morse_buffer);
}

static inline int get_interrupts(char c)
{
  switch (c) {
    case '*':
      return get_dit_interrupts();
    case '-':
    case ';':
      return get_dah_interrupts();
    case ' ':
      return get_space_interrupts();
  }
}

void buzzer_timer_interrupt(void)
{
  if (buzzer_state == BUZZER_OFF)
    return; // Nothing to do
  
  buffer_counter++;
  
  if (buzzer_state == BUZZER_BETWEEN_PART && buffer_counter < get_interrupts('*')) {
    // Check if the space between parts is done
    return;
  } else if (buzzer_state == BUZZER_PLAYING && buffer_counter < get_interrupts(*buffer_pos)) {
    // Check if the period for everything else is done (dit, dah, word/letter space)
    return;
  }

  // Handle what happens at the end of a state (unless buzzer_start)
  // If we finished playing a character, we need to wait 1 unit before going to the next,
  // unless that character was a word/letter space.
  // If we finished a part space, we always go to the next character.
  switch (buzzer_state) {
    case BUZZER_START:
      // Starting our first character
      buzzer_state = BUZZER_PLAYING;
      break;
    case BUZZER_PLAYING:
      // Finished a character 
      if (*buffer_pos == ';' || *buffer_pos == ' ') {
        // space characters don't need an additional part space
        buffer_pos++;
      } else {
        // start part space
        buzzer_state = BUZZER_BETWEEN_PART;
        buffer_counter = 0;
        buzzer_set_period(0);
        led_green_on();
        led_red_off();
        return;
      }
      break;
    case BUZZER_BETWEEN_PART:
      // we always move to the next character after part space
      buzzer_state = BUZZER_PLAYING;
      buffer_pos++;
      break;
  }

  // If we have a space, we pause, otherwise we play
  switch (*buffer_pos) {
    case '*':
      buzzer_set_period(BUZZER_DIT_CYCLES);
      led_green_on();
      led_red_off();
      break;
    case '-':
      buzzer_set_period(BUZZER_DAH_CYCLES);
      led_green_off();
      led_red_on();
      break;
    case ' ':
    case ';':
      buzzer_set_period(0);
      led_green_on();
      led_red_on();
      break;
    case '\0':
      // End of message, go back to default state
      toy_reset();
  }

  // Reset counter after every action
  buffer_counter = 0;
}

static LedState led_green = LED_OFF;
static LedState led_red = LED_OFF;
static unsigned char led_counter = 0;
static unsigned char led_blink_on = 1;

void led_init(void)
{
  led_green = LED_OFF;
  led_red = LED_OFF;
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;		/* leds initially off */
}

void led_green_on(void)
{
  led_green = LED_ON;
  P1OUT |= LED_GREEN;
}

void led_green_off(void)
{
  led_green = LED_OFF;
  P1OUT &= ~LED_GREEN;
}

void led_green_blink(void)
{
  // Only one LED will ever blink at once
  if (led_red == LED_BLINK)
    led_red_off();

  led_green = LED_BLINK;
  // Reset blink counter
  led_counter = 0;
  // Signal that the blink state is set to ON
  led_blink_on = 1;
  P1OUT |= LED_GREEN;
}

void led_red_on(void)
{
  led_red = LED_ON;
  P1OUT |= LED_RED;
}

void led_red_off(void)
{
  led_red = LED_OFF;
  P1OUT &= ~LED_RED;
}

void led_red_blink(void) {
  if (led_green == LED_BLINK)
    led_green_off();

  led_red = LED_BLINK;
  led_counter = 0;
  led_blink_on = 1;
  P1OUT |= LED_RED;
}

inline static void led_blink_toggle(void)
{
  led_counter = 0;
  led_blink_on = !led_blink_on;
  // If we are blinking are green is set to blink,
  // we toggle green, otherwise toggle red
  P1OUT ^= (led_green == LED_BLINK ? LED_GREEN : LED_RED);
}

void led_timer_interrupt(void)
{
  // If no LEDS are blinking exit early
  if (led_green != LED_BLINK && led_red != LED_BLINK)
    return;

  // At most only one LED will ever be blinking
  if (led_blink_on && led_counter++ >= LED_ON_INTERRUPTS)
    // led is on and we passed our ON counter limit
    led_blink_toggle();
  else if (!led_blink_on && led_counter++ >= LED_OFF_INTERRUPTS)
    // led is off and we passed our OFF counter limit
    led_blink_toggle();
}
