#ifndef MAIN_H
#define MAIN_H

#include <msp430.h>
#include "lcdutils.h"
#include "lcddraw.h"

/* Definitions *
#define LED BIT6

/* Switch definitions */
#define SW1 1
#define SW2 2
#define SW3 4
#define SW4 8
#define SWITCHES (SW1 | SW2 | SW3 | SW4)  /* Combines all switches */

/* Variables */
extern int col, row, currCol;          /* Used for drawing */
extern int centerCol, centerRow;       /* Center of the shape */
extern int switches;                   /* Holds the state of switches */

/* Function prototypes */
void switch_init();                    /* Initialize switches */
static char switch_update_interrupt_sense(); /* Update switch interrupt sense */
void draw_shape(int color);                     /* Draw the desired shape */
void switch_interrupt_handler();       /* Handle switch events */

#endif /* MAIN_H */


