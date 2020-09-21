/*
 * StateMachine.cpp
 *
 *  Created on: 01 mag 2020
 *      Author: Benjamin
 */

#include "StateMachine.h"

#define NUM_WAVES	3
#define VOLUME		0.1
#define BPM			130.0
#define SUSTAIN 	100 //ms
#define BPM_DIV		4
#define NUM_STEPS	16
#define NUM_OSCILS	2
#define NUM_DRUMS	2

float BPM_ClkFreq = ((BPM * BPM_DIV) / 60.0);
float BPM_ClkPeriod = (1 / (BPM_ClkFreq)) * 1000.0; // In 'ms'

float knob_A4, knob_A5, cutoffFreq;

unsigned long last_time;
// MIXERS SECTIONS
AudioMixer4     mixer1, mixer2;


// AUDIO SYNTHESIS COMPONENT
AudioSynthSimpleDrum drum1, drum2;

SequenceOsc oscil[NUM_OSCILS] = {
		SequenceOsc(0.1, 400),
		SequenceOsc(0.1, 400)
};

SequenceDrum drums[NUM_DRUMS] = {
		SequenceDrum(),
		SequenceDrum()
};


AudioFilterStateVariable filter1;
AudioAmplifier mainVolume;

AudioConnection patchCord0(oscil[0].envelope, 0, mixer1, 0);
AudioConnection patchCord1(oscil[1].envelope, 0, mixer1, 1);
AudioConnection patchCord2(drums[0].drum, 0, mixer2, 0);
AudioConnection patchCord6(drums[1].drum, 0, mixer2, 1);
AudioConnection patchCord3(mixer1, 0, filter1, 0);
AudioConnection patchCord4(mixer1, 0, filter1, 1);
AudioConnection patchCord5(filter1, 0, mixer2, 3);

// Now create 2 mixers for the main output
AudioMixer4     mixerLeft;
AudioMixer4     mixerRight;
AudioOutputI2S  audioOut;

// Mix all channels to both the outputs
AudioConnection patchCord33(mixer2, 0, mainVolume, 0);
AudioConnection patchCOrd34(mainVolume, 0, mixerLeft, 0);
AudioConnection patchCord37(mainVolume, 0, mixerRight, 0);
AudioConnection patchCord41(mixerLeft, 0, audioOut, 0);
AudioConnection patchCord42(mixerRight, 0, audioOut, 1);


float myMap(float num, float startScale_IN, float endScale_IN, float startScale_OUT, float endScale_OUT);

StateMachine::StateMachine() {
	// TODO Auto-generated constructor stub
	this->numOscils = NUM_OSCILS;
	this->numDrums = NUM_DRUMS;

	oscillator = (SequenceOsc**)malloc(numOscils * sizeof(SequenceOsc*));
	drum = (SequenceDrum**)malloc(numDrums * sizeof(SequenceDrum*));

	for(int i = 0; i < numOscils; i++)
	{
		oscillator[i] = &(oscil[i]);
	}

	for(int i = 0; i < numDrums; i++)
		drum[i] = &(drums[i]);

	step = 0;

	currentMode = INIT_MODE;
	lastSelectionMode = OSCIL_SELECTION_MODE;

	currentOscil = 0;
	currentDrum = 0;
}

StateMachine::~StateMachine() {
	// TODO Auto-generated destructor stub
	free(oscillator);
	free(drum);
}

void StateMachine::updateStateMachine()
{
	/*******************************************************/
	// MIDI Command Parser - [Clock frequency]
	if(usbMIDI.read())
		processMidiCommand();


	/*******************************************************/
	// Oscillator Envelope Update
	for(int i = 0; i < NUM_OSCILS; i++)
		oscillator[i]->UpdateState();



	/*******************************************************/
	// Step Sequencer Processor - [BPM based frequency]
	if(isClkTick())
	{
		manageStep(step);
		step = (step + 1) % STEP_NUMBER;
	}


	/*******************************************************/
	// Analog Processing
	knob_A5 = (float)analogRead(A5) / 1023.0;
	knob_A4 = (float)analogRead(A4) / 1023.0;

	cutoffFreq = knob_A4;

	//Serial.println(knob_A3);
	cutoffFreq = (cutoffFreq * cutoffFreq);
	cutoffFreq = myMap(cutoffFreq, 0.0, 1.0, 20.0, 10000.0);

	filter1.frequency(cutoffFreq);
	return;
}

