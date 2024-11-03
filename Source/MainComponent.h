#pragma once

#include <iostream>
#include <string>
#include <array>
#include <map>
#include <tuple> 

#include <JuceHeader.h>
#include <stdlib.h> 
#include <time.h>  

#include "Enums.h"
#include "../Source/Scales.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent  : public juce::AudioAppComponent
{
public:
    struct NoteStruct
    {
        std::string noteString;
        NoteEnum noteEnum;
    };

    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // Custom functions
    std::string deterimineNextRootNote(StepEnum step = StepEnum::NoSteps);
    std::array<std::string, 7> MainComponent::determineChord(ChordEnum chord = ChordEnum::NoChord);
    NoteStruct determineNoteByInterval(IntervalEnum interval, NoteEnum note, std::string noteString);

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    // TODO: This ultimately should not be initialized here but for now it's fine
    const static short NumOfEnharmNoteNames = 2;
    const static short NumForChordsArray = 5;

    NoteEnum CurrentNote = NoteEnum::C;

    // This can't be an array of chars because of the b's and #'s
    std::map<NoteEnum, std::array<std::string, NumOfEnharmNoteNames>> MusicalAlphabet = {
        {NoteEnum::A, {"A"}},
        {NoteEnum::Bb, {"Bb", "A#"}},
        {NoteEnum::B, {"B"}},
        {NoteEnum::C, {"C"}},
        {NoteEnum::Db, {"Db", "C#"}},
        {NoteEnum::D, {"D"}},
        {NoteEnum::Eb, {"Eb", "D#"}},
        {NoteEnum::E, {"E"}},
        {NoteEnum::F, {"F"}},
        {NoteEnum::Gb, {"Gb", "F#"}},
        {NoteEnum::G, {"G"}},
        {NoteEnum::Ab, {"Ab", "G#"}}
    };

    std::array<ChordEnum, NumForChordsArray> ChordOptions = {
        {ChordEnum::Triad,
        ChordEnum::Seventh,
        ChordEnum::Ninth,
        ChordEnum::Eleventh,
        ChordEnum::Thirteenth}
    };
    
    // This is confusing but I wanted to have a list of all the enharmonic spellings of a note
    // while also having one definitive enum for each pitch, ergo the weird matchings.
    struct NoteStruct adoubleflat { "Abb", NoteEnum::G };
    struct NoteStruct aflat { "Ab", NoteEnum::Ab };
    struct NoteStruct a { "A", NoteEnum::A };
    struct NoteStruct asharp { "A#", NoteEnum::Bb };
    struct NoteStruct adoublesharp { "Ax", NoteEnum::B };
    struct NoteStruct bdoubleflat { "Bbb", NoteEnum::A };
    struct NoteStruct bflat { "Bb", NoteEnum::Bb };
    struct NoteStruct b { "B", NoteEnum::B };
    struct NoteStruct bsharp { "B#", NoteEnum::C };
    struct NoteStruct bdoublesharp { "Bx", NoteEnum::Db };
    struct NoteStruct cdoubleflat { "Cbb", NoteEnum::Bb };
    struct NoteStruct cflat { "Cb", NoteEnum::B };
    struct NoteStruct c { "C", NoteEnum::C };
    struct NoteStruct csharp { "C#", NoteEnum::Db };
    struct NoteStruct cdoublesharp { "Cx", NoteEnum::D };
    struct NoteStruct ddoubleflat { "Dbb", NoteEnum::C };
    struct NoteStruct dflat { "Db", NoteEnum::Db };
    struct NoteStruct d { "D", NoteEnum::D };
    struct NoteStruct dsharp { "D#", NoteEnum::Eb };
    struct NoteStruct ddoublesharp { "Dx", NoteEnum::E };
    struct NoteStruct edoubleflat { "Ebb", NoteEnum::D };
    struct NoteStruct eflat { "Eb", NoteEnum::Eb };
    struct NoteStruct e { "E", NoteEnum::E };
    struct NoteStruct esharp { "E#", NoteEnum::F };
    struct NoteStruct edoublesharp { "Ex", NoteEnum::Gb };
    struct NoteStruct fdoubleflat { "Fbb", NoteEnum::Eb };
    struct NoteStruct fflat { "Fb", NoteEnum::E };
    struct NoteStruct f { "F", NoteEnum::F };
    struct NoteStruct fsharp { "F#", NoteEnum::Gb };
    struct NoteStruct fdoublesharp { "Fx", NoteEnum::G };
    struct NoteStruct gdoubleflat { "Gb", NoteEnum::Gb };
    struct NoteStruct gflat { "Gbb", NoteEnum::F };
    struct NoteStruct g { "G", NoteEnum::G };
    struct NoteStruct gsharp { "G#", NoteEnum::Ab };
    struct NoteStruct gdoublesharp { "Gx", NoteEnum::A };

    // TODO: Update the comment below
    // These can be thought of as absolute note groupings, as in, they are all of a specific Note enum. They are ordered from most commonly 
    // seen to least commonly seen and that may be able to be used in determining the level of randomness... this remains to be seen
    std::array<NoteStruct, 3> noteGroup0 = { a, bdoubleflat, gdoublesharp };
    std::array<NoteStruct, 3> noteGroup1 = { bflat, asharp, cdoubleflat };
    std::array<NoteStruct, 3> noteGroup2 = { b, cflat, adoublesharp };
    std::array<NoteStruct, 3> noteGroup3 = { c, bsharp, ddoubleflat };
    std::array<NoteStruct, 3> noteGroup4 = { csharp, dflat, csharp };
    std::array<NoteStruct, 3> noteGroup5 = { d, edoubleflat };
    std::array<NoteStruct, 3> noteGroup6 = { eflat, dsharp, fdoubleflat };
    std::array<NoteStruct, 3> noteGroup7 = { e, fflat, ddoublesharp };
    std::array<NoteStruct, 3> noteGroup8 = { f, esharp, gdoubleflat };
    std::array<NoteStruct, 3> noteGroup9 = { fsharp, gflat, edoublesharp };
    std::array<NoteStruct, 3> noteGroup10 = { g, fdoublesharp, adoubleflat };
    std::array<NoteStruct, 3> noteGroup11 = { gsharp, aflat };

    const std::map<short, std::array<NoteStruct, 3>> noteGroupings = {
        {0, noteGroup0}, {1, noteGroup1}, {2, noteGroup2}, {3, noteGroup3}, {4, noteGroup4}, {5, noteGroup5}, {6, noteGroup6}, 
        {7, noteGroup7}, {8, noteGroup8}, {9, noteGroup9}, {10, noteGroup10}, {11, noteGroup11}
    };

    std::array<char, 7> MusicalLetters = { 'A', 'B', 'C', 'D', 'E', 'F', 'G'};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};


