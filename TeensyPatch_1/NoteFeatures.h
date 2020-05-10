/*
 * NoteFeatures.h
 *
 *  Created on: 19 apr 2020
 *      Author: Benjamin
 */

#ifndef NOTEFEATURES_H_
#define NOTEFEATURES_H_


typedef enum
{
	C_m1, Cd_m1, D_m1, Dd_m1, E_m1, F_m1, Fd_m1, G_m1, Gd_m1, A_m1, Ad_m1, B_m1,
	C_0, Cd_0, D_0, Dd_0, E_0, F_0, Fd_0, G_0, Gd_0, A_0, Ad_0, B_0,
	C_1, Cd_1, D_1, Dd_1, E_1, F_1, Fd_1, G_1, Gd_1, A_1, Ad_1, B_1,
	C_2, Cd_2, D_2, Dd_2, E_2, F_2, Fd_2, G_2, Gd_2, A_2, Ad_2, B_2,
	C_3, Cd_3, D_3, Dd_3, E_3, F_3, Fd_3, G_3, Gd_3, A_3, Ad_3, B_3,
	C_4, Cd_4, D_4, Dd_4, E_4, F_4, Fd_4, G_4, Gd_4, A_4, Ad_4, B_4,
	C_5, Cd_5, D_5, Dd_5, E_5, F_5, Fd_5, G_5, Gd_5, A_5, Ad_5, B_5,
	C_6, Cd_6, D_6, Dd_6, E_6, F_6, Fd_6, G_6, Gd_6, A_6, Ad_6, B_6,
	C_7, Cd_7, D_7, Dd_7, E_7, F_7, Fd_7, G_7, Gd_7, A_7, Ad_7, B_7,
	C_8, Cd_8, D_8, Dd_8, E_8, F_8, Fd_8, G_8, Gd_8, A_8, Ad_8, B_8,
	C_9, Cd_9, D_9, Dd_9, E_9, F_9, Fd_9, G_9,
	NUM_NOTES,
} Note_t;

// Table of midi note frequencies * 2
//   They are times 2 for greater accuracy, yet still fits in a word.
//   Generated from Excel by =ROUND(2*440/32*(2^((x-9)/12)),0) for 0<x<128
// The lowest notes might not work, depending on the Arduino clock frequency

// This is for the Teensy Audio library which specifies
// frequencies as floating point. See make_notetab.xlsx
const float Note[NUM_NOTES] =
{
    8.1758,    8.6620,    9.1770,    9.7227,    10.3009,    10.9134,    11.5623,    12.2499,
    12.9783,   13.7500,   14.5676,   15.4339,   16.3516,    17.3239,    18.3540,    19.4454,
    20.6017,   21.8268,   23.1247,   24.4997,   25.9565,    27.5000,    29.1352,    30.8677,
    32.7032,   34.6478,   36.7081,   38.8909,   41.2034,    43.6535,    46.2493,    48.9994,
    51.9131,   55.0000,   58.2705,   61.7354,   65.4064,    69.2957,    73.4162,    77.7817,
    82.4069,   87.3071,   92.4986,   97.9989,   103.8262,   110.0000,   116.5409,   123.4708,
    130.8128,  138.5913,  146.8324,  155.5635,  164.8138,   174.6141,   184.9972,   195.9977,
    207.6523,  220.0000,  233.0819,  246.9417,  261.6256,   277.1826,   293.6648,   311.1270,
    329.6276,  349.2282,  369.9944,  391.9954,  415.3047,   440.0000,   466.1638,   493.8833,
    523.2511,  554.3653,  587.3295,  622.2540,  659.2551,   698.4565,   739.9888,   783.9909,
    830.6094,  880.0000,  932.3275,  987.7666,  1046.5023,  1108.7305,  1174.6591,  1244.5079,
    1318.5102, 1396.9129, 1479.9777, 1567.9817, 1661.2188,  1760.0000,  1864.6550,  1975.5332,
    2093.0045, 2217.4610, 2349.3181, 2489.0159, 2637.0205,  2793.8259,  2959.9554,  3135.9635,
    3322.4376, 3520.0000, 3729.3101, 3951.0664, 4186.0090,  4434.9221,  4698.6363,  4978.0317,
    5274.0409, 5587.6517, 5919.9108, 6271.9270, 6644.8752,  7040.0000,  7458.6202,  7902.1328,
    8372.0181, 8869.8442, 9397.2726, 9956.0635, 10548.0818, 11175.3034, 11839.8215, 12543.8540
};

const float velocity2amplitude[127] = {
0.01778,0.01966,0.02164,0.02371,0.02588,0.02814,0.03049,0.03294,0.03549,0.03812,
0.04086,0.04369,0.04661,0.04963,0.05274,0.05594,0.05924,0.06264,0.06613,0.06972,
0.07340,0.07717,0.08104,0.08500,0.08906,0.09321,0.09746,0.10180,0.10624,0.11077,
0.11539,0.12011,0.12493,0.12984,0.13484,0.13994,0.14513,0.15042,0.15581,0.16128,
0.16685,0.17252,0.17828,0.18414,0.19009,0.19613,0.20227,0.20851,0.21484,0.22126,
0.22778,0.23439,0.24110,0.24790,0.25480,0.26179,0.26887,0.27605,0.28333,0.29070,
0.29816,0.30572,0.31337,0.32112,0.32896,0.33690,0.34493,0.35306,0.36128,0.36960,
0.37801,0.38651,0.39511,0.40381,0.41260,0.42148,0.43046,0.43953,0.44870,0.45796,
0.46732,0.47677,0.48631,0.49595,0.50569,0.51552,0.52544,0.53546,0.54557,0.55578,
0.56609,0.57648,0.58697,0.59756,0.60824,0.61902,0.62989,0.64085,0.65191,0.66307,
0.67432,0.68566,0.69710,0.70863,0.72026,0.73198,0.74380,0.75571,0.76771,0.77981,
0.79201,0.80430,0.81668,0.82916,0.84174,0.85440,0.86717,0.88003,0.89298,0.90602,
0.91917,0.93240,0.94573,0.95916,0.97268,0.98629,1.00000
};
/*
#! /usr/bin/perl
# The Interpretation of MIDI Velocity
# Roger B. Dannenberg
# School of Computer Science, Carnegie Mellon University
$dynamic_range = 35;
$r = 10 ** ($dynamic_range / 20);
$b = 127 / (126 * sqrt($r)) - 1/126;
$m = (1 - $b) / 127;
print "const float velocity2amplitude[127] = {\n";
for ($v=1; $v <= 127; $v++) {
	$a = ($m * $v + $b) ** 2;
	printf "%.5f", $a;
	print "," if $v < 127;
	print "\n" if ($v % 10) == 0;
}
print "\n};\n";
*/


#endif /* NOTEFEATURES_H_ */