bool StateMachine::isClkTick()
{
	if( (millis() - last_time) >= (unsigned long)(BPM_ClkPeriod))
	{
		last_time = millis();
		return true;
	}
	return false;
}

void StateMachine::manageStep(int stepNumber)
{
	//bool isStepOn[NUM_OSCILS] = {false}; // TODO

	for(int oscilN = 0; oscilN < NUM_OSCILS; oscilN++)
	{
		if(oscillator[oscilN]->sequence.isStep_On(stepNumber))
		{
			oscillator[oscilN]->notePlay();
			//isStepOn[oscilN] = true; // TODO
		}
	}

	for(int drumN = 0; drumN < NUM_DRUMS; drumN++)
	{
		if(drum[drumN]->sequence.isStep_On(stepNumber))
			drum[drumN]->drumPlay();
	}

	updateStepLed(stepNumber/*, isStepOn*/);

	//AkaiMidi.blinkStep(stepNumber, color);
	return;
}

void StateMachine::updateStepLed(int stepNum/*, bool *isStepOn*/) //TODO
{
	int prevColor = OFF;
	int prevStep = (stepNum == 0 ? (STEP_NUMBER - 1) : (stepNum - 1));

	if(lastSelectionMode == OSCIL_SELECTION_MODE)
	{
		if(oscillator[currentOscil]->sequence.isStep_On(prevStep))
		{
			prevColor = GREEN;
		}

		if(oscillator[currentOscil]->sequence.isStep_On(stepNum)/*isStepOn[currentOscil]*/) //TODO
			AkaiMidi.blinkStep(stepNum, RED, prevColor);
		else
			AkaiMidi.blinkStep(stepNum, YELLOW, prevColor);
	}
	else if(lastSelectionMode == DRUM_SELECTION_MODE)
	{
		if(drum[currentDrum]->sequence.isStep_On(prevStep))
		{
			prevColor = GREEN;
		}

		if(drum[currentDrum]->sequence.isStep_On(stepNum)/*isStepOn[currentOscil]*/) //TODO
			AkaiMidi.blinkStep(stepNum, RED, prevColor);
		else
			AkaiMidi.blinkStep(stepNum, YELLOW, prevColor);
	}
}

void StateMachine::manageTimeEnvelope(FaderFunction_t fader, byte MIDIVal)
{
	float envVal;

	switch(fader)
	{
		case ENV_DELAY:
			envVal = myMap((float)MIDIVal, 0.0, 127.0, 0.0, 1000.0);
			oscillator[currentOscil]->setDelay(envVal);
			break;
		case ENV_ATTACK:
			envVal = myMap((float)MIDIVal, 0.0, 127.0, 0.0, 1000.0);
			oscillator[currentOscil]->setAttack(envVal);
			break;
		case ENV_HOLD:
			envVal = myMap((float)MIDIVal, 0.0, 127.0, 0.0, 1000.0);
			oscillator[currentOscil]->setHold(envVal);
			break;
		case ENV_DECAY:
			envVal = myMap((float)MIDIVal, 0.0, 127.0, 0.0, 1000.0);
			oscillator[currentOscil]->setDecay(envVal);
			break;
		case ENV_SUSTAIN:
			envVal = myMap((float)MIDIVal, 0.0, 127.0, 0.0, 10000.0);
			oscillator[currentOscil]->setSustain(envVal, -1.0);
			break;
		case ENV_RELEASE:
			envVal = myMap((float)MIDIVal, 0.0, 127.0, 0.0, 1000.0);
			oscillator[currentOscil]->setRelease(envVal);
			break;
	}
}