//std::array<std::map<std::string, Note>, 7> AflatScale = { 
    //    aflat, bflat, c, dflat, eflat, f, g
    //};
    //std::array<std::map<std::string, Note>, 7> AScale = {
    //    a, b, csharp, d, e, fsharp, gsharp
    //};
    //std::array<std::map<std::string, Note>, 7> AsharpScale = {
    //    asharp, bsharp, cdoublesharp, dsharp, esharp, fdoublesharp, gdoublesharp
    //};
    //std::array<std::map<std::string, Note>, 7> BflatScale = {
    //    bflat, c, d, eflat, f, g, a
    //};
    //std::array<std::map<std::string, Note>, 7> BScale = {
    //    b, csharp, dsharp, e, fsharp, gsharp, asharp
    //};
    //std::array<std::map<std::string, Note>, 7> BsharpScale = {
    //    bsharp, cdoublesharp, ddoublesharp, esharp, fdoublesharp, gdoublesharp, adoublesharp
    //};
    //std::array<std::map<std::string, Note>, 7> CflatScale = {
    //    cflat, dflat, eflat, fflat, gflat, aflat, bflat
    //};
    //std::array<std::map<std::string, Note>, 7> CScale = {
    //    c, d, e, f, g, a, b
    //};
    //std::array<std::map<std::string, Note>, 7> CsharpScale = {
    //    csharp, dsharp, esharp, fsharp, gsharp, asharp, bsharp
    //};
    //std::array<std::map<std::string, Note>, 7> DflatScale = {
    //    dflat, eflat, f, gflat, aflat, bflat, c
    //};
    //std::array<std::map<std::string, Note>, 7> DScale = {
    //    d, e, fsharp, g, a, b, csharp
    //};
    //std::array<std::map<std::string, Note>, 7> DsharpScale = {
    //    dsharp, esharp, fdoublesharp, gsharp, asharp, bsharp, cdoublesharp
    //};
    //std::array<std::map<std::string, Note>, 7> EflatScale = {
    //    eflat, f, g, aflat, bflat, c, d
    //};
    //std::array<std::map<std::string, Note>, 7> EScale = {
    //    e, fsharp, gsharp, a, b, csharp, dsharp
    //};
    //std::array<std::map<std::string, Note>, 7> EsharpScale = {
    //    esharp, fdoublesharp, gdoublesharp, asharp, bsharp, cdoublesharp, ddoublesharp
    //};
    //std::array<std::map<std::string, Note>, 7> FflatScale = {
    //    fflat, gflat, aflat, bdoubleflat, cflat, dflat, eflat
    //};
    //std::array<std::map<std::string, Note>, 7> FScale = {
    //    f, g, a, bflat, c, d, e
    //};
    //std::array<std::map<std::string, Note>, 7> FsharpScale = {
    //    fsharp, gsharp, asharp, b, csharp, dsharp, esharp
    //}; 
    //std::array<std::map<std::string, Note>, 7> GflatScale = {
    //    gflat, aflat, bflat, cflat, dflat, eflat, f
    //};
    //std::array<std::map<std::string, Note>, 7> GScale = {
    //    g, a, b, c, d, e, fsharp
    //};
    //std::array<std::map<std::string, Note>, 7> GsharpScale = {
    //    gsharp, asharp, bsharp, csharp, dsharp, esharp, fdoublesharp
    //};

    //Scale Ab = Scale("Ab", Note::Ab, AflatScale, &GSharp);
    //Scale A = Scale("A", Note::A, AScale, nullptr);
    //Scale ASharp = Scale("A#", Note::Bb, AsharpScale, &Bb);
    //Scale Bb = Scale("Bb", Note::Bb, BflatScale, &ASharp);
    //Scale B = Scale("B", Note::B, BScale, &Cb);
    //Scale BSharp = Scale("B#", Note::C, BsharpScale, &C);
    //Scale Cb = Scale("Cb", Note::B, CflatScale, &B);
    //Scale C = Scale("C", Note::C, CScale, &BSharp);
    //Scale CSharp = Scale("C#", Note::Db, CsharpScale, &Db);
    //Scale Db = Scale("Db", Note::Db, DflatScale, &CSharp);
    //Scale D = Scale("D", Note::D, DScale, nullptr);
    //Scale DSharp = Scale("D#", Note::Eb, DsharpScale, &Eb);
    //Scale Eb = Scale("Eb", Note::Eb, EflatScale, &DSharp);
    //Scale E = Scale("E", Note::E, EScale, &Fb);
    //Scale ESharp = Scale("E#", Note::F, EsharpScale, &F);
    //Scale Fb = Scale("Fb", Note::E, FflatScale, &E);
    //Scale F = Scale("F", Note::F, FScale, &ESharp);
    //Scale FSharp = Scale("F#", Note::Gb, FsharpScale, &Gb);
    //Scale Gb = Scale("Gb", Note::Gb, GflatScale, &FSharp);
    //Scale G = Scale("G", Note::G, GScale, nullptr);
    //Scale GSharp = Scale("G#", Note::Ab, GsharpScale, &Ab);

    //std::array<Scale, 21> Scales = {
    //    Ab, A, ASharp, Bb, B, BSharp, Cb, C, CSharp, Db, D, DSharp, Eb, E, ESharp, Fb, F, FSharp, Gb, G, GSharp
    //};


