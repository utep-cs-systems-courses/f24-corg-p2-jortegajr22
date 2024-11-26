#include <msp430.h>
#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"

int
main()
{
  configureClocks();
  lcd_init();
  u_char width = screenWidth, height = screenHeight;

  clearScreen(COLOR_BLUE);

  int col = 0, row = 0;
  int centerCol = width/2, centerRow = height-12;
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
}
