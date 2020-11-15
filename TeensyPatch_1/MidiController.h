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
typedef enum {NOTHING = 0, OSCIL_SELECT = 1, DRUM_SELECT = 2, KICK_SELECT = 3, STEP_SELECT = 4} Selection_t; // Selection Type
/*typedef enum {NULL_FADER_FUNCTION, MAIN_VOLUME,
	ENV_DELAY,	ENV_ATTACK,		ENV_HOLD,
	ENV_DECAY, 	ENV_SUSTAIN,	ENV_RELEASE} Fader_t;
	*/
typedef enum {
	FADER_0,	FADER_1,	FADER_2,
	FADER_3, 	FADER_4,	FADER_5,
	FADER_6, 	FADER_7, 	FADER_8} FaderIdx_t;

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
	int numDrums;
	int numKicks;
public:
	MidiController();
	virtual ~MidiController();

	void drawMainPanel(int mainPanelColor[][8]);
	void blinkStep(int stepNum, int color, int prevColor);
	Selection_t getSelectionType(uint8_t note);
	int getOscilIndex(uint8_t note);
	int getDrumIndex(uint8_t note);
	int getKickIndex(uint8_t note);
	int getSequencerStep(uint8_t note);
	uint8_t getSequencerStepMidiId(int stepNumber);
	void drawInitPanel(int numOscils, int numDrums, int numKicks);
	void setOscilOnOff(int oscilNum, bool isOn);
	void setDrumOnOff(int drumNum, bool isOn);
	void setKickOnOff(int kickNum, bool isOn);
	void setStepOnOff(uint8_t note, bool isOn);
	FaderIdx_t getFaderType(uint8_t note);
};

#endif /* MIDICONTROLLER_H_ */
