/*
 * state.c
 *
 *  Created on: 27/02/2019
 *      Author: chris
 */

#include "StateMachine.h"

/* State functions */
void Led_Idle(stateMachine_t* s) {

}

void Led_On(stateMachine_t* s) {

}

void Led_Off(stateMachine_t* s) {

}

/* Global definitions for state machine */

typedef struct {
	e_state state;
	void (*func)(stateMachine_t*);
} stateFunctionRow_t;

static stateFunctionRow_t stateFunctionA[] = {
		// NAME         	// FUNC
		{ state_state1,   &Led_Idle},      // ST_IDLE
		{ state_state2,   &Led_On},        // ST_LED_ON
		{ state_state3,   &Led_Off},       // ST_LED_OFF
};

typedef struct {
	e_state currState;
	e_event event;
	e_state nextState;
} stateTransMatrixRow_t;

static stateTransMatrixRow_t stateTransMatrix[] = {
		// CURR STATE  		// EVENT        // NEXT STATE
		{ state_state1,     event1,     	state_state1},
		{ state_state1,   	event1,         state_state2},
		{ state_state2,   	event2,     	state_state3},
		{ state_state2,  	event3,         state_state1},
		{ state_state3,  	event3,     	state_state2},
};


/* State machine servicing functions */
void StateMachine_Init(stateMachine_t * stateMachine) {
	/* Do code here */

	/* Update initial state */
	stateMachine->currState = state_state1;
	//stateMachine->eventPending = event_no_event;
}

void StateMachine_RunIterationSkipEventQueue(stateMachine_t *stateMachine, e_event event) {
	// Iterate through the state transition matrix, checking if there is both a match with the current state
	// and the event
	for(int i = 0; i < sizeof(stateTransMatrix)/sizeof(stateTransMatrix[0]); i++) {
		if(stateTransMatrix[i].currState == stateMachine->currState) {
			if((stateTransMatrix[i].event == event) || (stateTransMatrix[i].event == event_Any)) {

				/* Transition to the next state */
				stateMachine->currState =  stateTransMatrix[i].nextState;

				/* Call the function associated with transition */
				(stateFunctionA[stateMachine->currState].func)(stateMachine);
				break;
			}
		}
	}
}

void StateMachine_RunIteration(stateMachine_t *stateMachine) {
	e_event eventFromFifo;
	while(StateMachine_ReadEvent(stateMachine, &eventFromFifo)==eventStack_ok) {
		// Iterate through the state transition matrix, checking if there is both a match with the current state
		// and the event
		for(int i = 0; i < sizeof(stateTransMatrix)/sizeof(stateTransMatrix[0]); i++) {
			if(stateTransMatrix[i].currState == stateMachine->currState) {
				if((stateTransMatrix[i].event == eventFromFifo) || (stateTransMatrix[i].event == event_Any)) {

					/* Transition to the next state */
					stateMachine->currState =  stateTransMatrix[i].nextState;

					/* Call the function associated with transition */
					(stateFunctionA[stateMachine->currState].func)(stateMachine);
					break;
				}
			}
		}
	}
}

eventStackStatus_t StateMachine_WriteEvent(stateMachine_t* s, e_event event) {
	unsigned int next_index = (s->event.newest_index+1) % EVENT_FIFO_LENGTH;
	if (next_index== s->event.oldest_index) {
		return eventStack_full;
	}
	s->event.event_fifo[s->event.newest_index] = event;
	s->event.newest_index = next_index;
	return eventStack_ok;
}

eventStackStatus_t StateMachine_ReadEvent(stateMachine_t* s, e_event* event) {
	if(s->event.newest_index == s->event.oldest_index) {
		return eventStack_empty;
	}
	*event = s->event.event_fifo[s->event.oldest_index];
	s->event.oldest_index = (s->event.oldest_index + 1) % EVENT_FIFO_LENGTH;
	return eventStack_ok;
}

void StateMachine_EmptyEventQueue(stateMachine_t* s) {
	s->event.newest_index=0;
	s->event.oldest_index=0;
}
