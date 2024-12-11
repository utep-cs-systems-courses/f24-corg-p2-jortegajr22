#include <msp430.h>
#include <libTimer.h>
#include "snake.h"
#include "lcdutils.h"
#include "lcddraw.h"

#define SW1 1
#define SW2 2
#define SWITCHES (SW1 | SW2)

#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3

#define MOVE_STEP 5  // Number of pixels per movement
#define WDT_THRESHOLD 25  // WDT interrupt threshold (~10 Hz)

int centerCol, centerRow;  // Current position of the shape
int prevCol, prevRow;      // Previous position of the shape
int currDirection = DIR_RIGHT;  // Initial direction
volatile int redrawFlag = 0;

// Switch initialization and handlers
static char switch_update_interrupt_sense() {
  char p2val = P2IN;
  P2IES |= (p2val & SWITCHES);  // If switch up, sense down
  P2IES &= (p2val | ~SWITCHES); // If switch down, sense up
  return p2val;
}

void switch_init() {
  P2REN |= SWITCHES;   // Enable resistors for switches
  P2IE |= SWITCHES;    // Enable interrupts for switches
  P2OUT |= SWITCHES;   // Pull-ups for switches
  P2DIR &= ~SWITCHES;  // Set switches as input
  switch_update_interrupt_sense();
}

void switch_interrupt_handler(){
  char p2val = switch_update_interrupt_sense();
  char switches = ~p2val & SWITCHES;

  // Rotate vector on button
  if (switches & SW1){
    currDirection = (currDirection -1 +4) % 4;
  }
  else if (switches & SW2) {
    currDirection = (currDirection + 1) % 4;
  }
}

// Draw and erase the shape
void draw_shape(int color) {
  unsigned int drawColor = (color == 1) ? COLOR_YELLOW : COLOR_BLUE;
  fillRectangle((color ? centerCol : prevCol), (color ? centerRow : prevRow), 8, 8, drawColor);
}

void update_position() {
  // Save current position as previous
  prevCol = centerCol;
  prevRow = centerRow;

  // Update position based on current direction
  switch (currDirection) {
  case DIR_UP:    centerRow -= MOVE_STEP; break;
  case DIR_RIGHT: centerCol += MOVE_STEP; break;
  case DIR_DOWN:  centerRow += MOVE_STEP; break;
  case DIR_LEFT:  centerCol -= MOVE_STEP; break;
  }

  // Boundary checks to wrap around screen
  if (centerCol < 0) centerCol = screenWidth - MOVE_STEP;
  if (centerCol >= screenWidth) centerCol = 0;
  if (centerRow < 0) centerRow = screenHeight - MOVE_STEP;
  if (centerRow >= screenHeight) centerRow = 0;

  redrawFlag = 1;  // Signal redraw
}

void update_shape() {
  draw_shape(0);  // Erase old shape
  draw_shape(1);  // Draw new shape
}

// WDT interrupt handler
void wdt_c_handler() {
  static int secCount = 0;
  secCount++;

  if (secCount >= WDT_THRESHOLD) {
    secCount = 0;
    update_position();  // Move shape continuously
  }
}// Main program
int main() {
  configureClocks();
  lcd_init();
  switch_init();

  centerCol = screenWidth / 2;  // Initialize position at screen center
  centerRow = screenHeight / 2;
  prevCol = centerCol;
  prevRow = centerRow;

  enableWDTInterrupts();
  or_sr(0x8);  // Enable interrupts

  clearScreen(COLOR_BLUE);
  draw_shape(1);  // Draw initial shape

  while (1) {
    if (redrawFlag) {
      redrawFlag = 0;
      update_shape();  // Redraw shape
    }
    or_sr(0x10);  // Enter low-power mode
  }
}

// Interrupt vectors
void __interrupt_vec(PORT2_VECTOR) Port_2() {
  if (P2IFG & SWITCHES) {
    P2IFG &= ~SWITCHES;  // Clear pending switch interrupts
    switch_interrupt_handler();
    __bic_SR_register_on_exit(LPM0_bits);  // Exit low-power mode
  }
}
void __interrupt_vec(WDT_VECTOR) WDT() {
  wdt_c_handler();
  __bic_SR_register_on_exit(LPM0_bits);  // Exit low-power mode
}
