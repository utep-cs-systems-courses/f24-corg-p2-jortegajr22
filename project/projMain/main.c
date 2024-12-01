#include <msp430.h>
#include <libTimer.h>
#include "main.h"
#include "lcdutils.h"
#include "lcddraw.h"

#define LED BIT6

#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8

#define SWITCHES (SW1 | SW2 | SW3 | SW4)  /* Combines all switches */

int col = 0, row = 0, currCol = 0;
int centerCol = 64, centerRow = 80;
int redrawFlag = 0;
char drawCol = 0, drawRow = 0, stepCol = 0;

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
  P2DIR &= ~SWITCHES            ;/* set switches' bits for input */
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
    centerCol -= 3;
    if (centerCol < 3) centerCol = 3; // Prevent going off-screen
    redrawFlag = 1;
  }
  if (switches & SW2) {  // Move right
    centerCol += 3;
    if (centerCol > screenWidth - 3) centerCol = screenWidth - 3; // Prevent going off-screen
    redrawFlag = 1;
  }
}

void
draw_shape(int color)
{
  if (color == 1){
      for (row = 0; row < 5; row ++){
	for (col = -row; col <= row/4; col ++)
	  drawPixel(centerCol + col, centerRow + row, COLOR_YELLOW);
	for (; col <= row; col++)
	  drawPixel(centerCol + col, centerRow + row, COLOR_RED);
      }
      for (; row < 10; row ++){
	for (col = -row; col <= row/4; col += 2)
	  drawPixel(centerCol + col, centerRow + row, COLOR_YELLOW);
	for (; col <= row; col++)
	  drawPixel(centerCol + col, centerRow +row, COLOR_RED);
      }
      drawCol += 3; drawRow += 3;
  }else{
    for (row = 0; row < 5; row++) {
      for (col = -row; col <= row; row++){
	drawPixel(centerCol + col, centerRow +row, COLOR_BLUE);
      }
    }
  }
}

int
main()
{
  configureClocks();
  lcd_init();
  switch_init();

  enableWDTInterrupts();
  or_sr(0x8);
  

  clearScreen(COLOR_BLUE);
  draw_shape(1);

  while (1) {
    if (redrawFlag) {  // Check if redraw is necessary
      draw_shape(0);   //erase old shape
      draw_shape(1);   // redraw
    }
    or_sr(0x10);       // CPU OFF
  }
}

void
__interrupt_vec(PORT2_VECTOR) Port_2(){
    if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;          /* clear pending sw interrupts */
    switch_interrupt_handler();  /* single handler for all switches */
  }
}
