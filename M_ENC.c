#include "M_Encoder\M_ENC.h"


//Variables
//////////////////////////////////////////////////////////////////////////
volatile uint16_t menc_flags;
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
///@brief falling edge interrupt handler for CLICK
void MENC_ClickHandlerIRQ(void) {
	if (!MENC_KEY_STATE()) {
	MENC_SET_FLAG(menc_flags,MENC_SF_CLICK);
	DISABLE_CLICK_IRQ(); //for debounce
	}
}
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
///@brief rising and falling edge interrupt handler for TURN
void MENC_TurnHandlerIRQ(void) {
	
	
#ifdef MENC_USE_RISE_FALL_EDGE_IRQ_FOR_TURN
		if (!MENC_S1_STATE()) {
			if (MENC_S2_STATE() && !MENC_READ_FLAG(menc_flags,MENC_SF_TURN_STOP)) {
#endif

#ifdef MENC_USE_FALL_EDGE_IRQ_FOR_TURN
		if (MENC_S2_STATE()) {
#endif


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
#ifndef MENC_USE_HOLD_TURN
#ifndef MENC_USE_FAST_TURN
		MENC_SET_FLAG(menc_flags,MENC_AF_TURN_LEFT); //main turn poll
#endif
#endif

#ifdef MENC_USE_HOLD_TURN
#ifndef MENC_USE_FAST_TURN
		if (MENC_READ_FLAG(menc_flags,MENC_AF_HOLD)) MENC_SET_FLAG(menc_flags,MENC_AF_HOLD_TURN_LEFT);
			else MENC_SET_FLAG(menc_flags,MENC_AF_TURN_LEFT);
#endif
#endif

#ifndef MENC_USE_HOLD_TURN
#ifdef MENC_USE_FAST_TURN
		MENC_SET_FLAG(menc_flags,MENC_SF_TURN_FAST_LEFT);
#endif
#endif

#ifdef MENC_USE_HOLD_TURN
#ifdef MENC_USE_FAST_TURN
		if (MENC_READ_FLAG(menc_flags,MENC_AF_HOLD)) MENC_SET_FLAG(menc_flags,MENC_AF_HOLD_TURN_LEFT);
			else MENC_SET_FLAG(menc_flags,MENC_SF_TURN_FAST_LEFT);
#endif
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

#ifdef MENC_USE_RISE_FALL_EDGE_IRQ_FOR_TURN
				MENC_SET_FLAG(menc_flags,MENC_SF_TURN_STOP);
			}
			if (!MENC_S2_STATE()) {
#endif
//------------------------//
#ifdef MENC_USE_FALL_EDGE_IRQ_FOR_TURN
			} else {
#endif

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
#ifndef MENC_USE_HOLD_TURN
#ifndef MENC_USE_FAST_TURN
		MENC_SET_FLAG(menc_flags,MENC_AF_TURN_RIGHT); //main turn poll
#endif
#endif

#ifdef MENC_USE_HOLD_TURN
#ifndef MENC_USE_FAST_TURN
		if (MENC_READ_FLAG(menc_flags,MENC_AF_HOLD)) MENC_SET_FLAG(menc_flags,MENC_AF_HOLD_TURN_RIGHT);
			else MENC_SET_FLAG(menc_flags,MENC_AF_TURN_RIGHT);
#endif
#endif

#ifndef MENC_USE_HOLD_TURN
#ifdef MENC_USE_FAST_TURN
		MENC_SET_FLAG(menc_flags,MENC_SF_TURN_FAST_RIGHT);
#endif
#endif

#ifdef MENC_USE_HOLD_TURN
#ifdef MENC_USE_FAST_TURN
		if (MENC_READ_FLAG(menc_flags,MENC_AF_HOLD)) MENC_SET_FLAG(menc_flags,MENC_AF_HOLD_TURN_RIGHT);
			else MENC_SET_FLAG(menc_flags,MENC_SF_TURN_FAST_RIGHT);
#endif
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

#ifdef MENC_USE_RISE_FALL_EDGE_IRQ_FOR_TURN
				MENC_SET_FLAG(menc_flags,MENC_SF_TURN_STOP);
			}
		}
		if (MENC_S1_STATE()) MENC_CLEAR_FLAG(menc_flags,MENC_SF_TURN_STOP);

#endif

#ifdef MENC_USE_FALL_EDGE_IRQ_FOR_TURN
}
#endif
}
///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
void MENC_MainHandler(void) {
	static uint32_t enc_poll_tmr;
	static uint16_t counter;
#ifdef MENC_USE_FAST_TURN
	static uint8_t poll_counter;
	static uint8_t turns_counter;
#endif

//Polling
	if (MENC_TICK_SOURCE()-enc_poll_tmr>=MENC_POLL_FREQ) {
		enc_poll_tmr=MENC_TICK_SOURCE();


		if (MENC_KEY_STATE()) {
			counter=0;
			ENABLE_CLICK_IRQ();
			MENC_CLEAR_FLAG(menc_flags,MENC_AF_HOLD);
			MENC_CLEAR_FLAG(menc_flags,MENC_AF_CLICK);
		} else counter++;
		if (counter>MENC_BUT_DEBOUNCE_INTERVAL && MENC_READ_FLAG(menc_flags,MENC_SF_CLICK)) {
			MENC_SET_FLAG(menc_flags,MENC_AF_CLICK);
			MENC_CLEAR_FLAG(menc_flags,MENC_SF_CLICK);
		}
		if (counter>MENC_BUT_HOLD_INTERVAL) MENC_SET_FLAG(menc_flags,MENC_AF_HOLD);

	

#ifdef MENC_USE_FAST_TURN
		poll_counter++; //to make long poll for FastTurn
		if (poll_counter>5) { // MENC_poll_freq * poll_counter ( 20 ms * 5 = 100 ms)
			poll_counter=0;
			turns_counter=0;
		}
		if (MENC_READ_FLAG(menc_flags,MENC_SF_TURN_FAST_LEFT)) {
			turns_counter++;
			MENC_CLEAR_FLAG(menc_flags,MENC_SF_TURN_FAST_LEFT);
			if (turns_counter>1) MENC_SET_FLAG(menc_flags,MENC_AF_TURN_FAST_LEFT);
			if (turns_counter==1) MENC_SET_FLAG(menc_flags,MENC_AF_TURN_LEFT);
		}
		if (MENC_READ_FLAG(menc_flags,MENC_SF_TURN_FAST_RIGHT)) {
			turns_counter++;
			MENC_CLEAR_FLAG(menc_flags,MENC_SF_TURN_FAST_RIGHT);
			if (turns_counter>1) MENC_SET_FLAG(menc_flags,MENC_AF_TURN_FAST_RIGHT);
			if (turns_counter==1) MENC_SET_FLAG(menc_flags,MENC_AF_TURN_RIGHT);
		}
	
	
#endif
	}

}

