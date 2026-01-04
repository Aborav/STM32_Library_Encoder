#include "M_ENC.h"

//Variables
//////////////////////////////////////////////////////////////////////////
//uint16_t menc_flags;
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

/**
 *@brief interrupt handler for a CLICK
 *@param encoder structure prototype pointer
 */
void MENC_ClickHandlerIRQ(struct menc_struct_type *m) {
	if (MENC_KEY_STATE(m->key_port, m->key_pin)) {
		MENC_SET_FLAG(m->flags, MENC_SF_CLICK);
#ifdef MENC_USE_CLICK_DEB
		DISABLE_CLICK_IRQ(m->key_exti_line); //for debounce
#endif

	}
}
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief interrupt handler for a TURN
 * @param encoder struct prototype pointer
 */
void MENC_TurnHandlerIRQ(struct menc_struct_type *m) {
#ifdef MENC_USE_TURN_DEB
	DISABLE_TURN_IRQ(m->s1_exti_line); //for debounce
#endif

#ifdef MENC_USE_RISE_FALL_EDGE_IRQ_FOR_TURN
		if (MENC_S1_STATE()) {
			if (!MENC_S2_STATE() && !MENC_READ_FLAG(m->flags,MENC_SF_TURN_STOP)) {
#endif

#ifdef MENC_USE_RISE_EDGE_IRQ_FOR_TURN
	if (!MENC_S2_STATE(m->s2_port, m->s2_pin)) {
#endif

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
#ifndef MENC_USE_HOLD_TURN
#ifndef MENC_USE_FAST_TURN
		MENC_SET_FLAG(m->flags, MENC_AF_TURN_LEFT); //main turn poll
#endif
#endif

#ifdef MENC_USE_HOLD_TURN
#ifndef MENC_USE_FAST_TURN
		if (MENC_READ_FLAG(m->flags,MENC_AF_HOLD)) MENC_SET_FLAG(m->flags,MENC_AF_HOLD_TURN_LEFT);
			else MENC_SET_FLAG(m->flags,MENC_AF_TURN_LEFT);
#endif
#endif

#ifndef MENC_USE_HOLD_TURN
#ifdef MENC_USE_FAST_TURN
		MENC_SET_FLAG(m->flags, MENC_SF_TURN_FAST_LEFT);
#endif
#endif

#ifdef MENC_USE_HOLD_TURN
#ifdef MENC_USE_FAST_TURN
		if (MENC_READ_FLAG(m->flags,MENC_AF_HOLD)) MENC_SET_FLAG(m->flags,MENC_AF_HOLD_TURN_LEFT);
			else MENC_SET_FLAG(m->flags,MENC_SF_TURN_FAST_LEFT);
#endif
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

#ifdef MENC_USE_RISE_FALL_EDGE_IRQ_FOR_TURN
				MENC_SET_FLAG(m->flags,MENC_SF_TURN_STOP);
			}
			if (MENC_S2_STATE()) {
#endif
//------------------------//
#ifdef MENC_USE_RISE_EDGE_IRQ_FOR_TURN
	} else {
#endif

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
#ifndef MENC_USE_HOLD_TURN
#ifndef MENC_USE_FAST_TURN
		MENC_SET_FLAG(m->flags, MENC_AF_TURN_RIGHT); //main turn poll
#endif
#endif

#ifdef MENC_USE_HOLD_TURN
#ifndef MENC_USE_FAST_TURN
		if (MENC_READ_FLAG(m->flags,MENC_AF_HOLD)) MENC_SET_FLAG(m->flags,MENC_AF_HOLD_TURN_RIGHT);
			else MENC_SET_FLAG(m->flags,MENC_AF_TURN_RIGHT);
#endif
#endif

#ifndef MENC_USE_HOLD_TURN
#ifdef MENC_USE_FAST_TURN
		MENC_SET_FLAG(m->flags, MENC_SF_TURN_FAST_RIGHT);
#endif
#endif

#ifdef MENC_USE_HOLD_TURN
#ifdef MENC_USE_FAST_TURN
		if (MENC_READ_FLAG(m->flags,MENC_AF_HOLD)) MENC_SET_FLAG(m->flags,MENC_AF_HOLD_TURN_RIGHT);
			else MENC_SET_FLAG(m->flags,MENC_SF_TURN_FAST_RIGHT);
#endif
#endif
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

#ifdef MENC_USE_RISE_FALL_EDGE_IRQ_FOR_TURN
				MENC_SET_FLAG(m->flags,MENC_SF_TURN_STOP);
			}
		}
		if (!MENC_S1_STATE(m->s1_port,m->s1_pin)) MENC_CLEAR_FLAG(m->flags,MENC_SF_TURN_STOP);

#endif

#ifdef MENC_USE_RISE_EDGE_IRQ_FOR_TURN
	}
#endif

}

