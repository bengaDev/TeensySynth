/*
 * SequenceKick.cpp
 *
 *  Created on: 22 set 2020
 *      Author: Benjamin
 */

#include "SequenceKick.h"

/*
SequenceKick::SequenceKick() : oscillatorTone(0.1, 50) {
	// TODO Auto-generated constructor stub

}
*/

SequenceKick::SequenceKick() {
	// TODO Auto-generated constructor stub

}

SequenceKick::~SequenceKick() {
	// TODO Auto-generated destructor stub
}

AudioStream* SequenceKick::getKickAudioOut() {
	//return &(oscillatorTone.envelope);
	return &(shiftingSine);
}

void SequenceKick::setFreq(float freq)
{
	shiftingSine.targetFrequency(freq);
}

void SequenceKick::setLength(int32_t millis)
{
	shiftingSine.length(millis);
}

void SequenceKick::setMod(float modAmount)
{
	shiftingSine.freqMod(modAmount);
}

void SequenceKick::notePlay()
{
	//oscillatorTone.envelope.noteOn();
	shiftingSine.noteOn();
}