////////////////////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////////////////////
bool MENC_TurnRight(void) {
#ifdef MENC_USE_FALL_EDGE_IRQ_FOR_TURN
	if (MENC_READ_FLAG(menc_flags,MENC_AF_TURN_RIGHT)) {
		MENC_CLEAR_FLAG(menc_flags,MENC_AF_TURN_RIGHT);
	return 1;
  } else return 0;	
#endif
#ifndef MENC_USE_FALL_EDGE_IRQ_FOR_TURN
	if (MENC_READ_FLAG(menc_flags,MENC_AF_TURN_RIGHT) && !MENC_READ_FLAG(menc_flags,MENC_SF_TURN_STOP)) {
		MENC_CLEAR_FLAG(menc_flags,MENC_AF_TURN_RIGHT);
		return 1;
	} else return 0;	
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
bool MENC_TurnLeft(void) {
#ifdef MENC_USE_FALL_EDGE_IRQ_FOR_TURN
	if (MENC_READ_FLAG(menc_flags,MENC_AF_TURN_LEFT)) {
		MENC_CLEAR_FLAG(menc_flags,MENC_AF_TURN_LEFT);
	return 1;
  } else return 0;	
#endif
#ifndef MENC_USE_FALL_EDGE_IRQ_FOR_TURN
	if (MENC_READ_FLAG(menc_flags,MENC_AF_TURN_LEFT) && !MENC_READ_FLAG(menc_flags,MENC_SF_TURN_STOP)) {
		MENC_CLEAR_FLAG(menc_flags,MENC_AF_TURN_LEFT);
		return 1;
	} else return 0;	
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
bool MENC_Click(void) {
	if (MENC_READ_FLAG(menc_flags,MENC_AF_CLICK)) {
		MENC_CLEAR_FLAG(menc_flags,MENC_AF_CLICK);
#ifdef MENC_USE_FALL_RISE_EDGE_IRQ_FOR_TURN
		ENAB_PCINT_CLICK();
#endif
	return 1;
  } else return 0;
}

bool MENC_Hold(void) {
	return MENC_READ_FLAG(menc_flags,MENC_AF_HOLD);
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MENC_USE_HOLD_TURN
bool MENC_HoldTurnLeft(void) {
	if (MENC_READ_FLAG(menc_flags,MENC_AF_HOLD_TURN_LEFT)) {
		MENC_CLEAR_FLAG(menc_flags,MENC_AF_HOLD_TURN_LEFT);
	return 1;
  } else return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
bool MENC_HoldTurnRight(void) {
	if (MENC_READ_FLAG(menc_flags,MENC_AF_HOLD_TURN_RIGHT)) {
		MENC_CLEAR_FLAG(menc_flags,MENC_AF_HOLD_TURN_RIGHT);
	return 1;
  } else return 0;
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MENC_USE_FAST_TURN
bool MENC_TurnFastLeft(void) {
	if (MENC_READ_FLAG(menc_flags,MENC_AF_TURN_FAST_LEFT)) {
		MENC_CLEAR_FLAG(menc_flags,MENC_AF_TURN_FAST_LEFT);
	return 1;
  } else return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
bool MENC_TurnFastRight(void) {
	if (MENC_READ_FLAG(menc_flags,MENC_AF_TURN_FAST_RIGHT)) {
		MENC_CLEAR_FLAG(menc_flags,MENC_AF_TURN_FAST_RIGHT);
	return 1;
  } else return 0;
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////