///////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * @brief While(1) handler for encoder
 * @param encoder struct prototype pointer
 */
///////////////////////////////////////////////////////////////////////////////////////////////////
void MENC_MainHandler(struct menc_struct_type *m) {

//Polling
	if (MENC_TICK_SOURCE() - m->enc_poll_tmr >= MENC_POLL_FREQ) {
		m->enc_poll_tmr = MENC_TICK_SOURCE();

//TURN DEBOUNCE
#ifdef MENC_USE_DEB
/*main problem is in polling frequency
 I need to poll every 1 ms because timing between turns
 during fast rotation can be up to 4 ms
*/

//if (MENC_READ_FLAG(m->flags,MENC_SF_TURN_LEFT) || MENC_READ_FLAG(m->flags, MENC_SF_TURN_RIGHT)) can be change to menc_flags & 0b11 != 0
// If an interrupt has occurred, raise the flag of debounce
		if (MENC_READ_FLAG(m->flags,MENC_SF_TURN_LEFT) || MENC_READ_FLAG(m->flags, MENC_SF_TURN_RIGHT)) {
			MENC_SET_FLAG(m->flags, MENC_SF_TURN_DEB_START);
		}

//Check that a turn is over. If yes - increase counter, no - decrease
		if (MENC_READ_FLAG(m->flags, MENC_SF_TURN_DEB_START)) {
			if (!MENC_S1_STATE() && !MENC_S2_STATE()) {
				m->turn_deb_counter++;
			} else {
				MENC_CLEAR_FLAG(m->flags, MENC_SF_TURN_DEB_START);
				m->turn_deb_counter = 0;
			}
//Compare counter with defined value. If it's bigger -> set action flag, enable IRQ
			if (turn_deb_counter >= MENC_TURN_DEB_INTERVAL) {
				if (MENC_READ_FLAG(m->flags, MENC_SF_TURN_LEFT)) {
					MENC_SET_FLAG(m->flags, MENC_AF_TURN_LEFT);
					MENC_CLEAR_FLAG(m->flags, MENC_SF_TURN_LEFT);
				}
				if (MENC_READ_FLAG(m->flags, MENC_SF_TURN_RIGHT)) {
					MENC_SET_FLAG(m->flags, MENC_AF_TURN_RIGHT);
					MENC_CLEAR_FLAG(m->flags, MENC_SF_TURN_RIGHT);
				}
				m->turn_deb_counter = 0;
				MENC_CLEAR_FLAG(m->flags, MENC_SF_TURN_DEB_START);
				ENABLE_TURN_IRQ(m->s1_exti_line);
			}

		}
#endif

//CLICK DEBOUNCE
		if (!MENC_KEY_STATE(m->key_port, m->key_pin)) {
			m->counter = 0;
			ENABLE_CLICK_IRQ(m->key_exti_line);
			MENC_CLEAR_FLAG(m->flags, MENC_AF_HOLD);
			MENC_CLEAR_FLAG(m->flags, MENC_AF_CLICK);
		} else {
			m->counter++;
		}
		if (m->counter > MENC_BUT_DEBOUNCE_INTERVAL && MENC_READ_FLAG(m->flags, MENC_SF_CLICK)) {
			MENC_SET_FLAG(m->flags, MENC_AF_CLICK);
			MENC_CLEAR_FLAG(m->flags, MENC_SF_CLICK);
		}
		if (m->counter > MENC_BUT_HOLD_INTERVAL)
			MENC_SET_FLAG(m->flags, MENC_AF_HOLD);

#ifdef MENC_USE_FAST_TURN
		m->fast_turn_tick++; //to make long poll for FastTurn

		if (MENC_READ_FLAG(m->flags, MENC_SF_TURN_FAST_LEFT)) {
			MENC_CLEAR_FLAG(m->flags, MENC_SF_TURN_FAST_LEFT);
			m->curr_turn = m->fast_turn_tick;
			if ((m->curr_turn - m->prev_turn) < MENC_FAST_TURN_INTERVAL) { //if ticks between turns more than threshold
				MENC_SET_FLAG(m->flags, MENC_AF_TURN_FAST_LEFT);
			} else {
				MENC_SET_FLAG(m->flags, MENC_AF_TURN_LEFT);
			}
			m->prev_turn = m->curr_turn;
		}

		if (MENC_READ_FLAG(m->flags, MENC_SF_TURN_FAST_RIGHT)) {
			MENC_CLEAR_FLAG(m->flags, MENC_SF_TURN_FAST_RIGHT);
			m->curr_turn = m->fast_turn_tick;
			if ((m->curr_turn - m->prev_turn) < MENC_FAST_TURN_INTERVAL) { //if ticks between turns more than threshold
				MENC_SET_FLAG(m->flags, MENC_AF_TURN_FAST_RIGHT);
			} else {
				MENC_SET_FLAG(m->flags, MENC_AF_TURN_RIGHT);
			}
			m->prev_turn = m->curr_turn;
		}
#endif
	}

}

