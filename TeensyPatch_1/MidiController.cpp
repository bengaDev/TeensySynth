/*
 * MidiController.cpp
 *
 *  Created on: 25 apr 2020
 *      Author: Benjamin
 */

#include "MidiController.h"
#include <Audio.h>

MidiController::MidiController() {
	// TODO Auto-generated constructor stub
	MidiLayout = {
			{		56, 57, 58, 59, 60, 61, 62, 63,
					48, 49, 50, 51, 52, 53, 54, 55,
					40, 41, 42, 43, 44, 45, 46, 47,
					32, 33, 34, 35, 36, 37, 38, 39,
					24, 25, 26, 27, 28, 29, 30, 31,
					16, 17, 18, 19, 20, 21, 22, 23,
					8, 	9, 	10, 11, 12, 13, 14, 15,
					0, 	1, 	2, 	3, 	4, 	5, 	6, 	7 ,
			},
			{		82, 83, 84, 85, 86, 87, 88, 89, },
			{		64, 65, 66, 67, 68, 69, 70, 71, },
			{		48, 49, 50, 51, 52, 53, 54, 55, 56, },
			98,
	};

	sequencerRow = 4;

	numOscils = -1;
	numDrums = -1;
}

MidiController::~MidiController() {
	// TODO Auto-generated destructor stub
}

void MidiController::drawMainPanel(int mainPanelColor[][8])
{
	for(int row = 0; row < 8; row++)
	{
		for(int column = 0; column < 8; column++)
		{
			switch(mainPanelColor[row][column]){
			case OFF:
				usbMIDI.sendNoteOn(MidiLayout.mainPanelID[row][column], OFF, 0);
				break;
			case GREEN:
				usbMIDI.sendNoteOn(MidiLayout.mainPanelID[row][column], GREEN, 0);
				break;
			case RED:
				usbMIDI.sendNoteOn(MidiLayout.mainPanelID[row][column], RED, 0);
				break;
			case YELLOW:
				usbMIDI.sendNoteOn(MidiLayout.mainPanelID[row][column], YELLOW, 0);
				break;
			}
		}
	}
}

void MidiController::blinkStep(int stepNum, int color, int prevColor)
{
	int prevStep = (stepNum == 0 ? (STEP_NUMBER - 1) : (stepNum - 1));

	usbMIDI.sendNoteOn(MidiLayout.mainPanelID[sequencerRow + (prevStep / 8)][prevStep % 8], prevColor, 0);
	usbMIDI.sendNoteOn(MidiLayout.mainPanelID[sequencerRow + (stepNum / 8)][stepNum % 8], color, 0);
}

Selection_t MidiController::getSelectionType(uint8_t note)
{
	Selection_t selType = NOTHING;

	if(note >= MidiLayout.mainPanelID[0][0] && note <= MidiLayout.mainPanelID[0][numOscils - 1])
		selType = OSCIL_SELECT;
	else if(note >= MidiLayout.mainPanelID[0][7 - numDrums + 1] && note <= MidiLayout.mainPanelID[0][7])
		selType = DRUM_SELECT;
	else if(note >= MidiLayout.mainPanelID[0][7 - 2 - numKicks + 1] && note <= MidiLayout.mainPanelID[0][7 - 2])
		selType = KICK_SELECT;
	else if(note >= MidiLayout.mainPanelID[sequencerRow + (STEP_NUMBER / 8) - 1][0] && note <= MidiLayout.mainPanelID[sequencerRow][7])
		selType = STEP_SELECT;
/*
	Serial.print("Note:");
	Serial.println(note, DEC);
	Serial.print("MIDI LAyout:");
	Serial.print(MidiLayout.mainPanelID[sequencerRow][0], DEC);
	if(selType == NOTHING)
		Serial.println("getSelectionType: NOTHING");
	else if(selType == OSCIL_SELECT)
		Serial.println("getSelectionType: OSCIL_SELECT");
	else if(selType == STEP_SELECT)
		Serial.println("getSelectionType: STEP_SELECT");
	*/

	return selType;
}

