# Morse Code Buzzer

### Project Description
This project is intended to act as a toy where a user can press a button to have a a precoded
messaged played to them in morse code, complete with flashing leds indicating different things.

On it's default 'waiting state' buttons 1-3 will each play a different message. Only one message can
play at a time. The fourth button will reset the toy back to its waiting state, halting an ongoing
message being played. The green LED will flash indicating it is ready to accept input.

While playing a message, the the buzzer will use different tones for dots (dits) and dashes (dahs).
When a dot is played, only the green LED will be solid. During a dash, only the red LED will be
solid. During a pause between a word or letter, both LEDs will be solid. During a pause between a
part, however, only the green LED will be solid.
