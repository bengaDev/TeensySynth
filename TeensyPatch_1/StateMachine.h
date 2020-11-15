/*
 * StateMachine.h
 *
 *  Created on: 01 mag 2020
 *      Author: Benjamin
 */

#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "SequenceOsc.h"
#include "SequenceDrum.h"
#include "SequenceKick.h"
#include "MidiController.h"

class StateMachine {
public:
	StateMachine();
	virtual ~StateMachine();

	void updateStateMachine();
	void updateStepLed(int stepNum/*, bool *isStepOn*/); //TODO
	void manageStep(int stepNumber);

private:
	SequenceOsc **oscillator;
	SequenceDrum **drum;
	SequenceKick **kick;
	int numOscils, numDrums, numKicks;
	MidiController AkaiMidi;

	enum {INIT_MODE, OSCIL_SELECTION_MODE, DRUM_SELECTION_MODE, KICK_SELECTION_MODE, SEQUENCER_MODE, STEP_MODE}; // State Machine Mode
	typedef enum{	NULL_FADER_FUNCTION,
		MAIN_VOLUME,

		ENV_DELAY,	ENV_ATTACK,		ENV_HOLD,
		ENV_DECAY, 	ENV_SUSTAIN,	ENV_RELEASE,

		DRUM_FREQ, DRUM_LENGTH, DRUM_PITCH_MOD,

		KICK_FREQ, KICK_LENGTH, KICK_MOD_AMOUNT
	} FaderFunction_t;
	int currentMode, lastSelectionMode;
	int currentOscil, currentDrum, currentKick;

	int step;

	void processMidiCommand();
	void InitPanel();
	void refreshSequencer();
	void InitProcessingComponents();
	bool isClkTick();
	void manageTimeEnvelope(FaderFunction_t, byte);
	void manageDrumParams(FaderFunction_t, byte);
	void manageKickParams(FaderFunction_t, byte);
	int translateFaderIdxToFunction(FaderIdx_t);
};

#endif /* STATEMACHINE_H_ */
