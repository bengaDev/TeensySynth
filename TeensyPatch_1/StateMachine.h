/*
 * StateMachine.h
 *
 *  Created on: 01 mag 2020
 *      Author: Benjamin
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "SequenceOsc.h"
#include "MidiController.h"

class StateMachine {
public:
	StateMachine(SequenceOsc *oscils, int numOscils);
	virtual ~StateMachine();

	void updateStateMachine();
	void updateStep(int stepNum, bool *isStepOn);

private:
	SequenceOsc **oscillator;
	int numOscils;
	MidiController AkaiMidi;

	enum {INIT_MODE, OSCIL_SELECTION_MODE, SEQUENCER_MODE, STEP_MODE}; // State Machine Mode
	int currentMode;
	int currentOscil;

	void processMidiCommand();
	void InitPanel();
	void InitProcessingComponents();
};

#endif /* STATEMACHINE_H_ */
