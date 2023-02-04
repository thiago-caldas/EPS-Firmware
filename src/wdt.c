#include "inc/main.h"

void WDT_init( void )
{
  WDT_stop();
}

void WDT_stop( void )
{
  WDTCTL = WDTPW + WDTHOLD;
}

void WDT_refresh( void )
{
  WDTCTL = WDT_ADLY_1000;
}
