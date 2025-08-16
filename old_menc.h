#include "M_Encoder/M_ENC.h"






//////////////////////////////////////////////////////////////////////////////////////////
//----------------------------//
#ifdef MENC_USE_POLL_FOR_TURN
void MENC_MainTick(void) 
{
		if (!(MENC_S1_STATE & MENC_S2_STATE)) {
			if (MENC_S2_STATE && !MENC_READ_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP)) {
#endif
//----------------------------//
#ifdef MENC_USE_FALL_RISE_EDGE_ISR_FOR_TURN
void MENC_TurnFlagISR(void) {
		if (!MENC_S1_STATE) {
			if (MENC_S2_STATE && !MENC_READ_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP)) {
#endif
//----------------------------//
#ifdef MENC_USE_FALL_EDGE_ISR_FOR_TURN
void MENC_TurnFlagISR(void) {
		if (MENC_S2_STATE) {
#endif
////////////////////////////////		
			
			
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
#ifndef MENC_USE_HOLD_TURN
#ifndef MENC_USE_FAST_TURN
		MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_TURN_LEFT); //main turn poll
#endif
#endif

#ifdef MENC_USE_HOLD_TURN
#ifndef MENC_USE_FAST_TURN
		if (MENC_READ_FLAG(*menc_flags_ptr,MENC_AF_HOLD)) MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_HOLD_TURN_LEFT);
			else MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_TURN_LEFT);
#endif
#endif

#ifndef MENC_USE_HOLD_TURN
#ifdef MENC_USE_FAST_TURN
		MENC_SET_FLAG(*menc_flags_ptr,MENC_SF_TURN_FAST_LEFT);
#endif
#endif

#ifdef MENC_USE_HOLD_TURN
#ifdef MENC_USE_FAST_TURN
		if (MENC_READ_FLAG(*menc_flags_ptr,MENC_AF_HOLD)) MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_HOLD_TURN_LEFT);
			else MENC_SET_FLAG(*menc_flags_ptr,MENC_SF_TURN_FAST_LEFT);
#endif
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//



////////////////////////////
#ifdef MENC_USE_POLL_FOR_TURN
				MENC_SET_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP);
			}
			if (MENC_S1_STATE && !MENC_READ_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP)) {
#endif
//------------------------//
#ifdef MENC_USE_FALL_RISE_EDGE_ISR_FOR_TURN
				MENC_SET_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP);
			}
			if (!MENC_S2_STATE) {
#endif
//------------------------//
#ifdef MENC_USE_FALL_EDGE_ISR_FOR_TURN
			} else {
#endif
////////////////////////////


//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
#ifndef MENC_USE_HOLD_TURN
#ifndef MENC_USE_FAST_TURN
		MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_TURN_RIGHT); //main turn poll
#endif
#endif

#ifdef MENC_USE_HOLD_TURN
#ifndef MENC_USE_FAST_TURN
		if (MENC_READ_FLAG(*menc_flags_ptr,MENC_AF_HOLD)) MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_HOLD_TURN_RIGHT);
			else MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_TURN_RIGHT);
#endif
#endif

#ifndef MENC_USE_HOLD_TURN
#ifdef MENC_USE_FAST_TURN
		MENC_SET_FLAG(*menc_flags_ptr,MENC_SF_TURN_FAST_RIGHT);
#endif
#endif

#ifdef MENC_USE_HOLD_TURN
#ifdef MENC_USE_FAST_TURN
		if (MENC_READ_FLAG(*menc_flags_ptr,MENC_AF_HOLD)) MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_HOLD_TURN_RIGHT);
			else MENC_SET_FLAG(*menc_flags_ptr,MENC_SF_TURN_FAST_RIGHT);
#endif
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//


////////////////////////////
#ifdef MENC_USE_POLL_FOR_TURN
				MENC_SET_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP);
			}
		}
		if (MENC_S1_STATE && MENC_S2_STATE) MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP);
