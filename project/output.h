#ifndef output_included
#define output_included

#define LED_RED BIT0               // P1.0
#define LED_GREEN BIT6             // P1.6
#define LEDS (LED_RED | LED_GREEN)


void buzzer_init(void);
void buzzer_set_period(short cycles);

void led_init(void);

#endif // included
