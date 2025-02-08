#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (800, 600);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    _frequency = 440;
    _phase = 0;
    _waveTableSize = 1024;
    _amplitude = 0.25;
    _currentSampleRate = sampleRate;

    // One cycle of a sine wave
    for (int i = 0; i < _waveTableSize; i++)
    {
        _waveTable.insert(i, sin(2.0 * juce::double_Pi * i / _waveTableSize));
    }
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    deterimineNextRootNote(/*Step::WholeStep*/);
    std::array<std::string, 7> newChord = determineChord(ChordEnum::Seventh); // Just using 7th chords for the sake of testing
    
    for (int i = 0; i < NumForChordsArray; i++)
    {
        DBG(newChord[i] + "\n");
    }

    DBG("\n\n");

    // Ripping this straight from The Audio Programmer's "Juce Tutorial 11 - Basic Wavetable Synthesis in Juce"
    float* const leftSpeaker = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    float* const rightSpeaker = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (int sample; sample < bufferToFill.numSamples; ++sample)
    {
        leftSpeaker[sample] = _waveTable[(int)_phase] * _amplitude;
        rightSpeaker[sample] = _waveTable[(int)_phase] * _amplitude;
        updateFrequency();

        // LAST LEFT OFF: Ok so now I have a simple sine wave but I need to have the _frequency update according to the 
        // determined chords. First I'll have to send the returnedChord to the updateFrequency() function above. But then
        // we'll need to find a way to have the same frequency value set for more than a split second before updating to
        // the next value without interrupting the flow of processing the audio block, so no sleep() function. First and 
        // foremost though, I'll need to create some sortof list of enums or something that allows me to correlate the 
        // pitches to a frequency. I can just choose one pitch and then double or halve it as needed to get the right octave.
    }

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    //bufferToFill.clearActiveBufferRegion();
}

// This sets CurrentNote (declared in MainComponent.h) and returns the string version of the new note determined
void MainComponent::deterimineNextRootNote(StepEnum step)
{
    int noteNumericVal = (int)CurrentNote.noteEnum;
    int newNoteNumericVal;

    srand(time(0));
    int randomNum = rand();

    newNoteNumericVal = noteNumericVal += randomNum;
    newNoteNumericVal = newNoteNumericVal % 12;

    CurrentNote.noteEnum = (NoteEnum)newNoteNumericVal;

    // Since not all Notes in MusicalAlphabet have multiple names, check to make sure the index 
    // grabbed is not empty. If it is, just use [0] because that will always be populated
    CurrentNote.noteString = !MusicalAlphabet[CurrentNote.noteEnum][randomNum % NumOfEnharmNoteNames].empty()
        ? MusicalAlphabet[CurrentNote.noteEnum][randomNum % NumOfEnharmNoteNames] : MusicalAlphabet[CurrentNote.noteEnum][0];
}

std::array<std::string, 7> MainComponent::determineChord(ChordEnum chord)
{
    ChordEnum newChord;
    std::array<std::string, 7> returnChord;
    if (chord == ChordEnum::NoChord)
    {
        // TODO: We'll eventually want to elaborate on this function so that it's not always complete randomness.
        // We'll want some sort of determinative logic behind it so we don't giet diminished and augmented chords 50% of the time.
        srand(time(0));
        int randomNum = rand() % 5;

        newChord = ChordOptions[randomNum];
    }
    else
    {
        newChord = chord;
    }

    int numOfThirds = (int)newChord;
    NoteEnum currentNote = CurrentNote.noteEnum;
    std::array<IntervalEnum, 2> thirds = { IntervalEnum::min3, IntervalEnum::Maj3 };

    std::string currentNoteString = CurrentNote.noteString;
    returnChord[0] = currentNoteString;

    for (int i = 1; i < numOfThirds + 1; i++)
    {
        srand(time(0));

        IntervalEnum whichThird = thirds[rand() % 2];
        struct NoteStruct nextNote = determineNoteByInterval(whichThird, currentNote, currentNoteString);

        currentNote = nextNote.noteEnum;
        currentNoteString = nextNote.noteString;
        returnChord[i] = currentNoteString;
    }

    return returnChord;
}

// This takes in an Interval and a Note and determines what the next note is with the correct spelling
MainComponent::NoteStruct MainComponent::determineNoteByInterval(IntervalEnum interval, NoteEnum note, std::string noteString)
{
    GenericIntervalEnum genericInterval;

    // Take index 0 of the note string to get the letter value only
    char currentLetter = noteString[0];

    // Get the integer values of the note and interval
    int numOfHalfSteps = (int)interval;
    int numOfNote = (int)note;

    // Get index of the current letter in the MusicalLetters array
    int currentLetterIndex = 0;
    for (currentLetterIndex; currentLetterIndex < 7; currentLetterIndex++)
    {
        if (MusicalLetters[currentLetterIndex] == currentLetter)
        {
            break;
        }
    }

    // Determine the generic interval
    switch (interval)
    {
    case IntervalEnum::Unison:
        genericInterval = GenericIntervalEnum::Unison;
        break;
    case IntervalEnum::min2:
    case IntervalEnum::Maj2:
        genericInterval = GenericIntervalEnum::Second;
        break;
    case IntervalEnum::min3:
    case IntervalEnum::Maj3:
        genericInterval = GenericIntervalEnum::Third;
        break;
    case IntervalEnum::P4:
    case IntervalEnum::Tritone: //I'm just arbitrarily lumping the Tritone in with the 4th, could have been 5th but who cares
        genericInterval = GenericIntervalEnum::Fourth;
        break;
    case IntervalEnum::P5:
        genericInterval = GenericIntervalEnum::Fifth;
        break;
    case IntervalEnum::min6:
    case IntervalEnum::Maj6:
        genericInterval = GenericIntervalEnum::Sixth;
        break;
    case IntervalEnum::min7:
    case IntervalEnum::Maj7:
        genericInterval = GenericIntervalEnum::Seventh;
        break;
    }
    int nextLetterIndex = (currentLetterIndex += (int)genericInterval) % 7;
    char nextLetter = MusicalLetters[nextLetterIndex];

    // Now that we have the next letter, determine the absolute pitch
    short nextNoteNum = ((short)note + (short)interval) % 12;

    std::array<NoteStruct, 3> noteGroup = noteGroupings.at(nextNoteNum);
    struct NoteStruct newNote;
    for (int i = 0; i < 3; i++)
    {
        if (noteGroup[i].noteString[0] == nextLetter)
        {
            newNote = noteGroup[i];
            break;
        }
    }
    return newNote;
}

void MainComponent::updateFrequency()
{
    _increment = _frequency * _waveTableSize / _currentSampleRate;
    _phase = fmod((_phase + _increment), _waveTableSize);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setFont(14.0);
    g.setColour(juce::Colours::white);
    g.drawText("Test Label", getLocalBounds(), juce::Justification::centred, true);

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}