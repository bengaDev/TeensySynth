/*
 * MidiNote.cpp
 *
 *  Created on: 13 apr 2020
 *      Author: Benjamin
 */

#include "SequenceOsc.h"
/*
MidiNote::MidiNote(void): patchCord1(oscillator, envelope) {
	// TODO Auto-generated constructor stub
	setDefaultDAHDSR();
	setDefaultWaveformParams();
}*/

SequenceOsc::SequenceOsc(float ampl, float freq): patchCord1(oscillator, envelope) {
	// TODO Auto-generated constructor stub
	setDefaultDAHDSR();
	setDefaultWaveformParams();
	envelope.releaseNoteOn(1.0);
	oscillator.begin(waveform);
}

SequenceOsc::~SequenceOsc() {
	// TODO Auto-generated destructor stub
}

void SequenceOsc::setDefaultDAHDSR()
{
	setDAHDSR(0.0, 9.2, 2.1, 31.4, 50, 0.6, 84.5);
	isSustainPhase = false;
	lastSustainMillis = 0;
}

void SequenceOsc::setDefaultWaveformParams()
{
	setAmpl_Freq(0.0, 0.0);
	waveform = WAVEFORM_SINE;
	//setWaveformType(WAVEFORM_SINE);
}

void SequenceOsc::setDAHDSR(float delay, float attack, float hold, float decay,
		unsigned long sustainMillis, float sustainLvl, float release)
{
	delayMillis = delay;
	attackMillis = attack;
	holdMillis = hold;
	decayMillis = decay;
	sustainDuration = sustainMillis;
	sustainLevel = sustainLvl;
	releaseMillis = release;

	envelope.delay(delayMillis);
	envelope.attack(attackMillis);
	envelope.hold(holdMillis);
	envelope.decay(decayMillis);
	envelope.sustain(sustainLevel);
	envelope.release(releaseMillis);

}

void SequenceOsc::setDelay(float delay)
{
	delayMillis = delay;

	envelope.delay(delayMillis);
}

void SequenceOsc::setAttack(float attack)
{
	attackMillis = attack;

	envelope.attack(attackMillis);
}

void SequenceOsc::setHold(float hold)
{
	holdMillis = hold;

	envelope.hold(holdMillis);
}

void SequenceOsc::setDecay(float decay)
{
	decayMillis = decay;

	envelope.decay(decayMillis);
}

void SequenceOsc::setSustain(float sustainLvl, float sustainDur)
{
	sustainDuration = sustainDur;

	if(sustainLvl >= 0)
	{
		sustainLevel = sustainLvl;
		envelope.sustain(sustainLevel);
	}
}

void SequenceOsc::setRelease(float release)
{
	releaseMillis = release;

	envelope.release(releaseMillis);
}

void SequenceOsc::setAmplitude(float ampl)
{
	amplitude = ampl;

	oscillator.amplitude(amplitude);
}

void SequenceOsc::setFrequency(float freq)
{
	frequency = freq;

	oscillator.frequency(frequency);
}

int SequenceOsc::getWaveformType()
{
	return waveform;
}

void SequenceOsc::setWaveformType(int wf)
{
	waveform = wf;
	oscillator.begin(waveform);
}

void SequenceOsc::setAmpl_Freq(float ampl, float freq)
{
	amplitude = ampl;
	frequency = freq;

	oscillator.amplitude(amplitude);
	oscillator.frequency(frequency);
}

void SequenceOsc::setNote(Note_t selectedNote)
{
	frequency = Note[selectedNote];
	oscillator.frequency(frequency);
}

bool SequenceOsc::isSustainEnd()
{
	DEBUG_Time = millis();
	if( (DEBUG_Time - lastSustainMillis) > sustainDuration)
	{
		return true;
	}

	return false;
}

void SequenceOsc::notePlay()
{
	envelope.noteOn();

	lastSustainMillis = 0;
	isSustainPhase = false;
	/*if(envelope.isActive() && isStepOverflow == false)
	{
		envelope.releaseNoteOn(0);
		isStepOverflow = true;
	}
	else if(!envelope.isActive())
	{
		isStepOverflow = false;
		envelope.noteOn();
	}
	*/
}

void SequenceOsc::UpdateState()
{
	if(envelope.isSustain() && isSustainPhase == false)
	{
		lastSustainMillis = millis();
		isSustainPhase = true;
	}

	if(isSustainPhase == true && isSustainEnd())
	{
		envelope.noteOff();
		isSustainPhase = false;
	}
}