#endif
//------------------------//
#ifdef MENC_USE_FALL_RISE_EDGE_ISR_FOR_TURN
				MENC_SET_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP);
			}
		}
		if (MENC_S1_STATE) MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP);
}
void MENC_MainTick(void) 
{
#endif
//------------------------//
#ifdef MENC_USE_FALL_EDGE_ISR_FOR_TURN
		}
}
void MENC_MainTick(void) 
{
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////


	static uint32_t enc_poll_tmr;
	static uint16_t counter;
#ifdef MENC_USE_FAST_TURN
	static uint8_t poll_counter;
	static uint8_t turns_counter;
#endif

//Polling
	if (MENC_TICK_SOURCE()-enc_poll_tmr>=MENC_POLL_FREQ) {
		enc_poll_tmr=MENC_TICK_SOURCE();

		
#ifdef MENC_USE_POLL_FOR_CLICK
	//interval poll way
		if (!MENC_KEY_STATE) counter++;
			else {
				counter=0;
				MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_SF_CLICK_STOP);
				MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_CLICK);
				MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_HOLD);
			}
		if (counter>MENC_BUT_DEBOUNCE_INTERVAL && !MENC_READ_FLAG(*menc_flags_ptr,MENC_SF_CLICK_STOP)) {
			MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_CLICK);
			MENC_SET_FLAG(*menc_flags_ptr,MENC_SF_CLICK_STOP);
		}
		if (counter>MENC_BUT_HOLD_INTERVAL) MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_HOLD);
#endif

#ifdef MENC_USE_ISR_FOR_CLICK
	//PCINT + interval poll

		if (MENC_KEY_STATE) {
			counter=0;
			ENAB_PCINT_CLICK();
			MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_HOLD);
			MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_CLICK);
		} else counter++;
		if (counter>MENC_BUT_DEBOUNCE_INTERVAL && MENC_READ_FLAG(*menc_flags_ptr,MENC_SF_CLICK)) {
			MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_CLICK);
			MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_SF_CLICK);
		}
		if (counter>MENC_BUT_HOLD_INTERVAL) MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_HOLD);
	
	
#endif
	

#ifdef MENC_USE_FAST_TURN
		poll_counter++; //to make long poll for FastTurn
		if (poll_counter>5) { // MENC_poll_freq * poll_counter ( 20 ms * 5 = 100 ms)
			poll_counter=0;
			turns_counter=0;
		}
		if (MENC_READ_FLAG(*menc_flags_ptr,MENC_SF_TURN_FAST_LEFT)) {
			turns_counter++;
			MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_SF_TURN_FAST_LEFT);
			if (turns_counter>1) MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_TURN_FAST_LEFT);
			if (turns_counter==1) MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_TURN_LEFT);
		}
		if (MENC_READ_FLAG(*menc_flags_ptr,MENC_SF_TURN_FAST_RIGHT)) {
			turns_counter++;
			MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_SF_TURN_FAST_RIGHT);
			if (turns_counter>1) MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_TURN_FAST_RIGHT);
			if (turns_counter==1) MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_TURN_RIGHT);
		}
	}
	
#endif
	
	
}

////////////////////////////////////////////////////////////////////////////////////////////




#ifdef MENC_USE_FALL_RISE_EDGE_ISR_FOR_TURN
void MENC_ClickFlagISR(void) {
	if (!MENC_KEY_STATE) {
	MENC_SET_FLAG(*menc_flags_ptr,MENC_SF_CLICK);
	DISAB_PCINT_CLICK(); //disable PCINT7 interrupt for debounce
	}
}
#endif

bool MENC_TurnRight(void) {
#ifdef MENC_USE_FALL_EDGE_ISR_FOR_TURN
	if (MENC_READ_FLAG(*menc_flags_ptr,MENC_AF_TURN_RIGHT)) {
		MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_TURN_RIGHT);
	return 1;
  } else return 0;	
#endif
#ifndef MENC_USE_FALL_EDGE_ISR_FOR_TURN
	if (MENC_READ_FLAG(*menc_flags_ptr,MENC_AF_TURN_RIGHT) && !MENC_READ_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP)) {
		MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_TURN_RIGHT);
		return 1;
	} else return 0;	
#endif
}

bool MENC_TurnLeft(void) {
#ifdef MENC_USE_FALL_EDGE_ISR_FOR_TURN
	if (MENC_READ_FLAG(*menc_flags_ptr,MENC_AF_TURN_LEFT)) {
		MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_TURN_LEFT);
	return 1;
  } else return 0;	
#endif
#ifndef MENC_USE_FALL_EDGE_ISR_FOR_TURN
	if (MENC_READ_FLAG(*menc_flags_ptr,MENC_AF_TURN_LEFT) && !MENC_READ_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP)) {
		MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_TURN_LEFT);
		return 1;
	} else return 0;	
#endif
}

bool MENC_Click(void) {
	if (MENC_READ_FLAG(*menc_flags_ptr,MENC_AF_CLICK)) {
		MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_CLICK);
#ifdef MENC_USE_FALL_RISE_EDGE_ISR_FOR_TURN
		ENAB_PCINT_CLICK();
#endif
	return 1;
  } else return 0;
}

bool MENC_Hold(void) {
	return MENC_READ_FLAG(*menc_flags_ptr,MENC_AF_HOLD);
}