void StateMachine::manageDrumParams(FaderFunction_t fader, byte MIDIVal)
{
	float drumParamFloat;

	switch(fader)
	{
		case DRUM_FREQ:
			drumParamFloat = ((MIDIVal * 0.555662323) * (MIDIVal * 0.555662323)) + 20.0;//myMap((float)MIDIVal, 0.0, 127.0, 20.0, 5000.0);
			drum[currentDrum]->setFrequency(drumParamFloat);
			break;
		case DRUM_LENGTH:
			drumParamFloat = myMap((float)MIDIVal, 0.0, 127.0, 20.0, 5000.0);
			drum[currentDrum]->setLength(drumParamFloat);
			break;
		case DRUM_PITCH_MOD:
			drumParamFloat = myMap((float)MIDIVal, 0.0, 127.0, 0.0, 1.0);
			drum[currentDrum]->setPitchMod(drumParamFloat);
			break;
	}
}

void StateMachine::processMidiCommand()
{
	  byte type, channel, data1, data2, cable;

	  // fetch the MIDI message, defined by these 5 numbers (except SysEX)
	  //
	  type = usbMIDI.getType();       // which MIDI message, 128-255
	  channel = usbMIDI.getChannel(); // which MIDI channel, 1-16
	  data1 = usbMIDI.getData1();     // first data byte of message, 0-127
	  data2 = usbMIDI.getData2();     // second data byte of message, 0-127
	  cable = usbMIDI.getCable();     // which virtual cable with MIDIx8, 0-7

	  if(type == usbMIDI.NoteOn)
	  {
		  if(currentMode != INIT_MODE)
		  {
			  Selection_t selType = AkaiMidi.getSelectionType(data1);

			  switch(selType){
				  case OSCIL_SELECT:
					  AkaiMidi.setDrumOnOff(currentDrum, false); // Turn off drum selection
					  AkaiMidi.setOscilOnOff(currentOscil, false);
					  currentOscil = AkaiMidi.getOscilIndex(data1);
					  AkaiMidi.setOscilOnOff(currentOscil, true);
					  currentMode = OSCIL_SELECTION_MODE;
					  lastSelectionMode = OSCIL_SELECTION_MODE;
					  refreshSequencer();
					  break;
				  case DRUM_SELECT:
					  AkaiMidi.setOscilOnOff(currentOscil, false); // Turn off oscil selection
					  AkaiMidi.setDrumOnOff(currentDrum, false);
					  currentDrum = AkaiMidi.getDrumIndex(data1);
					  AkaiMidi.setDrumOnOff(currentDrum, true);
					  currentMode = DRUM_SELECTION_MODE;
					  lastSelectionMode = DRUM_SELECTION_MODE;
					  refreshSequencer();
					  break;
				  case STEP_SELECT:
				  {
					  int stepNum = AkaiMidi.getSequencerStep(data1);

					  if(lastSelectionMode == OSCIL_SELECTION_MODE)
					  {
						  oscillator[currentOscil]->sequence.setSequenceStep(stepNum);
						  if(oscillator[currentOscil]->sequence.isStep_On(stepNum))
							  AkaiMidi.setStepOnOff(data1, true);
						  else
							  AkaiMidi.setStepOnOff(data1, false);
					  }
					  else if(lastSelectionMode == DRUM_SELECTION_MODE)
					  {
						  drum[currentDrum]->sequence.setSequenceStep(stepNum);
						  if(drum[currentDrum]->sequence.isStep_On(stepNum))
							  AkaiMidi.setStepOnOff(data1, true);
						  else
							  AkaiMidi.setStepOnOff(data1, false);
					  }

					  currentMode = SEQUENCER_MODE;
					  break;
				  }
				  case NOTHING:
					  break;
			  }
		  }
		  else
		  {
			  if(data1 == 127 && data2 == 62) // Initialization Sequence
				  InitPanel();
		  }
	  }
	  if(type == usbMIDI.ControlChange)
	  {
		  FaderIdx_t faderIdx = AkaiMidi.getFaderType(data1);

		  FaderFunction_t faderFunction = (FaderFunction_t)translateFaderIdxToFunction(faderIdx);

		  float volume;

		  switch(faderFunction){
		  	  case MAIN_VOLUME:
		  		  volume = myMap((float)data2, 0.0, 127.0, 0.0, 0.5);
		  		  mainVolume.gain(volume);
		  		  break;
		  	  case ENV_DELAY ... ENV_RELEASE:
			  	  manageTimeEnvelope(faderFunction, data2);
			  	  break;
		  	  case DRUM_FREQ ... DRUM_PITCH_MOD:
			  	  manageDrumParams(faderFunction, data2);
			  	  break;
		  	  case NULL_FADER_FUNCTION:
		  		  break;
		  }
	  }

	  /*
	  switch (type) {
	      case usbMIDI.NoteOff: // 0x80
	        Serial.print("Note Off, ch=");
	        Serial.print(channel, DEC);
	        Serial.print(", note=");
	        Serial.print(data1, DEC);
	        Serial.print(", velocity=");
	        Serial.println(data2, DEC);
	        break;

	      case usbMIDI.NoteOn: // 0x90
	        Serial.print("Note On, ch=");
	        Serial.print(channel, DEC);
	        Serial.print(", note=");
	        Serial.print(data1, DEC);
	        Serial.print(", velocity=");
	        Serial.println(data2, DEC);
	        break;

	      case usbMIDI.AfterTouchPoly: // 0xA0
	        Serial.print("AfterTouch Change, ch=");
	        Serial.print(channel, DEC);
	        Serial.print(", note=");
	        Serial.print(data1, DEC);
	        Serial.print(", velocity=");
	        Serial.println(data2, DEC);
	        break;

	      case usbMIDI.ControlChange: // 0xB0
	        Serial.print("Control Change, ch=");
	        Serial.print(channel, DEC);
	        Serial.print(", control=");
	        Serial.print(data1, DEC);
	        Serial.print(", value=");
	        Serial.println(data2, DEC);
	        break;

	      case usbMIDI.ProgramChange: // 0xC0
	        Serial.print("Program Change, ch=");
	        Serial.print(channel, DEC);
	        Serial.print(", program=");
	        Serial.println(data1, DEC);
	        break;

	      case usbMIDI.AfterTouchChannel: // 0xD0
	        Serial.print("After Touch, ch=");
	        Serial.print(channel, DEC);
	        Serial.print(", pressure=");
	        Serial.println(data1, DEC);
	        break;

	      case usbMIDI.PitchBend: // 0xE0
	        Serial.print("Pitch Change, ch=");
	        Serial.print(channel, DEC);
	        Serial.print(", pitch=");
	        Serial.println(data1 + data2 * 128, DEC);
	        break;

	      case usbMIDI.SystemExclusive: // 0xF0
	        // Messages larger than usbMIDI's internal buffer are truncated.
	        // To receive large messages, you *must* use the 3-input function
	        // handler.  See InputFunctionsComplete for details.
	        Serial.print("SysEx Message: ");
	        //printBytes(usbMIDI.getSysExArray(), data1 + data2 * 256);
	        Serial.println();
	        break;

	      case usbMIDI.TimeCodeQuarterFrame: // 0xF1
	        Serial.print("TimeCode, index=");
	        Serial.print(data1 >> 4, DEC);
	        Serial.print(", digit=");
	        Serial.println(data1 & 15, DEC);
	        break;

	      case usbMIDI.SongPosition: // 0xF2
	        Serial.print("Song Position, beat=");
	        Serial.println(data1 + data2 * 128);
	        break;

	      case usbMIDI.SongSelect: // 0xF3
	        Serial.print("Sond Select, song=");
	        Serial.println(data1, DEC);
	        break;

	      case usbMIDI.TuneRequest: // 0xF6
	        Serial.println("Tune Request");
	        break;

	      case usbMIDI.Clock: // 0xF8
	        Serial.println("Clock");
	        break;

	      case usbMIDI.Start: // 0xFA
	        Serial.println("Start");
	        break;

	      case usbMIDI.Continue: // 0xFB
	        Serial.println("Continue");
	        break;

	      case usbMIDI.Stop: // 0xFC
	        Serial.println("Stop");
	        break;

	      case usbMIDI.ActiveSensing: // 0xFE
	        Serial.println("Actvice Sensing");
	        break;

	      case usbMIDI.SystemReset: // 0xFF
	        Serial.println("System Reset");
	        break;

	      default:
	        Serial.println("Opps, an unknown MIDI message type!");
	    }
	  */
}

