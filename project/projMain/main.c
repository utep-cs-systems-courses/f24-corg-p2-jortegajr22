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

#define SWITCHES 15


int col = 0, row = 0, int currCol = 0;
int centerCol = width/2, centerRow = height-12;

static char
switch_update_interupt_sense()
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
draw_shape()
{
  for (row; row < 5; row ++){
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
}

int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLUE);

  draw_shape();
}

void
__interupt_vec(PORT2_VECTOR) Port_2(){
    if (P2IFG & SWITCHES) {      /* did a button cause this interrupt? */
    P2IFG &= ~SWITCHES;          /* clear pending sw interrupts */
    switch_interrupt_handler();  /* single handler for all switches */
  }
}
