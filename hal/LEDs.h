#ifndef LEDS_H_
#define LEDS_H_

#if defined(Landungsbruecke) || defined(LandungsbrueckeSmall)
	#define LED_ON()            *HAL.IOs->pins->LED_STAT.resetBitRegister   = HAL.IOs->pins->LED_STAT.bitWeight
	#define LED_OFF()           *HAL.IOs->pins->LED_STAT.setBitRegister     = HAL.IOs->pins->LED_STAT.bitWeight
	#define LED_TOGGLE()        HAL.IOs->pins->LED_STAT.GPIOBase->PTOR      ^= GPIO_PTOR_PTTO(HAL.IOs->pins->LED_STAT.bitWeight)

	#define LED_ERROR_ON()      *HAL.IOs->pins->LED_ERROR.resetBitRegister  = HAL.IOs->pins->LED_ERROR.bitWeight
	#define LED_ERROR_OFF()     *HAL.IOs->pins->LED_ERROR.setBitRegister    = HAL.IOs->pins->LED_ERROR.bitWeight
	#define LED_ERROR_TOGGLE()  HAL.IOs->pins->LED_ERROR.GPIOBase->PTOR     ^= GPIO_PTOR_PTTO(HAL.IOs->pins->LED_ERROR.bitWeight)
#elif defined(LandungsbrueckeV3)
	#define LED_ON()            *HAL.IOs->pins->LED_STAT.resetBitRegister   = HAL.IOs->pins->LED_STAT.bitWeight
	#define LED_OFF()           *HAL.IOs->pins->LED_STAT.setBitRegister     = HAL.IOs->pins->LED_STAT.bitWeight
	#define LED_TOGGLE()         GPIO_TG(HAL.IOs->pins->LED_STAT.port)      = HAL.IOs->pins->LED_STAT.bitWeight

	#define LED_ERROR_ON()      *HAL.IOs->pins->LED_ERROR.resetBitRegister  = HAL.IOs->pins->LED_ERROR.bitWeight
	#define LED_ERROR_OFF()     *HAL.IOs->pins->LED_ERROR.setBitRegister    = HAL.IOs->pins->LED_ERROR.bitWeight
	#define LED_ERROR_TOGGLE()  GPIO_TG(HAL.IOs->pins->LED_ERROR.port)      = HAL.IOs->pins->LED_ERROR.bitWeight

#elif defined(LandungsbrueckeGD32F303VGT6)
	#define LED_ON()            *HAL.IOs->pins->LED_STAT.resetBitRegister   = HAL.IOs->pins->LED_STAT.bitWeight
	#define LED_OFF()           *HAL.IOs->pins->LED_STAT.setBitRegister     = HAL.IOs->pins->LED_STAT.bitWeight
	#define LED_TOGGLE()        																			\
	{																										\
		if((uint32_t)RESET != (GPIO_OCTL(HAL.IOs->pins->LED_STAT.port)&(HAL.IOs->pins->LED_STAT.bitWeight)))\
			GPIO_BC(HAL.IOs->pins->LED_STAT.port) = HAL.IOs->pins->LED_STAT.bitWeight;						\
		else 																								\
			GPIO_BOP(HAL.IOs->pins->LED_STAT.port) = HAL.IOs->pins->LED_STAT.bitWeight;						\
	}
	

	#define LED_ERROR_ON()      *HAL.IOs->pins->LED_ERROR.resetBitRegister  = HAL.IOs->pins->LED_ERROR.bitWeight
	#define LED_ERROR_OFF()     *HAL.IOs->pins->LED_ERROR.setBitRegister    = HAL.IOs->pins->LED_ERROR.bitWeight
	#define LED_ERROR_TOGGLE()        																			\
	{																											\
		if((uint32_t)RESET != (GPIO_OCTL(HAL.IOs->pins->LED_ERROR.port)&(HAL.IOs->pins->LED_ERROR.bitWeight)))	\
			GPIO_BC(HAL.IOs->pins->LED_ERROR.port) = HAL.IOs->pins->LED_ERROR.bitWeight;							\
		else 																									\
			GPIO_BOP(HAL.IOs->pins->LED_ERROR.port) = HAL.IOs->pins->LED_ERROR.bitWeight;						\
	}
#endif

	#include "IOs.h"

	typedef struct
	{
		void (*on)(void);
		void (*off)(void);
		void (*toggle)(void);
	} LEDTypeDef;

	typedef struct
	{
		void (*init)(void);
		LEDTypeDef stat;
		LEDTypeDef error;
	} LEDsTypeDef;

	extern LEDsTypeDef LEDs;

#endif /* LEDS_H_ */
