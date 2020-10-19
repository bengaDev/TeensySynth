/*
 * SequenceKick.cpp
 *
 *  Created on: 22 set 2020
 *      Author: Benjamin
 */

#include "SequenceKick.h"

SequenceKick::SequenceKick() : oscillatorTone(0.1, 50) {
	// TODO Auto-generated constructor stub

}

SequenceKick::~SequenceKick() {
	// TODO Auto-generated destructor stub
}

AudioEffectEnvelope* SequenceKick::getKickAudioOut() {
	return &(oscillatorTone.envelope);
}

void SequenceKick::notePlay()
{
	oscillatorTone.envelope.noteOn();
}

