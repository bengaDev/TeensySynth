/*
 * SequenceDrum.cpp
 *
 *  Created on: 08 ago 2020
 *      Author: Benjamin
 */

#include "SequenceDrum.h"

SequenceDrum::SequenceDrum() {
	// TODO Auto-generated constructor stub
	setDefaultDrumParams();
}

SequenceDrum::~SequenceDrum() {
	// TODO Auto-generated destructor stub
}

void SequenceDrum::drumPlay()
{
	drum.noteOn();
}

void SequenceDrum::setFrequency(float freq)
{
	drum.frequency(freq);
}

void SequenceDrum::setNote(Note_t selectedNote)
{
	float frequency = Note[selectedNote];
	drum.frequency(frequency);
}

void SequenceDrum::setLength(float millis)
{
	drum.length(millis);
}

void SequenceDrum::setPitchMod(float modAmount)
{
	drum.pitchMod(modAmount);
}

void SequenceDrum::setDefaultDrumParams()
{
	drum.frequency(200);
	drum.length(500);
	drum.pitchMod(0.5);
}
