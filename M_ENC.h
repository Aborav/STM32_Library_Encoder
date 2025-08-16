#ifndef __M_ENC_H__
#define __M_ENC_H__


/* 
The library doesn't provide debounce for turns
only for clicks.


I_N_S_T_R_U_C_T_I_O_N

	-use 2 interrupts (1 for S1, 1 for click)
	-set pins defines (S1,S2,KEY)
	-set tick source (HAL_GetTick())
	-activate features
	-adjust timings
	-use IRQ functions in interrupt handler
	
	void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){
	if(GPIO_Pin==ENCODER_KEY_Pin){
		MENC_ClickHandlerIRQ();
	}
	if(GPIO_Pin==ENCODER_S1_Pin){
		MENC_TurnHandlerIRQ();
	}
}

	int main()
		
	while(1)
		MainHandler();
		if (click)
		if (turn)
		...
*/

//Includes
//////////////////////////////////////////////////////////////////////////
#include "main.h"
#include <stdbool.h>
//////////////////////////////////////////////////////////////////////////


//SETTINGS
//////////////////////////////////////////////////////////////////////////

//Pins defines
//////////////////////////////////////////////////////////////////////////
//only S1 pin has to be connected to a interrupt poll source pin
//other pins can be connected to any MCU pins
#define MENC_S1_STATE() HAL_GPIO_ReadPin(ENCODER_S1_GPIO_Port,ENCODER_S1_Pin)
#define MENC_S2_STATE() HAL_GPIO_ReadPin(ENCODER_S2_GPIO_Port,ENCODER_S2_Pin)
#define MENC_KEY_STATE() HAL_GPIO_ReadPin(ENCODER_KEY_GPIO_Port,ENCODER_KEY_Pin)
//////////////////////////////////////////////////////////////////////////

//Tick source
////////////////////////////////////////////
#define MENC_TICK_SOURCE() HAL_GetTick()
///////////////////////////////////////////

//IRQ type
////////////////////////////////////////////
#define MENC_USE_RISE_FALL_EDGE_IRQ_FOR_TURN
//#define MENC_USE_FALL_EDGE_IRQ_FOR_TURN
////////////////////////////////////////////

//Click IRQ
////////////////////////////////////////////
#define ENABLE_CLICK_IRQ() NVIC_EnableIRQ(EXTI15_10_IRQn)
#define DISABLE_CLICK_IRQ() NVIC_DisableIRQ(EXTI15_10_IRQn)
////////////////////////////////////////////

//Turn IRQ
////////////////////////////////////////////
#define ENABLE_TURN_IRQ() NVIC_EnableIRQ(EXTI15_10_IRQn)
#define DISABLE_TURN_IRQ() NVIC_DisableIRQ(EXTI15_10_IRQn)
////////////////////////////////////////////

//Additional features
////////////////////////////////////////////
//#define MENC_USE_HOLD_TURN //if you need turn while hold action
//#define MENC_USE_FAST_TURN //if you need fast turn action
////////////////////////////////////////////

//Timings
////////////////////////////////////////////
#define MENC_POLL_FREQ 10 //polling frequency in main handler
#define MENC_BUT_DEBOUNCE_INTERVAL 3 // x 10 ms
#define MENC_BUT_HOLD_INTERVAL 40 // x 10 ms
#define MENC_FAST_TURN_INTERVAL 5 // x 10 ms
////////////////////////////////////////////




//Status flags
//////////////////////////////////////////////////////////////////////////
//Action flags
#define MENC_AF_TURN_RIGHT 0
#define MENC_AF_TURN_LEFT 1
#define MENC_AF_HOLD_TURN_RIGHT 2
#define MENC_AF_HOLD_TURN_LEFT 3
#define MENC_AF_TURN_FAST_RIGHT 4
#define MENC_AF_TURN_FAST_LEFT 5
#define MENC_AF_CLICK 6
#define MENC_AF_HOLD 7

//Service flags
#define MENC_SF_TURN_STOP 8
#define MENC_SF_CLICK_STOP 9
#define MENC_SF_CLICK 10
#define MENC_SF_TURN_FAST_RIGHT 11
#define MENC_SF_TURN_FAST_LEFT 12
//////////////////////////////////////////////////////////////////////////

//Variables
//////////////////////////////////////////////////////////////////////////
extern volatile uint16_t menc_flags;
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////
#define MENC_SET_FLAG(flag_ptr,shift) (flag_ptr)|=(1<<(shift))
#define MENC_CLEAR_FLAG(flag_ptr,shift) (flag_ptr)&=~(1<<(shift))
#define MENC_READ_FLAG(flag_ptr,shift) (((flag_ptr) >> (shift)) & 1)
//////////////////////////////////////////////////////////////////

//Functions
//////////////////////////////////////////////////////////////////////////  
  void MENC_MainHandler(void);
  
	void MENC_TurnHandlerIRQ(void);
  void MENC_ClickHandlerIRQ(void);
  
	bool MENC_TurnLeft(void);
  bool MENC_TurnRight(void);
  bool MENC_Click(void);
  bool MENC_Hold(void);
	
#ifdef MENC_USE_HOLD_TURN	
	bool MENC_HoldTurnLeft(void);
  bool MENC_HoldTurnRight(void);
#endif

#ifdef MENC_USE_FAST_TURN
  bool MENC_TurnFastLeft(void);
  bool MENC_TurnFastRight(void);
#endif
//////////////////////////////////////////////////////////////////////////
#endif
