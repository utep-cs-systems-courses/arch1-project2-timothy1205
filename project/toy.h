#ifndef states_included
#define states_included

typedef enum toy_states{ STATE_WAITING, STATE_OUTPUTTING_MORSE } ToyState;

/*
 * STATE_WAITING: Waiting for input. First button triggers inputting morse, the rest output
 * preconfigured messages in mourse. Blinking green LED to show the machine is ready.
 * 
 * STATE_OUTPUTTING_MORSE: Outtputting preconfigured morse message, either from a button press or
 * manual morse input. LEDS controlled by buzzer state (current morse code) Last button can cancel and return to waiting state.

 */

void toy_reset(void);
void toy_play_preset(unsigned char preset);
void button_pressed(char p2val);
void button_unpressed(char p2val);

#endif
