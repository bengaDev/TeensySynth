/*
 * ShiftingSine.cpp
 *
 *  Created on: 25 ott 2020
 *      Author: Benjamin
 */

#include "ShiftingSine.h"

ShiftingSine::ShiftingSine() : AudioStream(1, inputQueueArray)
{
    length(600);
    targetFrequency(60);
    freqMod(0.5);
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
    timeEnvDecrement = (ShiftingSine::TIME_ENV_INTERVALS/len_samples);

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
}

void ShiftingSine::freqMod(float depth)
{
	  // Validate parameter
	  if(depth < 0)
	    depth = 0;
	  else if(depth > 1.0)
	    depth = 1.0;

	  depth += 1.0;

	  // Turn 1.0 to 2.0 into a reasonable scale.
	  // The scale that is chosen is to go up and down of 2 octaves.
	  // Going up of 1 octave doubles frequency (and thus Phase Increment), going down of 1 octave halves it.
	  // This means that the range will be [Ph_incr/4, Ph_incr*4].
	  // The human ear perception of a frequency shift is logarithmic with base 2.

}

ShiftingSine::~ShiftingSine() {
	// TODO Auto-generated destructor stub
}

