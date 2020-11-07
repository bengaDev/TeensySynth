/*
 * ShiftingSine.h
 *
 *  Created on: 25 ott 2020
 *      Author: Benjamin
 */

#ifndef SHIFTINGSINE_H_
#define SHIFTINGSINE_H_
#include "AudioStream.h"
#include "utility/dspinst.h"

class ShiftingSine : public AudioStream {
public:
	ShiftingSine();// : AudioStream(1, inputQueueArray)

	void noteOn();
	void targetFrequency(float freq);
	void length(int32_t milliseconds);
	void freqMod(float depth);

	using AudioStream::release;
	virtual void update(void);

	virtual ~ShiftingSine();
private:
	  audio_block_t *inputQueueArray[1];

	  uint32_t timeEnvDecrement;
	  uint32_t targetPhaseIncrement;
	  uint32_t lowerPhaseIncrement, higherPhaseIncrement;
	  uint32_t phaseIncrementDiffTotal;

	  // ------------------------------------------------------------------------------------
	  // Constant Definitions
	  // ------------------------------------------------------------------------------------

	  // The following constants are:
	  // 	- static: 	Meaning it belongs to the current class, not to any instance of the class.
	  // 				It is accessible from outside with: 	className::varName
	  // 	- const: 	Means that it is a read-only variable (it cannot be modified)
	  // 	- private: 	It can be accessed through previously defined method [className::varName] only from
	  // 				members belonging to that class (methods).
	  // 				If it was public, it could be accessed from any file that includes this header,
	  // 				through className::varName, or from any instance of this class, as a normal member
	  // 				instance.varName
	  static const uint32_t TIME_ENV_INTERVALS = 0x80000000; // 2147483648
	  static const float PHASE_INCR_INTERVALS = 4294967296.0;
};

#endif /* SHIFTINGSINE_H_ */