////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
bool MENC_TurnRight(struct menc_struct_type *m) {
#ifdef MENC_USE_RISE_EDGE_IRQ_FOR_TURN
	if (MENC_READ_FLAG(m->flags, MENC_AF_TURN_RIGHT)) {
		MENC_CLEAR_FLAG(m->flags, MENC_AF_TURN_RIGHT);
		return 1;
	} else
		return 0;
#endif
#ifndef MENC_USE_RISE_EDGE_IRQ_FOR_TURN
	if (MENC_READ_FLAG(m->flags,MENC_AF_TURN_RIGHT) && !MENC_READ_FLAG(m->flags,MENC_SF_TURN_STOP)) {
		MENC_CLEAR_FLAG(m->flags,MENC_AF_TURN_RIGHT);
		return 1;
	} else return 0;	
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
bool MENC_TurnLeft(struct menc_struct_type *m) {
#ifdef MENC_USE_RISE_EDGE_IRQ_FOR_TURN
	if (MENC_READ_FLAG(m->flags, MENC_AF_TURN_LEFT)) {
		MENC_CLEAR_FLAG(m->flags, MENC_AF_TURN_LEFT);
		return 1;
	} else
		return 0;
#endif
#ifndef MENC_USE_RISE_EDGE_IRQ_FOR_TURN
	if (MENC_READ_FLAG(m->flags,MENC_AF_TURN_LEFT) && !MENC_READ_FLAG(m->flags,MENC_SF_TURN_STOP)) {
		MENC_CLEAR_FLAG(m->flags,MENC_AF_TURN_LEFT);
		return 1;
	} else return 0;	
#endif
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
bool MENC_Click(struct menc_struct_type *m) {
	if (MENC_READ_FLAG(m->flags, MENC_AF_CLICK)) {
		MENC_CLEAR_FLAG(m->flags, MENC_AF_CLICK);
#ifdef MENC_USE_FALL_RISE_EDGE_IRQ_FOR_TURN
		ENAB_PCINT_CLICK();
#endif
		return 1;
	} else
		return 0;
}

bool MENC_Hold(struct menc_struct_type *m) {
	return MENC_READ_FLAG(m->flags, MENC_AF_HOLD);
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MENC_USE_HOLD_TURN
bool MENC_HoldTurnLeft(struct menc_struct_type *m) {
	if (MENC_READ_FLAG(m->flags,MENC_AF_HOLD_TURN_LEFT)) {
		MENC_CLEAR_FLAG(m->flags,MENC_AF_HOLD_TURN_LEFT);
		return 1;
	} else return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
bool MENC_HoldTurnRight(struct menc_struct_type *m) {
	if (MENC_READ_FLAG(m->flags,MENC_AF_HOLD_TURN_RIGHT)) {
		MENC_CLEAR_FLAG(m->flags,MENC_AF_HOLD_TURN_RIGHT);
		return 1;
	} else return 0;
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
#ifdef MENC_USE_FAST_TURN
bool MENC_TurnFastLeft(struct menc_struct_type *m) {
	if (MENC_READ_FLAG(m->flags, MENC_AF_TURN_FAST_LEFT)) {
		MENC_CLEAR_FLAG(m->flags, MENC_AF_TURN_FAST_LEFT);
		return 1;
	} else
		return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
bool MENC_TurnFastRight(struct menc_struct_type *m) {
	if (MENC_READ_FLAG(m->flags, MENC_AF_TURN_FAST_RIGHT)) {
		MENC_CLEAR_FLAG(m->flags, MENC_AF_TURN_FAST_RIGHT);
		return 1;
	} else
		return 0;
}
#endif
////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
