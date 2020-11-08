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

#include "DSP_Arithmetic/ExpLUT.c"
#include "DSP_Arithmetic/LogLUT.c"

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

	  uint32_t timeEnvCurrent;
	  uint32_t timeEnvIncrement;
	  uint32_t targetPhaseIncrement;
	  uint32_t currentPhaseIncrement;
	  uint32_t phaseAccumulator;

	  // Linear Phase variables
	  int32_t linearLUTIncrementDiff;
	  uint32_t linearCurrentLUT_Idx;
	  uint32_t linearStartLUT_Idx;

	  uint32_t LINEAR_LUT_IDX_MAX;// = (uint32_t)((20000.0 * (PHASE_INCR_INTERVALS / AUDIO_SAMPLE_RATE_EXACT)) + 0.5);
	  uint32_t LINEAR_LUT_IDX_MIN;// = (uint32_t)((40.0 * (PHASE_INCR_INTERVALS / AUDIO_SAMPLE_RATE_EXACT)) + 0.5);

	  //uint32_t lowerPhaseIncrement, higherPhaseIncrement;
	  uint32_t phaseIncrementDiffTotal;

	  // DSP Utilities
	  float roundingFactor;

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
	  static const uint32_t TIME_ENV_INTERVALS = 0x40000000;
	  static constexpr float PHASE_INCR_INTERVALS = 4294967296.0; //0x7FFFFFFF;
	  static const uint32_t PHASE_INCR_MAX = (uint32_t)((20000.0 * (PHASE_INCR_INTERVALS / AUDIO_SAMPLE_RATE_EXACT)) + 0.5);
	  static const uint32_t PHASE_INCR_MIN = (uint32_t)((40.0 * (PHASE_INCR_INTERVALS / AUDIO_SAMPLE_RATE_EXACT)) + 0.5);
};

#endif /* SHIFTINGSINE_H_ */
