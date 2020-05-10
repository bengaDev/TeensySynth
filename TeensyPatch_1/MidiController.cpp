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

void MidiController::drawInitPanel(int numOscils)
{
	this->numOscils = numOscils;

	for(int oscilN = 0; oscilN < numOscils; oscilN++)
	{
		if(oscilN == 0)
			usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][oscilN], GREEN, 0);

		usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][oscilN], YELLOW, 0);
	}
	usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][0], GREEN, 0);
	//usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][0], GREEN, 0);
	//usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][0], GREEN, 0);
}

void MidiController::setOscilOnOff(int oscilNum, bool isOn)
{
	if(isOn)
		usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][oscilNum], GREEN, 0);
	else
		usbMIDI.sendNoteOn(MidiLayout.mainPanelID[0][oscilNum], YELLOW, 0);
}

void MidiController::setStepOnOff(uint8_t note, bool isOn)
{
	if(isOn)
		usbMIDI.sendNoteOn(note, GREEN, 0);
	else
		usbMIDI.sendNoteOn(note, OFF, 0);
}

Fader_t MidiController::getFaderType(uint8_t note)
{
	Fader_t faderType = NULL_FADER_FUNCTION;

	if(note == 56)
		faderType = MAIN_VOLUME;

	return faderType;
}

