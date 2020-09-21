/*
 * SequenceDrum.h
 *
 *  Created on: 08 ago 2020
 *      Author: Benjamin
 */

#ifndef SEQUENCEDRUM_H_
#define SEQUENCEDRUM_H_

#include <Audio.h>
#include "NoteFeatures.h"
#include "Sequence.h"

class SequenceDrum {
public:
	SequenceDrum();
	virtual ~SequenceDrum();

////////////////////////////////////////////////

	AudioSynthSimpleDrum drum;

	Sequence sequence;

	void drumPlay();

	// Drum Settings
	void setFrequency(float);
	void setNote(Note_t selectedNote);
	void setLength(float millis);
	void setPitchMod(float modAmount);

private:
	void setDefaultDrumParams();

	// General purpose variables
};

#endif /* SEQUENCEDRUM_H_ */
