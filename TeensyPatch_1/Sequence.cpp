/*
 * Sequence.cpp
 *
 *  Created on: 22 apr 2020
 *      Author: Benjamin
 */

#include "Sequence.h"

Sequence::Sequence() {
	// TODO Auto-generated constructor stub
	currentSequence = 0x00000000;
}

Sequence::~Sequence() {
	// TODO Auto-generated destructor stub
}

void Sequence::setSequenceStep(int stepNumber)
{
	currentSequence = currentSequence ^ (1 << stepNumber);
}

bool Sequence::isStep_On(int stepNumber)
{
	return ((currentSequence & (1 << stepNumber)) != 0) ? true : false;
}

