/*
 * MidiController.h
 *
 *  Created on: 25 apr 2020
 *      Author: Benjamin
 */

#ifndef MIDICONTROLLER_H_
#define MIDICONTROLLER_H_

#include <usb_midi.h>
#include "Sequence.h"

enum {OFF = 0x00, GREEN = 0x01, RED = 0x03, YELLOW = 0x05};
typedef enum {NOTHING = 0, OSCIL_SELECT = 1, STEP_SELECT = 2} Selection_t; // Selection Type
typedef enum {NULL_FADER_FUNCTION, MAIN_VOLUME} Fader_t;

class MidiController {
private:
	int sequencerRow;

	typedef struct MIDILayout {
		uint8_t mainPanelID[8][8];
		uint8_t verticalButtonsID[8];
		uint8_t horizontalButtonsID[8];
		uint8_t fadersID[9];
		uint8_t shift;
	} MIDILayout_t;

	MIDILayout_t MidiLayout;

	int numOscils;
public:
	MidiController();
	virtual ~MidiController();

	void drawMainPanel(int mainPanelColor[][8]);
	void blinkStep(int stepNum, int color, int prevColor);
	Selection_t getSelectionType(uint8_t note);
	int getOscilIndex(uint8_t note);
	int getSequencerStep(uint8_t note);
	void drawInitPanel(int numOscils);
	void setOscilOnOff(int oscilNum, bool isOn);
	void setStepOnOff(uint8_t note, bool isOn);
	Fader_t getFaderType(uint8_t note);
};

#endif /* MIDICONTROLLER_H_ */
