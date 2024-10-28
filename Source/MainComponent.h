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
    //==============================================================================
    MainComponent();
    ~MainComponent() override;

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // Custom functions
    std::string deterimineNextRootNote(Step step = Step::NoSteps);
    std::array<std::string, 7> MainComponent::determineChord(Chord chord = Chord::NoChord);
    std::string isCorrectNote(Interval interval);

    //==============================================================================
    void paint (juce::Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    // Your private member variables go here...

    // TODO: This ultimately should not be initialized here but for now it's fine
    const static short NumOfEnharmNoteNames = 2;
    const static short NumForChordsArray = 5;

    Note CurrentNote = Note::C;

    // This can't be an array of chars because of the b's and #'s
    std::map<Note, std::array<std::string, NumOfEnharmNoteNames>> MusicalAlphabet = {
        {Note::A, {"A"}},
        {Note::Bb, {"Bb", "A#"}},
        {Note::B, {"B"}},
        {Note::C, {"C"}},
        {Note::Db, {"Db", "C#"}},
        {Note::D, {"D"}},
        {Note::Eb, {"Eb", "D#"}},
        {Note::E, {"E"}},
        {Note::F, {"F"}},
        {Note::Gb, {"Gb", "F#"}},
        {Note::G, {"G"}},
        {Note::Ab, {"Ab", "G#"}}
    };

    std::array<Chord, NumForChordsArray> ChordOptions = {
        {Chord::Triad,
        Chord::Seventh,
        Chord::Ninth,
        Chord::Eleventh,
        Chord::Thirteenth}
    };
    
    // This is confusing but I wanted to have a list of all the enharmonic spellings of a note
    // while also having one definitive enum for each pitch, ergo the weird matchings.
    std::map<std::string, Note> adoubleflat = { {"Abb", Note::G } };
    std::map<std::string, Note> aflat = { {"Ab", Note::Ab } };
    std::map<std::string, Note> a = { {"A", Note::A } };
    std::map<std::string, Note> asharp = { {"A#", Note::Bb } };
    std::map<std::string, Note> adoublesharp = { {"Ax", Note::B } };
    std::map<std::string, Note> bdoubleflat = { {"Bbb", Note::A } };
    std::map<std::string, Note> bflat = { {"Bb", Note::Bb } };
    std::map<std::string, Note> b = { {"B", Note::B } };
    std::map<std::string, Note> bsharp = { {"B#", Note::Ab } };
    std::map<std::string, Note> bdoublesharp = { {"Bx", Note::C } };
    std::map<std::string, Note> cdoubleflat = { {"Cbb", Note::Bb } };
    std::map<std::string, Note> cflat = { {"Cb", Note::B } };
    std::map<std::string, Note> c = { {"C", Note::C } };
    std::map<std::string, Note> csharp = { {"C#", Note::Db } };
    std::map<std::string, Note> cdoublesharp = { {"Cx", Note::D } };
    std::map<std::string, Note> ddoubleflat = { {"Dbb", Note::C } };
    std::map<std::string, Note> dflat = { {"Db", Note::Db } };
    std::map<std::string, Note> d = { {"D", Note::D } };
    std::map<std::string, Note> dsharp = { {"D#", Note::Eb } };
    std::map<std::string, Note> ddoublesharp = { {"Dx", Note::E } };
    std::map<std::string, Note> edoubleflat = { {"Ebb", Note::D } };
    std::map<std::string, Note> eflat = { {"Eb", Note::Eb } };
    std::map<std::string, Note> e = { {"E", Note::E } };
    std::map<std::string, Note> esharp = { {"E#", Note::F } };
    std::map<std::string, Note> edoublesharp = { {"Ex", Note::Gb } };
    std::map<std::string, Note> fdoubleflat = { {"Fbb", Note::Eb } };
    std::map<std::string, Note> fflat = { {"Fb", Note::E } };
    std::map<std::string, Note> f = { {"F", Note::F } };
    std::map<std::string, Note> fsharp = { {"F#", Note::Gb } };
    std::map<std::string, Note> fdoublesharp = { {"Fx", Note::G } };
    std::map<std::string, Note> gflat = { {"Gb", Note::Gb } };
    std::map<std::string, Note> gdoubleflat = { {"Gbb", Note::F } };
    std::map<std::string, Note> g = { {"G", Note::G } };
    std::map<std::string, Note> gsharp = { {"G#", Note::Ab } };
    std::map<std::string, Note> gdoublesharp = { {"Gx", Note::A } };

    // TODO: Update the comment below
    // These can be thought of as absolute note groupings, as in, they are all of a specific Note enum. They are ordered from most commonly 
    // seen to least commonly seen and that will be used in determining the level of randomness. This could also be thought of as which #/b 
    // comes first in the order of #s/bs. For example, Bb is the 1st b which I would put before A# which is the 5th #. Some of these groupings
    // In the scenario where there are 2 notes of similar enough commonality (like noteGroup4) I just alternate them.
    std::array<std::map<std::string, Note>, 5> noteGroup0 = { a, a, a, bdoubleflat, gdoublesharp };
    std::array<std::map<std::string, Note>, 5> noteGroup1 = { bflat, asharp, bflat, asharp, cdoubleflat };
    std::array<std::map<std::string, Note>, 5> noteGroup2 = { b, b, b, cflat, adoublesharp };
    std::array<std::map<std::string, Note>, 5> noteGroup3 = { c, c, c, bsharp, ddoubleflat };
    std::array<std::map<std::string, Note>, 5> noteGroup4 = { csharp, dflat, csharp, dflat, csharp };
    std::array<std::map<std::string, Note>, 5> noteGroup5 = { d, d, d, d, edoubleflat };
    std::array<std::map<std::string, Note>, 5> noteGroup6 = { eflat, dsharp, eflat, dsharp, fdoubleflat };
    std::array<std::map<std::string, Note>, 5> noteGroup7 = { e, e, e, fflat, ddoublesharp };
    std::array<std::map<std::string, Note>, 5> noteGroup8 = { f, f, f, esharp, gdoubleflat };
    std::array<std::map<std::string, Note>, 5> noteGroup9 = { fsharp, gflat, fsharp, gflat, edoublesharp };
    std::array<std::map<std::string, Note>, 5> noteGroup10 = { g, g, g, fdoublesharp, adoubleflat };
    std::array<std::map<std::string, Note>, 5> noteGroup11 = { aflat, gsharp, aflat, gsharp, aflat };

    // TODO: Might want to consider reworking some of the above, I only gave the array room enough for 3 since that was the max amount of
    // versions the Note enum was used in. However, if I'm going to use the 3 spaces as a way to control the level of randomness (i.e. light
    // randomness = only using notes in noteGroup[0], light-medium randomnes = using modulo to sometimes only use noteGroup[0] and other times
    // use from noteGroup[1], medium randomness = using notes up to noteGroup[1], and so on) then the arrangement of say, noteGroup0, doesn't
    // make much sense. Even with light-medium randomness there is no reason to have an Bbb there. What I'm thinking is perhaps to increase the
    // space for the array to hold like 5 or 6 spaces and just repeat the more common ones towards the beginning.

    std::map<short, std::array<std::map<std::string, Note>, 5>> noteGroupings = {
        {0, noteGroup0}, {1, noteGroup1}, {2, noteGroup2}, {3, noteGroup3}, {4, noteGroup4}, {5, noteGroup5}, {6, noteGroup6}, 
        {7, noteGroup7}, {8, noteGroup8}, {9, noteGroup9}, {10, noteGroup10}, {11, noteGroup11}
    };

    std::array<std::map<std::string, Note>, 7> AflatScale = { 
        aflat, bflat, c, dflat, eflat, f, g
    };
    std::array<std::map<std::string, Note>, 7> AScale = {
        a, b, csharp, d, e, fsharp, gsharp
    };
    std::array<std::map<std::string, Note>, 7> AsharpScale = {
        asharp, bsharp, cdoublesharp, dsharp, esharp, fdoublesharp, gdoublesharp
    };
    std::array<std::map<std::string, Note>, 7> BflatScale = {
        bflat, c, d, eflat, f, g, a
    };
    std::array<std::map<std::string, Note>, 7> BScale = {
        b, csharp, dsharp, e, fsharp, gsharp, asharp
    };
    std::array<std::map<std::string, Note>, 7> BsharpScale = {
        bsharp, cdoublesharp, ddoublesharp, esharp, fdoublesharp, gdoublesharp, adoublesharp
    };
    std::array<std::map<std::string, Note>, 7> CflatScale = {
        cflat, dflat, eflat, fflat, gflat, aflat, bflat
    };
    std::array<std::map<std::string, Note>, 7> CScale = {
        c, d, e, f, g, a, b
    };
    std::array<std::map<std::string, Note>, 7> CsharpScale = {
        csharp, dsharp, esharp, fsharp, gsharp, asharp, bsharp
    };
    std::array<std::map<std::string, Note>, 7> DflatScale = {
        dflat, eflat, f, gflat, aflat, bflat, c
    };
    std::array<std::map<std::string, Note>, 7> DScale = {
        d, e, fsharp, g, a, b, csharp
    };
    std::array<std::map<std::string, Note>, 7> DsharpScale = {
        dsharp, esharp, fdoublesharp, gsharp, asharp, bsharp, cdoublesharp
    };
    std::array<std::map<std::string, Note>, 7> EflatScale = {
        eflat, f, g, aflat, bflat, c, d
    };
    std::array<std::map<std::string, Note>, 7> EScale = {
        e, fsharp, gsharp, a, b, csharp, dsharp
    };
    std::array<std::map<std::string, Note>, 7> EsharpScale = {
        esharp, fdoublesharp, gdoublesharp, asharp, bsharp, cdoublesharp, ddoublesharp
    };
    std::array<std::map<std::string, Note>, 7> FflatScale = {
        fflat, gflat, aflat, bdoubleflat, cflat, dflat, eflat
    };
    std::array<std::map<std::string, Note>, 7> FScale = {
        f, g, a, bflat, c, d, e
    };
    std::array<std::map<std::string, Note>, 7> FsharpScale = {
        fsharp, gsharp, asharp, b, csharp, dsharp, esharp
    }; 
    std::array<std::map<std::string, Note>, 7> GflatScale = {
        gflat, aflat, bflat, cflat, dflat, eflat, f
    };
    std::array<std::map<std::string, Note>, 7> GScale = {
        g, a, b, c, d, e, fsharp
    };
    std::array<std::map<std::string, Note>, 7> GsharpScale = {
        gsharp, asharp, bsharp, csharp, dsharp, esharp, fdoublesharp
    };

    Scale Ab = Scale("Ab", Note::Ab, AflatScale, &GSharp);
    Scale A = Scale("A", Note::A, AScale, nullptr);
    Scale ASharp = Scale("A#", Note::Bb, AsharpScale, &Bb);
    Scale Bb = Scale("Bb", Note::Bb, BflatScale, &ASharp);
    Scale B = Scale("B", Note::B, BScale, &Cb);
    Scale BSharp = Scale("B#", Note::C, BsharpScale, &C);
    Scale Cb = Scale("Cb", Note::B, CflatScale, &B);
    Scale C = Scale("C", Note::C, CScale, &BSharp);
    Scale CSharp = Scale("C#", Note::Db, CsharpScale, &Db);
    Scale Db = Scale("Db", Note::Db, DflatScale, &CSharp);
    Scale D = Scale("D", Note::D, DScale, nullptr);
    Scale DSharp = Scale("D#", Note::Eb, DsharpScale, &Eb);
    Scale Eb = Scale("Eb", Note::Eb, EflatScale, &DSharp);
    Scale E = Scale("E", Note::E, EScale, &Fb);
    Scale ESharp = Scale("E#", Note::F, EsharpScale, &F);
    Scale Fb = Scale("Fb", Note::E, FflatScale, &E);
    Scale F = Scale("F", Note::F, FScale, &ESharp);
    Scale FSharp = Scale("F#", Note::Gb, FsharpScale, &Gb);
    Scale Gb = Scale("Gb", Note::Gb, GflatScale, &FSharp);
    Scale G = Scale("G", Note::G, GScale, nullptr);
    Scale GSharp = Scale("G#", Note::Ab, GsharpScale, &Ab);

    std::array<Scale, 21> Scales = {
        Ab, A, ASharp, Bb, B, BSharp, Cb, C, CSharp, Db, D, DSharp, Eb, E, ESharp, Fb, F, FSharp, Gb, G, GSharp
    };

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

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
