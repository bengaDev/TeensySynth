/*
 * StateMachine.cpp
 *
 *  Created on: 01 mag 2020
 *      Author: Benjamin
 */

#include "StateMachine.h"

StateMachine::StateMachine(SequenceOsc *oscils, int numOscils) {
	// TODO Auto-generated constructor stub
	this->numOscils = numOscils;

	oscillator = (SequenceOsc**)malloc(numOscils * sizeof(SequenceOsc*));

	for(int i = 0; i < numOscils; i++)
	{
		oscillator[i] = &(oscils[i]);
	}


	currentMode = INIT_MODE;

	currentOscil = 0;
}

StateMachine::~StateMachine() {
	// TODO Auto-generated destructor stub
	free(oscillator);
}

void StateMachine::updateStateMachine()
{
	if(usbMIDI.read())
		processMidiCommand();

	return;
}

void StateMachine::updateStep(int stepNum, bool *isStepOn)
{
	int prevColor = OFF;
	int prevStep = (stepNum == 0 ? (STEP_NUMBER - 1) : (stepNum - 1));

	if(oscillator[currentOscil]->sequence.isStep_On(prevStep))
	{
		prevColor = GREEN;
	}

	if(isStepOn[currentOscil])
		AkaiMidi.blinkStep(stepNum, RED, prevColor);
	else
		AkaiMidi.blinkStep(stepNum, YELLOW, prevColor);
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
					  AkaiMidi.setOscilOnOff(currentOscil, false);
					  currentOscil = AkaiMidi.getOscilIndex(data1);
					  AkaiMidi.setOscilOnOff(currentOscil, true);
					  currentMode = OSCIL_SELECTION_MODE;
					  break;
				  case STEP_SELECT:
				  {
					  int stepNum = AkaiMidi.getSequencerStep(data1);
					  oscillator[currentOscil]->sequence.setSequenceStep(stepNum);
					  Serial.print("STATE_MACHINE:");
					  Serial.println(oscillator[currentOscil]->sequence.currentSequence);
					  if(oscillator[currentOscil]->sequence.isStep_On(stepNum))
						  AkaiMidi.setStepOnOff(data1, true);
					  else
						  AkaiMidi.setStepOnOff(data1, false);

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
	  else if(type == usbMIDI.ControlChange)
	  {
		  Fader_t faderType = AkaiMidi.getFaderType(data1);

		  switch(faderType){
		  	  case MAIN_VOLUME:

		  		  break;
		  	  case NULL_FADER_FUNCTION:
		  		  break;
		  }
	  }

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
}

void StateMachine::InitProcessingComponents()
{

}

void StateMachine::InitPanel()
{
	InitProcessingComponents();
	AkaiMidi.drawInitPanel(numOscils);
	currentMode = OSCIL_SELECTION_MODE;
}