int StateMachine::translateFaderIdxToFunction(FaderIdx_t faderIdx)
{
	FaderFunction_t faderFunction = NULL_FADER_FUNCTION;

	if(faderIdx == FADER_8)
		faderFunction = MAIN_VOLUME;
	else if(lastSelectionMode == OSCIL_SELECTION_MODE)
	{
		if(faderIdx >= FADER_0 && faderIdx <= FADER_5)
			faderFunction = (FaderFunction_t)(faderIdx + 2); // Ex.:   FADER_0 (Idx=0) -> ENV_DELAY (Idx=2)
	}
	else if(lastSelectionMode == DRUM_SELECTION_MODE)
	{
		if(faderIdx >= FADER_0 && faderIdx <= FADER_2)
			faderFunction = (FaderFunction_t)(faderIdx + 8); // Ex.:	FADER_0 (Idx=0) -> DRUM_FREQ (Idx=8)
	}
	return faderFunction;
}

void StateMachine::InitProcessingComponents()
{
	mainVolume.gain(0.2);

	oscillator[0]->setDAHDSR(0.0, 5.0, 2.1, 31.4, 200, 0.6, 200.0);
	oscillator[0]->setAmplitude(1.0);
	oscillator[0]->setNote(C_2);
	oscillator[0]->setWaveformType(WAVEFORM_SAWTOOTH);

	oscillator[1]->setDAHDSR(0.0, 50.2, 2.1, 31.4, 50, 0.6, 84.5);
	oscillator[1]->setAmplitude(1.0);
	oscillator[1]->setNote(E_2);
	oscillator[1]->setWaveformType(WAVEFORM_SAWTOOTH);

	drum1.frequency(70);
	drum1.length(70);

	drum2.frequency(500);
	drum2.length(30);

	filter1.frequency(400);
}