// Thoughs on the project log:
    // 
    // *I keep finding myself not wanting to delete these notes in case I want to come back to them and experiment so I'll just keep them,
    // with a new paragraph reperesenting the next time I worked on this and the most recent paragraph being at the bottom.*
    // 
    // LAST LEFT OFF: Another way I can conceive of this (for determining correct chord tones) is by having two lists, one of just the
    // musical letters and another of all 12 tones. If we want to go up 4 half-steps from C we get E which will be fine, but if we go up
    // 4 half-steps from C# and get F, we can compare that note to the musical alphabet, see if the note is not exactly a (generic) 3rd
    // away from the starting note, and based on whether it was greater or less than a 3rd will dertermine whether we take the note before
    // or after the determined note (the F in this case) and then add the appropriate # or b. Then that string value could be used as a key 
    // to a dictionary to get the correct enum. The only thing that might be tricky is how to determine bb's and x's. In the case of Eb dim, 
    // we need the notes Eb-Gb-Bbb so how would we determine that? Perhaps we could check the length of the 12-tone list and see if it's only
    // 1 (meaning it's plain A, B, C, etc. not C#, Ab, and so on) and if that's the case we can assume it needs to be a double accidental. 
    // But I would need to do a lot of testing on that to make sure that works. Also on top of that, I can see this going awry pretty quickly
    // if this isn't implemented carefully and then we end up in a place where we're getting quadruple #'s or now everything becomes a bb or x.
    // 
    // UPDATE TO ABOVE: I actually think that maybe using a combined approach of the above and the noteGroups would be best. Meaning, the 
    // 2nd 12-tone list should just be the noteGroupings. In whatever function is created to determine the enharmonic spellings, take the logic
    // above, but then determine the correct string spelling of the note (meaning which letter and accidentals) and then determine the absolute
    // note number (0 = A, Bbb, Gx / 1 = A#, Bb, Cbb / etc.) and then use the string value we just determined as the key to find the correct
    // Note enum and pitch.
    //
    // I did a lot of clean up and finally got a function to determine the correct note spelling using the approach above though slightly modified.
    // Basically, since no letter would ever appear twice in a grouping, I just checked the first letter of the NoteString. However, one major-ish
    // thing I changed was that I made a struct of note enum and note string and replaced the mess of maps and arrays I was using previously for
    // the noteGroupings. This seems to be the way to go overall, but if that is the case then I'll need to reconfigure how I'm using the global
    // CurrentNote because that should probably be one of those structs from the get go. It'll keep everything easier to keep track of since the
    // CurrentNote only keeps track of the absolute pitch, the string name can vary widely and I'm relying on the CurrentNote to determine the next.
