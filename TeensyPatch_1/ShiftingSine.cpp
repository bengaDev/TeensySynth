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
extern bool DebugActive;

// ----------------------------------------------------------------------------------------------------------
// OPTIMIZED SELF-MODULATING SINE WITH REALISTIC LOGARITHMIC SCALE:
// 		Author: Benjamin Lorin Libby
//
// OVERVIEW:	This class produces a sine wave which modulates in frequency on a given time range.
// 				In other words, the wave will start from an initial frequency and gradually shift it towards
// 				a final target frequency in a given time slot.
// 				This frequency 'shift' (or modulation) should follow a logarithmic scale to be more realistic
// 				(human ear perceives frequency variations with a log2 scale) but in most algorithms, due to
// 				computational complexity, it is either linear or parabolic (approximation of log).
// 				Here, a logarithmic shift is made possible thanks to a highly optimized method to calculate
// 				logarithm (and its inverse exponential): it uses bit twiddling techniques on IEEE-754 float
// 				numbers and the complexity is comparable to the one of a simple access to a LUT table.
// 				This technique was tested on a 64-bit Windows machine with an Intel Core i5-4210U, and
// 				results show that the speed of a base 2 log operation increased by a factor of x22 respect
// 				to its counterpart standard GNU C functions.

// 				The basic parameters controlling this algorithm are:
// 				- A target frequency (or ending frequency), which is the frequency the wave will reach at
// 				  the end of modulation.
// 				- A time interval, which sets the time during which modulation occurs.
// 				- A modulation amount. This number is an abstraction and it basically sets the frequency
// 				  that the wave will have at the beginning of modulation (starting frequency).
//
// NOTE:		A phase increment is just another way of describing a frequency, which is easier for
// 			  	calculations. When using the term 'frequency modulation' here, what is actually meant is a
//				'phase increment modulation'.
//
// IN DEPTH:	Given a certain 'targetPhaseIncrement' (frequency), this synth performs a frequency modulation
//				by shifting the phase increment (frequency) up or down in time.
// 				The modulation is based on two basic parameters:
// 				- Time duration: managed in the 'length()' method
// 				- Modulation amount: managed in the 'freqMod()' method
// 				The frequency modulation amount is controlled by a parameter, the 'depth', which is LINEAR.
// 				The human ear IS NOT LINEAR in frequency domain: it is LOGARITHMIC in base 2, so a
// 				transformation needs to be applied.
//
// 				The scale that is chosen is to go up and down of 2 octaves.
// 				Going up of 1 octave doubles frequency (and thus Phase Increment), going down of 1 octave
// 				halves it.
// 				This means that, given a target phase increment 'Ph_incr', the maximum range will be
// 				[Ph_incr/4, Ph_incr*4] -> (Ph_incr * [0.25, 4.0]).
// ----------------------------------------------------------------------------------------------------------

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
	linearCurrentMultiplier = linearStartMultiplier;
	phaseAccumulator = 0;

	// Turn 'linearCurrentMultiplier' [0x0, 0x80000000] into a multiplier increment difference, depending on duration of modulation.
	// This can also be seen as a phase increment difference (=frequency difference), i.e. the amount of
	// phase increment to add/subtract at each clock cycle.
	// NOTE: linearCurrentMultiplier = 0x40000000 corresponds to an actual multiplier of 1.0
	// 	timeEnvIncrement : TIME_ENV_INTERVALS = x : (0x40000000 - linearCurrentMultiplier)
	linearMultiplierIncrementDiff = (int32_t)( ( (1.0 - ((float)linearCurrentMultiplier/(float)ShiftingSine::TIME_ENV_INTERVALS)) * (float)timeEnvIncrement) + roundingFactor);

	__enable_irq();
}

