#ifndef output_included
#define output_included

#define LED_RED BIT6               // P1.6
#define LED_GREEN BIT0             // P1.0
#define LEDS (LED_RED | LED_GREEN)
typedef enum BuzzerState{ BUZZER_PLAYING, BUZZER_START, BUZZER_BETWEEN_PART, BUZZER_OFF } BuzzerState;
void buzzer_init(void);
void buzzer_set_period(short cycles);
void buzzer_stop(void);
void buzzer_play_preset(unsigned char preset);
void buzzer_play_message(char *msg);
void buzzer_timer_interrupt(void);

typedef enum LedState{ LED_ON, LED_OFF, LED_BLINK } LedState;
void led_init(void);
void led_green_on(void);
void led_green_off(void);
void led_green_blink(void);
void led_red_on(void);
void led_red_off(void);
void led_red_blink(void);
void led_timer_interrupt(void);

#endif // included
