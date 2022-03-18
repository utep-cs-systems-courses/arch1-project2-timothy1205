#ifndef output_included
#define output_included

#define LED_RED BIT0               // P1.0
#define LED_GREEN BIT6             // P1.6
#define LEDS (LED_RED | LED_GREEN)


void buzzer_init(void);
void buzzer_set_period(short cycles);

#define LED_ON_INTERRUPTS 250
#define LED_OFF_INTERRUPTS 250
typedef enum LedState{ LED_ON, LED_OFF, LED_BLINK } LedState;
void led_init(void);
void led_green_on(void);
void led_green_off(void);
void led_green_blink(void);
void led_red_on(void);
void led_red_off(void);
void led_red_blink(void);

#endif // included
