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

int col = 0, row = 0, currCol = 0;
int centerCol, centerRow;
int redrawFlag = 0;
//char drawCol = 0, drawRow = 0, stepCol = 0;

static char
switch_update_interrupt_sense()
{
  char p2val = P2IN;
  /* update switch iinterrupt to detect changes from current buttons */
  P2IES |= (p2val & SWITCHES);   /* if switch up, sense down */
  P2IES &= (p2val | ~SWITCHES);  /* if switch down, sense up */
  return p2val;
}

void
switch_init()
{
  P2REN |= SWITCHES;             /* enables resistors for switches */
  P2IE |= SWITCHES;              /* enable interrupts from switches */
  P2OUT |= SWITCHES;             /* pull-ups for switches */
  P2DIR &= ~SWITCHES;            /* set switches' bits for input */
/*  while (1) {
    if (!(P2IN & SW1)) {  // Check if SW1 is pressed
      P1OUT ^= LED_RED;
    }
    }*/
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
    
    if (centerCol > 3){
      centerCol -= 3; // Prevent going off-screen
      redrawFlag = 1;
      P1OUT ^= LED_GREEN; // Turn on green LED for debug
      draw_shape(0);
      draw_shape(1);
    }
  }
  if (switches & SW2) {  // Move right
    
    if (centerCol < screenWidth - 3){
      centerCol += 3; // Prevent going off-screen
      redrawFlag = 1;
      draw_shape(0);
      draw_shape(1);
      P1OUT ^= LED_GREEN; // Turn on green LED for debug
    }
  }
}

void
draw_shape(int color)
{
  unsigned int drawColor = (color == 1) ? COLOR_YELLOW : COLOR_BLUE;
  
  for (row = 0; row < 5; row++) {
    for (col = -row; col <= row; col++) {
      drawPixel(centerCol + col, centerRow + row, drawColor);
    }
  }
  for (; row < 10; row++) {
    for (col = -row; col <= row; col+=2) {
      drawPixel(centerCol + col, centerRow + row, drawColor);
    }
    }
  //drawPixel(screenWidth /2, screenHeight / 2, COLOR_RED);
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

  enableWDTInterrupts();
  or_sr(0x8);
  

  clearScreen(COLOR_BLUE);
  draw_shape(1);

  while (1) {
    if (redrawFlag) {
      redrawFlag = 0;// Check if redraw is necessary
      // draw_shape(0);   //erase old shape
      //draw_shape(1);   // redraw
      
      // __delay_cycles(500000);
      P1OUT &= ~LED_RED;  // Turn off green LED after processing
      
    }
    // P1OUT &= ~LEDS;
    or_sr(0x10);       // CPU OFF
    //P1OUT |= LEDS;
  }
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
    if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;          /* clear pending sw interrupts */
    // P1OUT ^= LED_GREEN;
    switch_interrupt_handler();  /* single handler for all switches */
  }
}
void __interrupt_vec(WDT_VECTOR) WDT(){
  // Toggle LED on each WDT interrupt

}

