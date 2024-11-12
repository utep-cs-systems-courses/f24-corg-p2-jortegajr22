#include <msp430.h>
#include "led.h"

void blinkUpdate(int blinkLimit)
{
  static int blinkCount = 0;
  blinkCount ++;
  if (blinkCount >= blinkLimit){
    blinkCount = 0;
    greenControl(1);
  } else
    greenControl(0);
}
