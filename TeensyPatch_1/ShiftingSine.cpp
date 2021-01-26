/*
 * ShiftingSine.cpp
 *
 *  Created on: 25 ott 2020
 *      Author: Benjamin
 */

#include "ShiftingSine.h"

extern "C" {
extern const int16_t AudioWaveformSine[257];
}

ShiftingSine::ShiftingSine() : AudioStream(1, inputQueueArray)
{
	timeEnvCurrent = ShiftingSine::TIME_ENV_INTERVALS + 1;
    length(600);
    targetFrequency(60);
    freqMod(0.5);
}

void ShiftingSine::noteOn(void)
{
	__disable_irq();

	timeEnvCurrent = 0;
	linearCurrentLUT_Idx = linearStartLUT_Idx;
	phaseAccumulator = 0;

	__enable_irq();
}

void ShiftingSine::length(int32_t milliseconds)
{
    if(milliseconds < 0)
      return;
    if(milliseconds > 10000)
    	milliseconds = 10000;

    // Calculates the number of samples having T=milliseconds and sampling Freq. Fs=44117.64706
    // # of samples = (T(s) * Fs(Hz))
    int32_t len_samples = milliseconds*(AUDIO_SAMPLE_RATE_EXACT/1000.0);

    // Calculates the amount of Time Envelope Decrement to apply at each clock cycle (where
    // 	clock has freq. Fs: the period corresponds to the time between one sample and another).
    timeEnvIncrement = (ShiftingSine::TIME_ENV_INTERVALS/len_samples);

    // Total time envelope intervals are 0x80000000.
    // This value was chosen so that it makes future calculations easier and more efficient (this
    // 	will become clear when performing the phase increment algorithm).
    // For now, just note that the most significant 16-bit [=0x8000] is a power of 2, and its
    // 	square doesn't exceed 32-bit: (0x8000)^2 = 0x40000000
}

void ShiftingSine::targetFrequency(float freq)
{
	// Checking whether the input if frequency is greater than Fs/2 (Nyquist Theorem)
    if(freq < 0.0)
      freq = 0;
    else if(freq > (AUDIO_SAMPLE_RATE_EXACT/2))
      freq = AUDIO_SAMPLE_RATE_EXACT/2;

    // The following formula comes from Wavetable Synthesis theory:
    // N		: Wavetable length
    // Fs		: Sampling frequency (this is the default frequency between two samples of the WT)
    // F_out	: The resulting output frequency of the wave with when sampling at Fs
    // F_wave	: The output wave frequency that is wanted
    // Ph_incr	: The phase increment (how many samples must be skipped at each sampling period)

    // 		F_out = (Fs / N)	=>	F_wave = (F_out * Ph_incr) = ((Fs / N) * Ph_incr)
    // 		Ph_incr = (F_wave * (N / Fs))

    targetPhaseIncrement = (uint32_t)((freq * (PHASE_INCR_INTERVALS / AUDIO_SAMPLE_RATE_EXACT)) + 0.5);

    FloatBits_t multiplier;
    FloatBits_t logVal;

    // -----------------------------------------------------------------------------------------
    // MAX
    // -----------------------------------------------------------------------------------------
    multiplier.fValue = ((float)PHASE_INCR_MAX / (float)targetPhaseIncrement);

    // 						| ------ Integer Part ------ |   | ------------------ Fractional Part ------------------- |
    logVal.fValue = (float)((multiplier.bits >> 23) - 127) + log_512[(multiplier.bits >> LOG_Q) & LOG_LUT_MASK].fValue;
    Serial.println(logVal.fValue);

    //logVal.fValue += 2.0;

    //if(logVal.fValue > 4.0)
    if(logVal.fValue > 2.0)
    	LINEAR_LUT_IDX_MAX = 0x80000000;
    else
    	LINEAR_LUT_IDX_MAX = (uint32_t)((logVal.fValue * (float)(1 << 29)) + 0.5);


    // -----------------------------------------------------------------------------------------
    // MIN
    // -----------------------------------------------------------------------------------------
    multiplier.fValue = ((float)PHASE_INCR_MIN / (float)targetPhaseIncrement);

    // 						| ------- Integer Part ------ |   | ------------------ Fractional Part ------------------- |
    logVal.fValue = ((float)((multiplier.bits >> 23) & 0xFF) - 127.0) + log_512[(multiplier.bits >> LOG_Q) & LOG_LUT_MASK].fValue;

    //logVal.fValue += 2.0;

    //if(logVal.fValue < 0.0)
    if(logVal.fValue < -2.0)
    	LINEAR_LUT_IDX_MIN = 0x00000000;
    else
    	LINEAR_LUT_IDX_MIN = (uint32_t)((logVal.fValue * (float)(1 << 29)) + 0.5);
}