int MidiController::getOscilIndex(uint8_t note)
{
	int oscilIndex = -1;

	for(int oscilN = 0; oscilN < numOscils; oscilN++)
	{
		if(note == MidiLayout.mainPanelID[0][oscilN])
			oscilIndex = oscilN;
	}

	return oscilIndex;
}

int MidiController::getDrumIndex(uint8_t note)
{
	int drumIndex = -1;

	for(int drumN = 0; drumN < numDrums; drumN++)
	{
		if(note == MidiLayout.mainPanelID[0][7 - numDrums + drumN + 1])
			drumIndex = drumN;
	}

	return drumIndex;
}

int MidiController::getKickIndex(uint8_t note)
{
	int kickIndex = -1;

	for(int kickN = 0; kickN < numKicks; kickN++)
	{
		if(note == MidiLayout.mainPanelID[0][7 - 2 - numKicks + kickN + 1])
			kickIndex = kickN;
	}

	return kickIndex;
}

int MidiController::getSequencerStep(uint8_t note)
{
	int stepNum = -1;

	if(note >= 24 && note <= 31)
		stepNum = note - 24;
	else if(note >= 16 && note <= 23)
		stepNum = note - 8;
	else if (note >= 8 && note <= 15)
		stepNum = note + 8;
	else if (note >= 0 && note <= 7)
		stepNum = note + 24;

	return (stepNum % STEP_NUMBER);
}

uint8_t MidiController::getSequencerStepMidiId(int stepNumber)
{
	uint8_t stepID;

	if(stepNumber >= 24 && stepNumber <= 31)
		stepID = stepNumber - 24;
	else if(stepNumber >= 16 && stepNumber <= 23)
		stepID = stepNumber - 8;
	else if(stepNumber >= 8 && stepNumber <= 15)
		stepID = stepNumber + 8;
	else if(stepNumber >= 0 && stepNumber <= 7)
		stepID = stepNumber + 24;

	return stepID;
}

void MidiController::drawInitPanel(int numOscils, int numDrums, int numKicks)
{
	this->numOscils = numOscils;
	this->numDrums = numDrums;
	this->numKicks = numKicks;

	for(int oscilN = 0; oscilN < numOscils; oscilN++)
		usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][oscilN], YELLOW, 0);

	for(int drumN = 0; drumN < numDrums; drumN++)
		usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][7 - numDrums + drumN + 1], YELLOW, 0);

	for(int kickN = 0; kickN < numKicks; kickN++)
		usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][7 - 2 - numKicks + kickN + 1], YELLOW, 0);

	usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][0], GREEN, 0);  // OSCIL 1

	usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][7 - numDrums + 1], GREEN, 0);  // DRUM 1
}

void MidiController::setOscilOnOff(int oscilNum, bool isOn)
{
	if(isOn)
		usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][oscilNum], GREEN, 0);
	else
		usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][oscilNum], YELLOW, 0);
}

void MidiController::setDrumOnOff(int drumNum, bool isOn)
{
	if(isOn)
		usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][7 - numDrums + drumNum + 1], GREEN, 0);
	else
		usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][7 - numDrums + drumNum + 1], YELLOW, 0);
}

void MidiController::setKickOnOff(int kickNum, bool isOn)
{
	if(isOn)
		usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][7 - 2 - numKicks + kickNum + 1], GREEN, 0);
	else
		usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][7 - 2 - numKicks + kickNum + 1], YELLOW, 0);
}

void MidiController::setStepOnOff(uint8_t note, bool isOn)
{
	if(isOn)
		usbMIDI.sendNoteOn(note, GREEN, 0);
	else
		usbMIDI.sendNoteOn(note, OFF, 0);
}

FaderIdx_t MidiController::getFaderType(uint8_t note)
{
	FaderIdx_t faderIdx;

	if(note >= 48 && note <= 56)
		faderIdx = (FaderIdx_t)( (note - 48) & 0xFF); // From (FADER_0 = 0) to (FADER_8 = 8)

	return faderIdx;
}

