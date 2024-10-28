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
std::string MainComponent::deterimineNextRootNote(Step step)
{
    std::string returnNote;

    int noteNumericVal = (int)CurrentNote;
    int newNoteNumericVal;

    srand(time(0));
    int randomNum = rand();

    newNoteNumericVal = noteNumericVal += randomNum;

    // TODO: Check if this if check is actually necessary or if we can just do the modulo regardless,
    // If we can get rid of the if check then also remove it below in the else block of this function and the 
    // determinChord function.
    if (newNoteNumericVal >= 12)
    {
        newNoteNumericVal = newNoteNumericVal % 12;
    }
    CurrentNote = (Note)newNoteNumericVal;

    // Since not all Notes in MusicalAlphabet have multiple names, check to make sure the index 
    // grabbed is not empty. If it is, just use [0] because that will always be populated
    returnNote = !MusicalAlphabet[CurrentNote][randomNum % NumOfEnharmNoteNames].empty() 
        ? MusicalAlphabet[CurrentNote][randomNum % NumOfEnharmNoteNames] : MusicalAlphabet[CurrentNote][0];

    return returnNote;
}

std::array<std::string, 7> MainComponent::determineChord(Chord chord)
{
    Chord newChord;
    std::array<std::string, 7> returnChord;
    if (chord == Chord::NoChord)
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
    Note currentNote = CurrentNote;
    std::array<Interval, 2> thirds = { Interval::min3, Interval::Maj3 };

    returnChord[0] = MusicalAlphabet[currentNote][0];
    for (int i = 1; i < numOfThirds; i++)
    {
        srand(time(0));

        int whichThird = (int)thirds[rand() % 2];
        currentNote = Note((int)currentNote + whichThird);

        if ((int)currentNote >= 12)
        {
            currentNote = Note((int)currentNote % 12);
        }

        // TODO: Call isCorrectNote here
        returnChord[i] = MusicalAlphabet[currentNote][0];
    }

    return returnChord;
}

// LAST LEFT OFF: I'm trying to implement a function that will determine the correct enharmonic
// spelling of the note needed. -> UPDATE: See other Last Left Off in MainComponent.H
std::string MainComponent::isCorrectNote(Interval interval)
{
    int numOfHalfSteps = (int)interval;
    /*if (numOfHalfSteps)*/
    return "";
}



/*// This sets CurrentNote (declared in MainComponent.h) and returns the string version of the new note determined
std::string MainComponent::deterimineNextRootNote(Step step)
{
    std::string returnNote;

    int noteNumericVal = (int)CurrentNote;
    int newNoteNumericVal;

    // If NoSteps, we will return a completely random note
    if (step == Step::NoSteps)
    {
        srand(time(0));
        int randomNum = rand();

        newNoteNumericVal = noteNumericVal += randomNum;

        // TODO: Check if this if check is actually necessary or if we can just do the modulo regardless,
        // If we can get rid of the if check then also remove it below in the else block of this function and the 
        // determinChord function.
        if (newNoteNumericVal >= 12)
        {
            newNoteNumericVal = newNoteNumericVal % 12;
        }
        CurrentNote = (Note)newNoteNumericVal;

        // Since not all Notes in MusicalAlphabet have multiple names, check to make sure the index 
        // grabbed is not empty. If it is, just use [0] because that will always be populated
        returnNote = !MusicalAlphabet[CurrentNote][randomNum % NumOfEnharmNoteNames].empty()
            ? MusicalAlphabet[CurrentNote][randomNum % NumOfEnharmNoteNames] : MusicalAlphabet[CurrentNote][0];

        return returnNote;
    }
    // Else, add the step to the note... 
    // TOOD: Not entirely sure how necessary this will be but thinking ahead for
    // how we'll want specificity in our randomness, it might be good to have.
    else
    {
        // Add the numeric value of the Step to the Note
        newNoteNumericVal = noteNumericVal += (int)step;

        // TODO: This only adds intervals right now, we should also consider subtracting intervals as well
        // Make sure the new note value isn't >= 12, if it is use modulo to get the accurate value
        if (newNoteNumericVal >= 12)
        {
            newNoteNumericVal = newNoteNumericVal % 12;
        }
        CurrentNote = (Note)newNoteNumericVal;

        // TODO: We have the array of enharmonic names for a reason so we'll have to
        // figure out how to properly determine which note name we should return later.
        returnNote = MusicalAlphabet[CurrentNote][0];
        return returnNote;
    }
}*/