void ShiftingSine::freqMod(float depth)
{
	// Validate parameter
	if(depth < 0.0)
		depth = 0.0;
	else if(depth > 4.0)
		depth = 4.0;

	roundingFactor = 0.5;
	if(depth < 2.0)
		roundingFactor = -0.5;

	// depth : 4.0 = x : 0x80000000 	-> 	x = (depth * 0x80000000) / 4.0
	//IdxLUT = (int)((float)((depth * (float)(1 << 31)) >> 2) + 0.5);
	linearStartLUT_Idx = (uint32_t)((depth * (float)(1 << 29)) + 0.5);

	Serial.println("Inside MOD:");
	if(linearStartLUT_Idx > LINEAR_LUT_IDX_MAX)
	{
		Serial.print("	linearStartLUT_Idx GREATER: ");
		Serial.println(linearStartLUT_Idx, HEX);
		linearStartLUT_Idx = LINEAR_LUT_IDX_MAX;
	}
	else if(linearStartLUT_Idx < LINEAR_LUT_IDX_MIN)
	{
		Serial.print("	linearStartLUT_Idx = ");
		Serial.print(linearStartLUT_Idx, HEX);
		Serial.print(" SMALLER than LINEAR_LUT_IDX_MIN = ");
		Serial.print(LINEAR_LUT_IDX_MIN, HEX);
		Serial.println(LINEAR_LUT_IDX_MIN, DEC);
		linearStartLUT_Idx = LINEAR_LUT_IDX_MIN;
	}


	linearCurrentLUT_Idx = linearStartLUT_Idx;

	// Turn 0.0 to 4.0 into a phase increment difference (=frequency difference).
	// The scale that is chosen is to go up and down of 2 octaves.
	// Going up of 1 octave doubles frequency (and thus Phase Increment), going down of 1 octave halves it.
	// This means that the range will be [Ph_incr/4, Ph_incr*4].
	// The human ear perception of a frequency shift is logarithmic with base 2.
	//startPhaseIncrement = targetPhaseIncrement * exp_512[(depth * EXP_LUT_FACTOR) + 0.5];
	//startPhaseIncrement = targetPhaseIncrement * exp_512[linearCurrentLUT_Idx >> 22].fValue;

	// Can be placed either here or in length function, depending on where it is most efficient (in the least used of the two methods)
	linearLUTIncrementDiff = (int32_t)( ( (1.0 - ((float)linearCurrentLUT_Idx/(float)ShiftingSine::TIME_ENV_INTERVALS)) * timeEnvIncrement) + roundingFactor);


	Serial.print("	linearLUTIncrementDiff: ");
	Serial.println(linearLUTIncrementDiff);
	/*
	Serial.print("0x40000000/0x40000000 - 1 = ");
	Serial.println((((float)linearCurrentLUT_Idx/(float)ShiftingSine::TIME_ENV_INTERVALS) - 1.0));

	Serial.print("0x40000000/0x40000000 = ");
	Serial.println(((float)linearCurrentLUT_Idx/(float)ShiftingSine::TIME_ENV_INTERVALS));

	Serial.print("linearCurrentLUT_Idx = ");
	Serial.println((float)linearCurrentLUT_Idx);

	Serial.print("TIME_ENV_INTERVALS = ");
	Serial.println((float)ShiftingSine::TIME_ENV_INTERVALS);
	*/
	//currentPhaseIncrement = startPhaseIncrement;
}

void ShiftingSine::update(void)
{
	audio_block_t *block_wav;
	int16_t *p_wave, *end;
	int32_t sin_l, sin_r, interp;
	int32_t index, scale;

	block_wav = allocate();
	if (!block_wav) return;
	p_wave = (block_wav->data);
	end = p_wave + AUDIO_BLOCK_SAMPLES;

	// 50 is arbitrary threshold...
	// low values of second are inaudible, and we can save CPU cycles
	// by not calculating second when it's really quiet.
	//do_second = (wav_amplitude2 > 50);

	while(p_wave < end)
	{
		// If time envelope has expired, it means that signal has reached its target frequency
		if(timeEnvCurrent >= ShiftingSine::TIME_ENV_INTERVALS)
		{
			// If envelope has expired, then stuff zeros into output buffer.
			*p_wave = 0;
			p_wave++;
		}
		else
		{
			timeEnvCurrent += timeEnvIncrement;

			if(timeEnvCurrent >= ShiftingSine::TIME_ENV_INTERVALS)
				timeEnvCurrent = ShiftingSine::TIME_ENV_INTERVALS;

			timeEnvSqr_Norm = (timeEnvCurrent >> 15);
			timeEnvSqr_Norm *= timeEnvSqr_Norm;
			gain = ((ShiftingSine::TIME_ENV_INTERVALS - timeEnvSqr_Norm) >> 15);

			linearCurrentLUT_Idx += linearLUTIncrementDiff;


			currentPhaseIncrement = ((float)targetPhaseIncrement * exp_512[linearCurrentLUT_Idx >> 22].fValue);

			phaseAccumulator += currentPhaseIncrement;
			//phaseAccumulator &= 0xFFFFFFFF;

			// Phase to Sine lookup * interp:
			index = phaseAccumulator >> 24; // take top valid 8 bits
			sin_l = AudioWaveformSine[index];
			sin_r = AudioWaveformSine[index+1];

			// Linear Interp
			scale = (phaseAccumulator >> 8) & 0xFFFF;
			sin_r *= scale;
			sin_l *= 0x10000 - scale;
			interp = sin_l + sin_r;

			*p_wave = multiply_32x32_rshift32(interp, gain);
			p_wave++;
		}
	}

	transmit(block_wav, 0);
	release(block_wav);

}

ShiftingSine::~ShiftingSine() {
	// TODO Auto-generated destructor stub
}

