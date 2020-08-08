#include "Arduino.h"
//#include "MIDI.h"

#include <Audio.h>
#include <Wire.h>
//#define MIDI_INTERFACE
//#include <usb_midi.h>
#include <synth_simple_drum.h>

#include "SequenceOsc.h"
#include "MidiController.h"
#include "StateMachine.h"

//usb_midi_class usbMIDI;

//MIDI_CREATE_DEFAULT_INSTANCE();
/*
#define NUM_WAVES	3
#define VOLUME		0.1
#define BPM			120.0
#define SUSTAIN 	100 //ms
#define BPM_DIV		4
#define NUM_STEPS	16
#define NUM_OSCILS	2

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

StateMachine benStateMachine(oscil, NUM_OSCILS);

AudioFilterStateVariable filter1;
AudioAmplifier mainVolume;

AudioConnection patchCord0(oscil[0].envelope, 0, mixer1, 0);
AudioConnection patchCord1(oscil[1].envelope, 0, mixer1, 1);
AudioConnection patchCord2(drum1, 0, mixer2, 0);
AudioConnection patchCord6(drum2, 0, mixer2, 1);
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
*/

StateMachine benStateMachine;

bool isSustainDone();
bool isBPM();

int colorMatrix[8][8] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 2, 0, 0, 2, 0, 0,
		0, 2, 2, 2, 2, 2, 2, 0,
		0, 2, 2, 2, 2, 2, 2, 0,
		0, 0, 2, 2, 2, 2, 0, 0,
		0, 0, 0, 2, 2, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
};

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);

	//AkaiMidi.drawMainPanel(colorMatrix);
// Add your initialization code here
	AudioMemory(18);
	/*
	mainVolume.gain(0.1);

	oscil[0].setDAHDSR(0.0, 5.0, 2.1, 31.4, 200, 0.6, 200.0);
	oscil[0].setAmplitude(1.0);
	oscil[0].setNote(C_1);
	oscil[0].setWaveformType(WAVEFORM_SAWTOOTH);

	oscil[1].setDAHDSR(0.0, 50.2, 2.1, 31.4, 50, 0.6, 84.5);
	oscil[1].setAmplitude(1.0);
	oscil[1].setNote(E_1);
	oscil[1].setWaveformType(WAVEFORM_SAWTOOTH);

	drum1.frequency(70);
	drum1.length(70);

	drum2.frequency(500);
	drum2.length(30);

	filter1.frequency(400);
	*/
	// Initialize processor and memory measurements


	AudioProcessorUsageMaxReset();
	AudioMemoryUsageMaxReset();
}

int currentNote = 0;
bool even = false;
int step = 0;

// The loop function is called in an endless loop
void loop()
{
	/*
	if(isClkTick())
	{
		manageStep(step);
		step = (step + 1) % STEP_NUMBER;
	}

	knob_A5 = (float)analogRead(A5) / 1023.0;
	knob_A4 = (float)analogRead(A4) / 1023.0;

	cutoffFreq = knob_A4;

	//Serial.println(knob_A3);
	cutoffFreq = (cutoffFreq*cutoffFreq);
	cutoffFreq = map(cutoffFreq, 0.0, 1.0, 20.0, 10000.0);

	filter1.frequency(cutoffFreq);
	*/

	/*
	bool noteChange = false;

	for(int i = 0; i < NUM_OSCILS; i++)
		oscil[i].UpdateState();
	*/

	// Receive MIDI data
	//manageMIDIMessage();

	benStateMachine.updateStateMachine();
}

/*
void manageStep(int stepNumber)
{
	bool isStepOn[NUM_OSCILS] = {false};

	for(int oscilN = 0; oscilN < NUM_OSCILS; oscilN++)
	{
		if(oscil[oscilN].sequence.isStep_On(stepNumber))
		{
			oscil[oscilN].notePlay();
			isStepOn[oscilN] = true;
		}
	}

	benStateMachine.updateStep(stepNumber, isStepOn);

	//AkaiMidi.blinkStep(stepNumber, color);
	return;
}
*/

void manageMIDIMessage()
{
	//if(usbMIDI.read())
		//processMIDICommand();
	return;
}


/*
float map(float num, float startScale_IN, float endScale_IN, float startScale_OUT, float endScale_OUT){
	// num : (endIN - startIN) = x : (endOUT - startOUT)
	float noOffsetOutput = (num * (endScale_OUT - startScale_OUT)) / (endScale_IN - startScale_IN);

	return startScale_OUT + noOffsetOutput;
}
*/

/*
bool isBPM()
{
	unsigned long diff = millis();
	if( (diff - last_time) >= (unsigned long)( (1.0 / (BPM / 60.0)) * 1000.0) )
	{
		//Serial.print(diff);
		//Serial.println("");
		last_time = millis();
		return true;
	}

	return false;
}
*/

/*
bool isClkTick()
{
	if( (millis() - last_time) >= (unsigned long)(BPM_ClkPeriod))
	{
		last_time = millis();
		return true;
	}
	return false;
}
*/
/*
bool isHalfBPM()
{
	if( (millis() - last_time_half) >= (int)(( (1.0 / (BPM / 60.0)) * 1000.0) / 2) )
	{
		last_time_half = millis();
		return true;
	}

	return false;
}
*/
