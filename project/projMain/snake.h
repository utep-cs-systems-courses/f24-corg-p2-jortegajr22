#ifndef SNAKE_H
#define SNAKE_H

#include <msp430.h>

// Constants
#define SW1 1
#define SW2 2
#define SWITCHES (SW1 | SW2)

#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT 3

#define MOVE_STEP 5          // Number of pixels per movement
#define WDT_THRESHOLD 25     // WDT interrupt threshold (~10 Hz)

// Globals
extern int centerCol, centerRow;  // Current position of the shape
extern int prevCol, prevRow;      // Previous position of the shape
extern int currDirection;         // Current direction of movement
extern volatile int redrawFlag;   // Flag to indicate a redraw is needed

// Function Prototypes
void switch_init();
void switch_interrupt_handler();
void draw_shape(int color);
void update_position();
void update_shape();
void wdt_c_handler();

#endif /* SNAKE_H */
