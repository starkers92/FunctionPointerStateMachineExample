/*
 * state.h
 *
 *  Created on: 27/02/2019
 *      Author: chris
 */

#ifndef STATE_MACHINE_STATE_H_
#define STATE_MACHINE_STATE_H_

typedef enum {
	state_state1,
	state_state2,
	state_state3,
} e_state;

typedef enum {
	event_Any,
	event1,
	event2,
	event3,
	event_no_event,
} e_event;

#define EVENT_FIFO_LENGTH 32
typedef struct {
	e_event event_fifo[EVENT_FIFO_LENGTH];
	unsigned int newest_index;
	unsigned int oldest_index;
} eventStack_t;

typedef enum {
	eventStack_empty,
	eventStack_full,
	eventStack_ok,
} eventStackStatus_t;

typedef struct {
    e_state currState;
    eventStack_t event;
    /* Define any variables required here for state machine process */
} stateMachine_t;

void StateMachine_Init(stateMachine_t* s);
void StateMachine_Update(stateMachine_t *stateMachine);
void StateMachine_SetEvent(stateMachine_t* s, e_event event);
eventStackStatus_t StateMachine_WriteEvent(stateMachine_t* s, e_event event);
eventStackStatus_t StateMachine_ReadEvent(stateMachine_t* s, e_event* event);
void StateMachine_EmptyEventQueue(stateMachine_t* s);
#endif /* STATE_MACHINE_STATE_H_ */
