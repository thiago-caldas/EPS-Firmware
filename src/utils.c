#include "inc/main.h"

enUtilsTypeDef UTILS_checkTimeout( uint32_t *tmt, uint32_t compareValue ){
  uint32_t currentTick = MAIN_getTick();
  
  if( currentTick < *tmt ){
    uint32_t diff = 0xFFFFFFFF - *tmt;
    
    if( (diff + currentTick) >= compareValue )
      return HAL_OK;
  }else{
    if( (currentTick - *tmt) >= compareValue )
      return HAL_OK;
  }
  
  return HAL_ERROR;
}
