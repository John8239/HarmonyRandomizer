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
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    std::string newNote = deterimineNextRootNote(/*Step::WholeStep*/);
    std::array<std::string, 7> newChord = determineChord();

    for (int i = 0; i < NumForChordsArray; i++)
    {
        DBG(newChord[i] + "\n");
    }

    DBG("\n\n");

    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
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

// This sets CurrentNote (declared in MainComponent.h) and returns the string version of the new note determined
std::string MainComponent::deterimineNextRootNote(StepEnum step)
{
    std::string returnNote;

    int noteNumericVal = (int)CurrentNote;
    int newNoteNumericVal;

    srand(time(0));
    int randomNum = rand();

    newNoteNumericVal = noteNumericVal += randomNum;
    newNoteNumericVal = newNoteNumericVal % 12;
    
    CurrentNote = (NoteEnum)newNoteNumericVal;

    // Since not all Notes in MusicalAlphabet have multiple names, check to make sure the index 
    // grabbed is not empty. If it is, just use [0] because that will always be populated
    returnNote = !MusicalAlphabet[CurrentNote][randomNum % NumOfEnharmNoteNames].empty() 
        ? MusicalAlphabet[CurrentNote][randomNum % NumOfEnharmNoteNames] : MusicalAlphabet[CurrentNote][0];

    return returnNote;
}

std::array<std::string, 7> MainComponent::determineChord(ChordEnum chord)
{
    ChordEnum newChord;
    std::array<std::string, 7> returnChord;
    if (chord == ChordEnum::NoChord)
    {
        srand(time(0));
        int randomNum = rand() % 5;

        newChord = ChordOptions[randomNum];
    }
    else
    {
        newChord = chord;
    }

    int numOfThirds = (int)newChord;
    NoteEnum currentNote = CurrentNote;
    std::array<IntervalEnum, 2> thirds = { IntervalEnum::min3, IntervalEnum::Maj3 };

    std::string currentNoteString = MusicalAlphabet[currentNote][0];
    returnChord[0] = currentNoteString; // TODO: Will have to update this to work with determineNoteByInterval()
    for (int i = 1; i < numOfThirds; i++)
    {
        srand(time(0));

        IntervalEnum whichThird = thirds[rand() % 2];
        struct NoteStruct nextNote = determineNoteByInterval(whichThird, currentNote, currentNoteString);

        returnChord[i] = nextNote.noteString;
        currentNote = nextNote.noteEnum;
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