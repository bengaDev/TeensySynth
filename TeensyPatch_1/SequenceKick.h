/*
 * SequenceKick.h
 *
 *  Created on: 22 set 2020
 *      Author: Benjamin
 */

#ifndef SEQUENCEKICK_H_
#define SEQUENCEKICK_H_

#include <Audio.h>
#include "Sequence.h"
#include "SequenceOsc.h"
#include "ShiftingSine.h"

class SequenceKick {
public:
	SequenceKick();
	virtual ~SequenceKick();
	AudioStream* getKickAudioOut();
	void notePlay();
	void setFreq(float freq);
	void setLength(float millis);
	void setMod(float modAmount);

	//SequenceOsc oscillatorTone;
	ShiftingSine shiftingSine;
	//SequenceOsc oscillatorClick;

	//AudioSynthWaveform oscillatorTone;
	//AudioSynthWaveform oscillatorClick;
	//AudioEffectEnvelope envelopeTone;

	Sequence sequence;

private:
	//AudioConnection patchCord1;

};

#endif /* SEQUENCEKICK_H_ */
