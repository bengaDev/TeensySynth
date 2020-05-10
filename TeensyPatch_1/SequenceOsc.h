/*
 * MidiNote.h
 *
 *  Created on: 13 apr 2020
 *      Author: Benjamin
 */

#ifndef SEQUENCEOSC_H_
#define SEQUENCEOSC_H_

#include <Audio.h>
#include "NoteFeatures.h"
#include "Sequence.h"

class SequenceOsc {
public:
	//MidiNote();
	SequenceOsc(float, float);
	virtual ~SequenceOsc();

	AudioSynthWaveform oscillator;
	AudioEffectEnvelope envelope;

	Sequence sequence;

	void UpdateState();
	void notePlay();

	// Oscillator Settings
	void setAmplitude(float);
	void setFrequency(float);
	void setAmpl_Freq(float, float);
	void setNote(Note_t selectedNote);
	int getWaveformType();
	void setWaveformType(int);

	// Envelope Settings
	void setDAHDSR(float, float, float, float, unsigned long, float, float);

private:
	bool isSustainEnd();
	void setDefaultDAHDSR();
	void setDefaultWaveformParams();

	unsigned long DEBUG_Time;

	AudioConnection patchCord1;

	// General purpose variables

	// Envelope Variables
	bool isSustainPhase;
	int delayMillis, attackMillis, holdMillis, decayMillis, releaseMillis;
	float sustainLevel;
	unsigned long sustainDuration, lastSustainMillis;

	float amplitude, frequency;
	int waveform;
};

#endif /* SEQUENCEOSC_H_ */
