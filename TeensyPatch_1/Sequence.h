/*
 * Sequence.h
 *
 *  Created on: 22 apr 2020
 *      Author: Benjamin
 */

#ifndef SEQUENCE_H_
#define SEQUENCE_H_

#include "utility/dspinst.h"

enum {
	Step_1, Step_2, Step_3, Step_4, Step_5, Step_6, Step_7, Step_8,
	Step_9, Step_10, Step_11, Step_12, Step_13, Step_14, Step_15, Step_16,
	Step_17, Step_18, Step_19, Step_20, Step_21, Step_22, Step_23, Step_24,
	Step_25, Step_26, Step_27, Step_28, Step_29, Step_30, Step_31, Step_32,
	STEP_NUMBER
};

class Sequence {

private:

public:
	uint32_t currentSequence;
	Sequence();
	virtual ~Sequence();

	void setSequenceStep(int stepNumber);
	bool isStep_On(int stepNumber);


};

#endif /* SEQUENCE_H_ */
