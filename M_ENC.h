#ifndef __M_ENC_H__
#define __M_ENC_H__


#include "main.h"
#include <stdbool.h>


//INFO & CONFIG
//////////////////////////////////////////////////////////////////////////
/*
 The library doesn't provide debounce for turns
 only for clicks.


 I_N_S_T_R_U_C_T_I_O_N

 MAIN.C
 -declare structures "menc_struct_type"
 -use 2 interrupts for every encoder (1 for S1, 1 for click)
 -if you want to use HOLD-TURN use different EXTI lines for click and turn
 	if (GPIO_Pin == GPIO_PIN_4) {
		MENC_ClickHandlerIRQ(&menc1);
	}
	if (GPIO_Pin == GPIO_PIN_2) {
		MENC_TurnHandlerIRQ(&menc1);
	}

	if (GPIO_Pin == GPIO_PIN_1) {
		MENC_ClickHandlerIRQ(&menc2);
	}
	if (GPIO_Pin == GPIO_PIN_0) {
		MENC_TurnHandlerIRQ(&menc2);
	}

 -fill up the structures in int(main)
 	menc1.s1_pin = GPIO_PIN_2;
	menc1.s2_pin = GPIO_PIN_3;
	menc1.key_pin = GPIO_PIN_4;
	menc1.s1_port = GPIOE;
	menc1.s2_port = GPIOE;
	menc1.key_port = GPIOE;
	menc1.key_exti_line = EXTI4_IRQn;
	#ifdef MENC_USE_TURN_DEB
	menc1.s1_exti_line = EXTI2_IRQn;
	#endif
 -use functions that way
  int main()

 	 while(1)
 	 	 MainHandler(&enc1);
 	 	 if (MENC_TurnRight(&enc1) {
 	 	 ....
 	 	 }
 	 	 if (MENC_TurnRight(&enc2) {
 	 	 ....
 	 	 }
 	 	 if (MENC_Click(&enc1) {
 	 	 ....
 	 	 }


M_ENC.H
 -set tick source (HAL_GetTick())
 -activate features
 -adjust timings
 -use IRQ functions at a  interrupt handler:
 */

//SETTINGS
//////////////////////////////////////////////////////////////////////////

//Tick source
////////////////////////////////////////////
#define MENC_TICK_SOURCE() HAL_GetTick()


//IRQ type
////////////////////////////////////////////
//#define MENC_USE_RISE_FALL_EDGE_IRQ_FOR_TURN
#define MENC_USE_RISE_EDGE_IRQ_FOR_TURN


//Additional features
////////////////////////////////////////////
//#define MENC_USE_HOLD_TURN //if you need turn while hold action
//#define MENC_USE_FAST_TURN //if you need fast turn action
#define MENC_USE_ANY_TURN //if you need turn at any side flag


//Debounce
////////////////////////////////////////////
//#define MENC_USE_TURN_DEB
#define MENC_USE_CLICK_DEB


//Timings
////////////////////////////////////////////
#define MENC_POLL_FREQ 10 ///polling frequency in main handler, ms
#define MENC_BUT_DEBOUNCE_INTERVAL 3 // x ms = MENC_POLL_FREQ * INTERVAL
#define MENC_TURN_DEB_INTERVAL 1 // x ms = MENC_POLL_FREQ * INTERVAL
#define MENC_BUT_HOLD_INTERVAL 40 // x ms = MENC_POLL_FREQ * INTERVAL
#define MENC_FAST_TURN_INTERVAL 2 // x ms = MENC_POLL_FREQ * INTERVAL (has to be more than 1)
//////////////////////////////////////////////////////////////////////////
//END OF CONFIG





//Status flags
//////////////////////////////////////////////////////////////////////////
//volatile uint16_t flags: 1 << flag code

//Action flags
#define MENC_AF_TURN_RIGHT 0
#define MENC_AF_TURN_LEFT 1
#define MENC_AF_HOLD_TURN_RIGHT 2
#define MENC_AF_HOLD_TURN_LEFT 3
#define MENC_AF_TURN_FAST_RIGHT 4
#define MENC_AF_TURN_FAST_LEFT 5
#define MENC_AF_CLICK 6
#define MENC_AF_HOLD 7
#define MENC_AF_ANY_TURN 8

//Service flags
#define MENC_SF_TURN_STOP 9
#define MENC_SF_CLICK_STOP 10
#define MENC_SF_CLICK 11
#define MENC_SF_TURN_FAST_RIGHT 12
#define MENC_SF_TURN_FAST_LEFT 13


//Read pins
//////////////////////////////////////////////////////////////////////////
#define MENC_S1_STATE(x,y) HAL_GPIO_ReadPin(x,y)
#define MENC_S2_STATE(x,y) HAL_GPIO_ReadPin(x,y)
#define MENC_KEY_STATE(x,y) HAL_GPIO_ReadPin(x,y)


//Click IRQ
////////////////////////////////////////////
#define ENABLE_CLICK_IRQ(x) NVIC_EnableIRQ(x)
#define DISABLE_CLICK_IRQ(x) NVIC_DisableIRQ(x)


//Turn IRQ
////////////////////////////////////////////
#define ENABLE_TURN_IRQ(x) NVIC_EnableIRQ(x)
#define DISABLE_TURN_IRQ(x) NVIC_DisableIRQ(x)


//Service defines//Variables
///////////////////////////////////////////////////////
//extern uint16_t ina_calib_val;//calibration value
///////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////
#define MENC_SET_FLAG(flags,shift) (flags)|=(1<<(shift))
#define MENC_CLEAR_FLAG(flags,shift) (flags)&=~(1<<(shift))
#define MENC_READ_FLAG(flags,shift) (((flags) >> (shift)) & 1)


//Init structure
//////////////////////////////////////////////////////////////////
typedef struct __menc_struct_type {
	uint16_t s1_pin;
	uint16_t s2_pin;
	uint16_t key_pin;
	GPIO_TypeDef *s1_port;
	GPIO_TypeDef *s2_port;
	GPIO_TypeDef *key_port;
	volatile uint16_t flags;
	IRQn_Type key_exti_line;
#ifdef MENC_USE_TURN_DEB
	IRQn_Type s1_exti_line;
#endif
	uint32_t enc_poll_tmr;
	uint16_t counter;
#ifdef MENC_USE_FAST_TURN
	uint16_t fast_turn_tick;
	uint16_t prev_turn;
	uint16_t curr_turn;
#endif
} menc_struct_type;
 //////////////////////////////////////////////////////////////////////////




//Functions
//////////////////////////////////////////////////////////////////////////
void MENC_MainHandler(menc_struct_type *m);

void MENC_TurnHandlerIRQ(menc_struct_type *m);
void MENC_ClickHandlerIRQ(menc_struct_type *m);

bool MENC_TurnLeft(menc_struct_type *m);
bool MENC_TurnRight(menc_struct_type *m);
bool MENC_AnyTurn(menc_struct_type *m);
bool MENC_Click(menc_struct_type *m);
bool MENC_Hold(menc_struct_type *m);

#ifdef MENC_USE_HOLD_TURN	
  bool MENC_HoldTurnLeft(menc_struct_type *m);
  bool MENC_HoldTurnRight(menc_struct_type *m);
#endif

#ifdef MENC_USE_FAST_TURN
  bool MENC_TurnFastLeft(menc_struct_type *m);
  bool MENC_TurnFastRight(menc_struct_type *m);
#endif
//////////////////////////////////////////////////////////////////////////
#endif
