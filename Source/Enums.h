/*
  ==============================================================================

    Enums.h
    Created: 12 Oct 2024 3:21:23pm
    Author:  John

  ==============================================================================
*/

#pragma once

#include <iostream>

enum class NoteEnum {
	A = 0,
	Bb = 1,
	B = 2,
	C = 3,
	Db = 4,
	D = 5,
	Eb = 6,
	E = 7,
	F = 8,
	Gb = 9,
	G = 10,
	Ab = 11
};

enum class StepEnum {
	// Instead of using an optional parameter I figure it may be more versatile to just have a no step option
	NoSteps = 0, 
	HalfStep = 1,
	WholeStep = 2
};

enum class IntervalEnum {
	Unison = 0,
	min2 = 1,
	Maj2 = 2,
	min3 = 3,
	Maj3 = 4,
	P4 = 5,
	Tritone = 6,
	P5 = 7,
	min6 = 8,
	Maj6 = 9,
	min7 = 10,
	Maj7 = 11,
	Octave = 12
};

// These will be used to add to the non-accidental letter 
// (I.e. D (MusicalLetters[3]) + a Third (2) = F (MusicalLetters[5])
enum class GenericIntervalEnum {
	Unison = 0,
	Second = 1,
	Third = 2,
	Fourth = 3,
	Fifth = 4,
	Sixth = 5,
	Seventh = 6
};

enum class ChordEnum {
	// The numeric values represent the number of thirds needed to complete the chord
	// i.e. a Triad = m3 & M3 == 2 thirds, a Seventh = m3, M3 & m3 == 3 thirds, etc.

	// TODO: We may have to end up being more specific on the chord types... Depending on
	// the amount of specificity we want in determining just how random to get, we may wan to
	// actually create structs of each chord build (what kind of thirds are needed) and
	// then create corresponding eumns and a map<Note, array<new_chord_struct> like we
	// do for the MusicalAlphabet... Idk yet

	// Same reasoning for NoChord as NoStep above in Steps
	NoChord = 0,
	Triad = 2,
	Seventh = 3,
	Ninth = 4,
	Eleventh = 5,
	Thirteenth = 6
};
