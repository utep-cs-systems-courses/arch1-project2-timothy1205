#ifndef states_included
#define states_included

typedef enum toy_states{ STATE_WAITING, STATE_LISTENING_MORSE, STATE_REJECTED_MORSE, STATE_OUTPUTTING_MORSE } ToyState;

/*
 * STATE_WAITING: Waiting for input. First button triggers inputting morse, the rest output
 * preconfigured messages in mourse. Solid green LED to show the machine is ready.
 * 
 * STATE_ACCEPTING_MORSE: Tracking one-word morse input. First button is used to accept the
 * morse input. Second button marks input as done. Third does nothing. Last cancels input and
 * goes back to waiting state. Green LED should blink (on, turns off momentarily)
 *
 * STATE_REJECTED_MORSE: The previous morse input does not match any preconfigured
 * messages. Solid red LED. Last button returns to waiting state.
 *
 * STATE_OUTPUTTING_MORSE: Outtputting preconfigured morse message, either from a button press or
 * manual morse input. Blinking red LED. Last button can cancel and return to waiting state.

 */

void toy_reset(void);
void toy_play_preset(unsigned char preset);
void toy_listen(void);
void toy_reject_input(void);
void button_pressed(char p2val);
void button_unpressed(char p2val);

#endif
