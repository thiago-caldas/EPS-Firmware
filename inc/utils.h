#ifndef _UTILS_H_
#define _UTILS_H_

#define __VAL_1_SEGUNDO__       (1000U)
#define __VAL_500_MS__          (500U)

typedef enum enUtilsTypeDef{
    HAL_OK       = 0x00,
    HAL_ERROR    = 0x01,
    HAL_BUSY     = 0x02,
    HAL_TIMEOUT  = 0x03,
    HAL_PENDING  = 0x04
} enUtilsTypeDef;

/* Prototipos */

enUtilsTypeDef UTILS_checkTimeout( uint32_t *tmt, uint32_t compareValue );

#endif//_UTILS_H_


