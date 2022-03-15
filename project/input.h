#ifndef input_included
#define input_included

#define SW1 BIT0		/* switch1 is p1.3 */
#define SW2 BIT1		/* switch1 is p1.3 */
#define SW3 BIT2		/* switch1 is p1.3 */
#define SW4 BIT3		/* switch1 is p1.3 */
#define SWITCHES (SW1 | SW2 | SW3 | SW4)		/* only 1 switch on this board */

void switch_interrupt_handler(void);
void switch_init(void);

#endif