#ifdef MENC_USE_HOLD_TURN
bool MENC_HoldTurnLeft(void) {
	if (MENC_READ_FLAG(*menc_flags_ptr,MENC_AF_HOLD_TURN_LEFT)) {
		MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_HOLD_TURN_LEFT);
	return 1;
  } else return 0;
}

bool MENC_HoldTurnRight(void) {
	if (MENC_READ_FLAG(*menc_flags_ptr,MENC_AF_HOLD_TURN_RIGHT)) {
		MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_HOLD_TURN_RIGHT);
	return 1;
  } else return 0;
}
#endif

#ifdef MENC_USE_FAST_TURN
bool MENC_TurnFastLeft(void) {
	if (MENC_READ_FLAG(*menc_flags_ptr,MENC_AF_TURN_FAST_LEFT)) {
		MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_TURN_FAST_LEFT);
	return 1;
  } else return 0;
}

bool MENC_TurnFastRight(void) {
	if (MENC_READ_FLAG(*menc_flags_ptr,MENC_AF_TURN_FAST_RIGHT)) {
		MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_TURN_FAST_RIGHT);
	return 1;
  } else return 0;
}
#endif








	/*
#ifdef MENC_USE_POLL_FOR_TURN
    if (!MENC_S1_STATE || !MENC_S2_STATE) {
      if (MENC_S1_STATE && !MENC_READ_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP)) {
#ifndef MENC_USE_FAST_TURN
		MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_TURN_RIGHT); //main poll
#endif
#ifdef MENC_USE_HOLD_TURN
		MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_HOLD_TURN_RIGHT);
#endif
#ifdef MENC_USE_FAST_TURN
		MENC_SET_FLAG(*menc_flags_ptr,MENC_SF_TURN_FAST_RIGHT);
#endif
		MENC_SET_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP);
      } else if (MENC_S2_STATE && !MENC_READ_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP)) {
#ifndef MENC_USE_FAST_TURN
		MENC_SET_FLAG(*menc_flags_ptr,MENC_ACTION_TURN_LEFT); //main poll
#endif
#ifdef MENC_USE_HOLD_TURN
		MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_HOLD_TURN_LEFT);
#endif
#ifdef MENC_USE_FAST_TURN
		MENC_SET_FLAG(*menc_flags_ptr,MENC_SF_TURN_FAST_LEFT);
#endif
        MENC_SET_FLAG(*menc_flags_ptr,MENC_SF_TURN_STOP);
      }
    }
    if (MENC_S1_STATE && MENC_S2_STATE) MENC_CLEAR_FLAG(MENC_SF_TURN_STOP);
#endif
*/

/*
	//Usual click, only debounce
	//Works fine, but it sets 1 every debounce time
	//Need to cleen flag at Click function
	if (!MENC_KEY_STATE && (Millis()-debounce_tmr)>=200) {
		debounce_tmr=Millis();
		MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_CLICK);
	}
*/
/*
	// debounce timer as hold timer + counter of funct enterance
	//works bad : 
	//-couple clicks before hold
	//-bad decounce elimination
	if ((!MENC_KEY_STATE) && ((Millis()-hold_tmr)>=300)) {
		
		counter++;
		if (counter==1) MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_CLICK);
		if (counter>1) MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_HOLD);
		hold_tmr=Millis();
	}
	if (MENC_KEY_STATE && (Millis()-hold_tmr>=300)) counter=0;
*/

/*
	//Debounce + time count after click
	//works bad:
	//-count at start of program
	//-button click can be from 2 to 104 ms, 
	if (!MENC_KEY_STATE && (Millis()-debounce_tmr)>=200) {
		counter=Millis()-count_tmr;
		debounce_tmr=Millis();
	}
	if (MENC_KEY_STATE) {
		count_tmr=Millis();
		counter=0;
	}
	if (counter<130 && counter>1) MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_CLICK);
	if (counter>400) MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_HOLD);
	
	if (counter==0) {
		MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_CLICK);
		MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_HOLD);
	}
	if (!counter==0) return counter;
*/
/*
	//stop flag + debounce for push + debounce for release
	//works as usual millis debounce becouse stop flag changes during release
	if (!MENC_KEY_STATE && !stop_flag && (Millis()-push_deb_tmr)>=200) {
		push_deb_tmr=Millis();
		stop_flag=1;
		MENC_SET_FLAG(*menc_flags_ptr,MENC_AF_CLICK);
	}
	if (MENC_KEY_STATE && stop_flag && (Millis()-release_deb_tmr)>=200) {
		release_deb_tmr=Millis();
		stop_flag=0;
		MENC_CLEAR_FLAG(*menc_flags_ptr,MENC_AF_CLICK);
	}
*/