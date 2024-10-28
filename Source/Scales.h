/*
  ==============================================================================

    Scales.h
    Created: 26 Oct 2024 1:42:41pm
    Author:  John

  ==============================================================================
*/

#pragma once

#include <iostream>
#include "Enums.h"
#include <array>
#include <map>
#include <optional>

using namespace std;

class Scale {
    public:
        string RootString;
        Note RootEnum;
        array<map<string, Note>, 7> ScaleNotes;
        Scale* EnharmonicWith; // This will etiher be 'n' for next scale, 'p' for previous or 'x' for none

        Scale(string rootString, Note rootEnum, array<map<string, Note>, 7> scaleNotes, Scale* enharmonicWith)
        {
            RootString = rootString;
            RootEnum = rootEnum;
            ScaleNotes = scaleNotes;
            EnharmonicWith = enharmonicWith;
        }
};
