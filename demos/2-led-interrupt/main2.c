//Alternate LEDs from Off, Green, and Red

#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {

  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;

  configureClocks();/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();/* enable periodic interrupt */
  or_sr(0x18);/* CPU off, GIE on */
}

short led_on = 0;
int wcount = 0;

void
__interrupt_vec(WDT_VECTOR) WDT()/* 250 interrupts/sec */
{
  wcount ++;
  if (wcount == 250){
    wcount = 0;
  
    if (led_on){
      led_on = 0;
      P1OUT &= ~LED_GREEN;
    } else{
      led_on = 1;
      P1OUT |= LED_GREEN;
    }
  }
} 