void ShiftingSine::length(float milliseconds)
{
    if(milliseconds < 0)
      return;
    if(milliseconds > 10000)
    	milliseconds = 10000;

    // Calculates the number of samples having T=milliseconds and sampling Freq. Fs=44117.64706
    // # of samples = (T(s) * Fs(Hz))
    int32_t len_samples = (uint32_t)((milliseconds*(AUDIO_SAMPLE_RATE_EXACT/1000.0)) + 0.5 );

    // Calculates the amount of Time Envelope Decrement to apply at each clock cycle (where
    // 	clock has freq. Fs: the period corresponds to the time between one sample and another).
    timeEnvIncrement = (uint32_t)(((float)ShiftingSine::TIME_ENV_INTERVALS/(float)len_samples) + 0.5);

    // NOTE: Total time envelope intervals are 0x40000000.
    // 		 This value was chosen so that it makes future calculations easier and more efficient (this
    // 		 will become clear when performing the phase increment algorithm).
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
    // F_out	: The resulting output frequency of the wave when sampling at Fs
    // F_wave	: The output wave frequency that is wanted
    // Ph_incr	: The phase increment (how many samples must be skipped at each sampling period)

    // 		F_out = (Fs / N)	=>	F_wave = (F_out * Ph_incr) = ((Fs / N) * Ph_incr)
    // 		Ph_incr = (F_wave * (N / Fs))

    // NOTE: N here is scaled up (=4294967296) and is much larger than the actual wavetable length (=257).
    //		 This is to have more precision during calculations.

    targetPhaseIncrement = (uint32_t)((freq * (PHASE_INCR_INTERVALS / AUDIO_SAMPLE_RATE_EXACT)) + 0.5);



    // ----------------------------------------------------------------------------------------------------------
    // BOUNDARIES (MIN/MAX) calculation:
    // 		A target phase increment is now set. Applying a frequency modulation to it means multiplying
    // 		this phase increment by a multiplier which changes in time.
    // 		As per design, the MIN and MAX values of this multiplier are set to 0.25 and 4.0 respectively.
    // 		Also, an additional limit is given by the range of human audible frequencies [40Hz, 20kHz].
    // 		Here, the MIN and MAX bounds are calculated following the below steps:
    // 			- Calculation of the ratio between MIN/MAX phase increment and target phase increment. This
    // 			  gives a 'multiplier' constant, which must remain between 0.25 and 4.0.
    // 			- This multiplier follows the log2 scale, and it needs to be re-mapped to a linear scale in order
    // 			  to make calculations simpler. The linear interval is described with uint32_t and its range is
    // 			  [0x0, 0x80000000]. This means that 0.25 in log scale corresponds to 0x0, and 4.0 to 0x80000000.
    // 			- If the 'multiplier' is within its bounds, an optimized log2 function is applied to it: this
    // 			  gives a linear interpretation of it.
    // 			- If the 'multiplier' is not within its bounds, the default bounds of linear scale are chosen.
    // 			- The last step is to re-map the resulting linear scale from float to a reasonable uint32_t
    // 			  range. This is done to limit as much as possible the calculations to 'int arithmetic'.
    // ----------------------------------------------------------------------------------------------------------
    FloatBits_t multiplier;
    FloatBits_t logVal;


    // MAX:
    // 1. Calculation of multiplier
    multiplier.fValue = ((float)PHASE_INCR_MAX / (float)targetPhaseIncrement);

    // 2. Is multiplier within its bounds?
    if(multiplier.fValue >= 4.0)
    	LINEAR_MULTIPLIER_MAX = 0x80000000;
    else
    {
    	// 3. Transformation of multiplier from log2 to linear scale: [0.25, 4.0] -> [-2.0, 2.0]

        // 						| ------ Integer Part ------ |   | ------------------ Fractional Part ------------------- |
        logVal.fValue = (float)((multiplier.bits >> 23) - 127) + log_512[(multiplier.bits >> LOG_Q) & LOG_LUT_MASK].fValue;


        // 4. Transformation of multiplier from a float linear scale to a uint32_t one: [-2.0, 2.0] -> [0x0, 0x80000000]
    	LINEAR_MULTIPLIER_MAX = (uint32_t)(((logVal.fValue + 2.0) * (float)(1 << 29)) + 0.5);
    }


    // MIN:
    multiplier.fValue = ((float)PHASE_INCR_MIN / (float)targetPhaseIncrement);
    if(multiplier.fValue <= 0.25)
    	LINEAR_MULTIPLIER_MIN = 0x00000000;
    else
    {
        // 						| ------ Integer Part ------ |   | ------------------ Fractional Part ------------------- |
        logVal.fValue = ((float)(multiplier.bits >> 23) - 127) + log_512[(multiplier.bits >> LOG_Q) & LOG_LUT_MASK].fValue;

    	LINEAR_MULTIPLIER_MIN = (uint32_t)(((logVal.fValue + 2.0) * (float)(1 << 29)) + 0.5);
    }
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

	// Calculating "starting linear multiplier" (an 'exp' operation will have to be performed in order to have the
	// real starting multiplier value)
	// depth : 4.0 = x : 0x80000000 	-> 	x = (depth * 0x80000000) / 4.0
	//IdxLUT = (int)((float)((depth * (float)(1 << 31)) >> 2) + 0.5);
	linearStartMultiplier = (uint32_t)((depth * 536870912.0) + 0.5);


	// Boundary check (min and max bounds are calculated in 'targetFrequency' method)
	if(linearStartMultiplier > LINEAR_MULTIPLIER_MAX)
		linearStartMultiplier = LINEAR_MULTIPLIER_MAX;
	else if(linearStartMultiplier < LINEAR_MULTIPLIER_MIN)
		linearStartMultiplier = LINEAR_MULTIPLIER_MIN;


	// Assigning "starting linear multiplier" to "current linear multiplier", which keeps track of the current
	// value of multiplier.
	linearCurrentMultiplier = linearStartMultiplier;
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

			linearCurrentMultiplier += (uint32_t)linearMultiplierIncrementDiff;

			currentPhaseIncrement = ((float)targetPhaseIncrement * exp_512[linearCurrentMultiplier >> 22].fValue);

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

