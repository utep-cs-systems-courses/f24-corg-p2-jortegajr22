#include <msp430.h>
#include "libTimer.h"
#include "buzzer.h"
#include "switches.h"

static short period = 1000;
char sound_enabled = 1;

int main() {
    configureClocks();
    switch_init();
    buzzer_init();
    buzzer_set_period(1000);	/* start buzzing!!! 2MHz/1000 = 2kHz*/
    enableWDTInterrupts();

    or_sr(0x18);          // CPU off, GIE on
}

void
__interrupt_vec(WDT_VECTOR) WDT(){
  static short delay_counter = 0;
  
  if (sound_enabled){
    delay_counter++;
    if (delay_counter >= 250/2){
      delay_counter = 0;
      period += 100;
      if (period > 2000 || period < 500){
	period = 1000;
    }

    buzzer_set_period(period);
  }
  } else {
    buzzer_set_period(0);
  }
 }

void
__interrupt_vec(PORT1_VECTOR) Port_1() {
  if (P1IFG & SWITCHES) {
    P1IFG &= ~SWITCHES;
    switch_interrupt_handler();
    if(switch_state_down){
      sound_enabled = !sound_enabled;
    }
  }
}