void StateMachine::InitPanel()
{
	InitProcessingComponents();
	AkaiMidi.drawInitPanel(numOscils, numDrums);
	currentMode = OSCIL_SELECTION_MODE;
}

void StateMachine::refreshSequencer()
{
	uint8_t MIDINote;

	if(lastSelectionMode == OSCIL_SELECTION_MODE)
	{
		for(int step = 0; step < STEP_NUMBER; step++)
		{
			MIDINote = AkaiMidi.getSequencerStepMidiId(step);

			if(oscillator[currentOscil]->sequence.isStep_On(step))
				AkaiMidi.setStepOnOff(MIDINote, true);
			else
				AkaiMidi.setStepOnOff(MIDINote, false);
		}
	}
	else if(lastSelectionMode == DRUM_SELECTION_MODE)
	{
		for(int step = 0; step < STEP_NUMBER; step++)
		{
			MIDINote = AkaiMidi.getSequencerStepMidiId(step);

			if(drum[currentDrum]->sequence.isStep_On(step))
				AkaiMidi.setStepOnOff(MIDINote, true);
			else
				AkaiMidi.setStepOnOff(MIDINote, false);
		}
	}
}

float myMap(float num, float startScale_IN, float endScale_IN, float startScale_OUT, float endScale_OUT){
	// num : (endIN - startIN) = x : (endOUT - startOUT)
	float noOffsetOutput = (num * (endScale_OUT - startScale_OUT)) / (endScale_IN - startScale_IN);

	return (startScale_OUT + noOffsetOutput);
}
