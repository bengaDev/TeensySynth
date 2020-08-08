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
	StateMachine();
	virtual ~StateMachine();

	void updateStateMachine();
	void updateStepLed(int stepNum, bool *isStepOn);
	void manageStep(int stepNumber);

private:
	SequenceOsc **oscillator;
	int numOscils;
	MidiController AkaiMidi;

	enum {INIT_MODE, OSCIL_SELECTION_MODE, SEQUENCER_MODE, STEP_MODE}; // State Machine Mode
	int currentMode;
	int currentOscil;

	int step;

	void processMidiCommand();
	void InitPanel();
	void InitProcessingComponents();
	bool isClkTick();
	void manageTimeEnvelope(Fader_t, byte);
};

#endif /* STATEMACHINE_H_ */
