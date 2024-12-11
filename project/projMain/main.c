#include <msp430.h>
#include <libTimer.h>
#include "main.h"
#include "lcdutils.h"
#include "lcddraw.h"
#include "led.h"

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES (SW1 | SW2 | SW3 | SW4)
#define STATE_IDLE 0
#define STATE_MOVE_LEFT 1
#define STATE_MOVE_RIGHT 2

int col = 0, row = 0;
int centerCol, centerRow;
int prevCol;
int redrawFlag = 0;
int currState = STATE_IDLE;


static char
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch iinterrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);   /* if switch up, sense down */
  P2
    IES &= (p2val | ~SWITCHES);  /* if switch down, sense up */
  return p2val;
}

void
switch_init()
{
  P2REN |= SWITCHES;             /* enables resistors for switches */
  P2IE |= SWITCHES;              /* enable interrupts from switches */
  P2OUT |= SWITCHES;             /* pull-ups for switches */
  P2DIR &= ~SWITCHES;            /* set switches' bits for input */

  switch_update_interrupt_sense();
}
int switches = 0;

void
switch_interrupt_handler()
{
  char p2val = switch_update_interrupt_sense();
  switches = ~p2val & SWITCHES;
 
  // Adjust position based on button press
  if (switches & SW1) {  // Move left
    if (centerCol > 10){
       currState = STATE_MOVE_LEFT;
       
    }
  }
  if (switches & SW2) {  // Move right
    if (centerCol < screenWidth - 3){
       currState = STATE_MOVE_RIGHT;
    }
  }
}

void
draw_shape(int color)
{
  unsigned int drawColor = (color == 1) ? COLOR_YELLOW : COLOR_BLUE;      
  fillRectangle((color ? centerCol : prevCol), centerRow, 8, 8, drawColor);
}    
    
    

void
wdt_c_handler(){
  static int secCount = 0;
  secCount++;
  //P1OUT ^= LEDS;
  if (secCount >= 50){
    secCount = 0;
      
    //Handle shape movement
     if (currState == STATE_MOVE_LEFT){
      if (centerCol > 10){
	
	prevCol = centerCol;
	centerCol -= 5;
	redrawFlag = 1;
	//P1OUT ^= LED_RED;
      }
    }
    if (currState == STATE_MOVE_RIGHT){
      prevCol = centerCol;
      centerCol += 5;
      redrawFlag = 1;
      //P1OUT ^=LED_RED;
    }
    if (redrawFlag ==1){
      P1OUT ^= LED_RED;
      P1OUT ^= LED_RED;
    }
      //reset to idle
     currState = STATE_IDLE;
    
  }
}
int
main()
{
  P1DIR |= LEDS;
  P1OUT &= ~LED_GREEN;
  P1OUT |= LED_RED;
  
  configureClocks();
  lcd_init();
  switch_init();

  centerCol = screenWidth / 2;
  centerRow = screenHeight / 2;
  prevCol = centerCol;

  enableWDTInterrupts();
  or_sr(0x8);
  

  clearScreen(COLOR_BLUE);
  update_shape();  
  while (1) {
    
    if (redrawFlag == 1) {
      redrawFlag = 0;
      update_shape();
    
      
    }
    
    P1OUT &= LED_GREEN;
     or_sr(0x10);       // CPU OFF
    P1OUT |= LEDS;
  }
}
void
update_shape()
{
  draw_shape(0);

  draw_shape(1);

}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
    if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */
      P2IFG &= ~SWITCHES;          /* clear pending sw interrupts */
      
      switch_interrupt_handler();  /* single handler for all switches */
      __bic_SR_register_on_exit(LPM0_bits);
  }
}
void
__interrupt_vec(WDT_VECTOR) WDT(){
  wdt_c_handler();
  
  P1OUT &= ~LED_GREEN;
  }

