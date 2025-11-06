
#include "PlayerGUI.h"

using namespace juce;



void PlayerGUI::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    playerAudio.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void PlayerGUI::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    playerAudio.getNextAudioBlock(bufferToFill);
}

void PlayerGUI::releaseResources()
{
    playerAudio.releaseResources();
}
void PlayerGUI::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::darkgrey);
}

PlayerGUI::PlayerGUI()
{
    // Add buttons
    for (auto* btn : { &loadButton, &restartButton , &stopButton , &startButton , &pauseButton , &GoToEndButton , &GoToStartButton , &LoopButton ,&muteButton })
    {
        btn->addListener(this);
        addAndMakeVisible(btn);
    }

    // Volume slider
    volumeSlider.setRange(0.0, 1.0, 0.01);
    volumeSlider.setValue(0.5);
    volumeSlider.addListener(this);
    addAndMakeVisible(volumeSlider);
	
	speedlabel.setText("Speed", juce::dontSendNotification); 
	speedlabel.setJustificationType(juce::Justification::centred); 
	addAndMakeVisible(speedlabel);
	
	addAndMakeVisible(speedSlider);
	speedSlider.setRange(0.5, 2.0, 0.01);
	speedSlider.setValue(1.0);
	speedSlider.addListener(this);

	addAndMakeVisible(progressBar); 
	progressBar.setPercentageDisplay(false);

	addAndMakeVisible(timeLabel); 
	timeLabel.setJustificationType(juce::Justification::centred);
	timeLabel.setColour(juce::Label::textColourId, juce::Colours::white); 

	setSize(600, 300);
	startTimerHz(30); 
    MetaData.setColour(juce::Label::textColourId, juce::Colours::blue);
    MetaData.setJustificationType(juce::Justification::centredLeft);
    addAndMakeVisible(MetaData);

    addAndMakeVisible(playlist);
    playlist.setModel(this);

}

void PlayerGUI::resized()
{
    int y = 20;
    loadButton.setBounds(20, y, 100, 40);
    restartButton.setBounds(140, y, 80, 40);
    stopButton.setBounds(240, y, 80, 40);
	startButton.setBounds(340, y, 80, 40);
	pauseButton.setBounds(440, y, 80, 40);
	GoToEndButton.setBounds(540, y, 100, 40);
    GoToStartButton.setBounds(660, y, 120, 40);
	LoopButton.setBounds(800, y, 80, 40);
	muteButton.setBounds(900, y, 60, 40);

    volumeSlider.setBounds(20, 100, getWidth() - 40, 30);
	speedlabel.setBounds(20, 140, 60, 30); 
	speedSlider.setBounds(90, 140, getWidth() - 110, 30);

	progressBar.setBounds(20, 200, getWidth() - 40, 6); 
	timeLabel.setBounds(20, 230, getWidth() - 40, 20); 
	
	MetaData.setBounds(20, 150, getWidth() - 40, 100);
	playlist.setBounds(20, 270, getWidth() - 40, getHeight() - 300);
}

PlayerGUI::~PlayerGUI()
{
}

void PlayerGUI::buttonClicked(juce::Button* button)
{
    if (button == &loadButton)
    {
        juce::FileChooser chooser("Select audio files...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser = std::make_unique<juce::FileChooser>(
            "Select an audio file...",
            juce::File{},
            "*.wav;*.mp3");

        fileChooser->launchAsync(
            juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
            [this](const juce::FileChooser& fc)
            {
                auto file = fc.getResult();
                if (file.existsAsFile())
                {
                    playerAudio.loadFile(file);

                    if (!playlistAudio.contains(file))
                        playlistAudio.add(file);

                    playlist.updateContent();


                   
                    if (auto* reader = playerAudio.getFormatManager().createReaderFor(file))
                    {
                        juce::String info;
                        auto& metadata = reader->metadataValues;

                        info << "File: " << file.getFileName() << "\n";

                        if (metadata.size() > 0)
                        {
                            info << "\n---- Metadata ----\n";

                            for (auto key : metadata.getAllKeys())
                            {

                                if (key.equalsIgnoreCase("ISFT"))
                                    continue;

                                juce::String readableKey = key;

                                if (key.equalsIgnoreCase("TIT2") || key.equalsIgnoreCase("INAM"))
                                    readableKey = "Title";
                                else if (key.equalsIgnoreCase("TPE1") || key.equalsIgnoreCase("IART"))
                                    readableKey = "Artist";
                                else if (key.equalsIgnoreCase("TALB") || key.equalsIgnoreCase("IPRD"))
                                    readableKey = "Album";
                                else if (key.equalsIgnoreCase("COMM") || key.equalsIgnoreCase("ICMT"))
                                    readableKey = "Comment";

                                info << readableKey << ": " << metadata[key] << "\n";
                            }

                            info << "-------------------\n";
                        }
                        else
                        {
                            info << "No metadata found.\n";
                        }

                        double lengthInSeconds = reader->lengthInSamples / reader->sampleRate;
                        info << "Length: " << juce::String(lengthInSeconds, 2) << " sec\n";

                        MetaData.setText(info, juce::dontSendNotification);
						
                        delete reader;
                    }




                }
            });
    }

    if (button == &restartButton)
    {
        playerAudio.stop();
        playerAudio.setPosition(0.0);
        playerAudio.start();
    }

    if (button == &stopButton)
    {
        playerAudio.stop();
        playerAudio.setPosition(0.0);
    }

    if (button == &startButton)
    {
        playerAudio.start();
    }

    if (button == &pauseButton)
    {
        playerAudio.stop();
    }

    if (button == &GoToStartButton)
    {
        playerAudio.setPosition(0.0);
    }

    if (button == &GoToEndButton)
    {
        playerAudio.setPosition(playerAudio.getLength());
    }

    if (button == &LoopButton)
    {
        isLooping = !isLooping;
        playerAudio.setLooping(isLooping);

        if (isLooping)
            LoopButton.setButtonText("Looping");
        else
            LoopButton.setButtonText("Loop ()");
    }

    if (button == &muteButton)
    {
        isMuted = !isMuted;

        if (isMuted)
        {
            playerAudio.setGain(0.0f);
            muteButton.setButtonText("unmute");
        }
        else
        {
            playerAudio.setGain((float)volumeSlider.getValue());
            muteButton.setButtonText("mute");
        }
    }
}
void PlayerGUI::sliderValueChanged(juce::Slider* slider)
{
    if (slider == &volumeSlider)
        playerAudio.setGain((float)slider->getValue());
	else if (slider == &speedSlider)
	{
		playerAudio.setSpeed(slider->getValue());
	}
}
void PlayerGUI::setSpeedSliderValue(double speed)
{
    speedSlider.setValue(speed);
}
void PlayerGUI::timerCallback()
{
    double position = playerAudio.getPosition(); 
    double length = playerAudio.getLength();     

    if (length > 0)
    {
        progress = position / length;

        int currentMinutes = static_cast<int>(position) / 60;
        int currentSeconds = static_cast<int>(position) % 60;
        int totalMinutes = static_cast<int>(length) / 60;
        int totalSeconds = static_cast<int>(length) % 60;

        juce::String timeText = juce::String::formatted("%d:%02d / %d:%02d",
            currentMinutes, currentSeconds,
            totalMinutes, totalSeconds);

        timeLabel.setText(timeText, juce::dontSendNotification);
    }
    else
    {
        progress = 0.0;
        timeLabel.setText("0:00 / 0:00", juce::dontSendNotification);
    }

    repaint();
}

int PlayerGUI::getNumRows()
{
    return playlistAudio.size();
}

void PlayerGUI::paintListBoxItem(int rowNumber, juce::Graphics& G, int W, int H, bool SelectAudio)
{
    if (SelectAudio)
        G.fillAll(Colours::lightblue);
    else
        G.fillAll(Colours::grey);

    if (rowNumber < playlistAudio.size())
    {
        G.setColour(Colours::white);
        G.drawText(playlistAudio[rowNumber].getFileName(),
            10, 0, W - 10, H,
            juce::Justification::centredLeft);
    }
}

void PlayerGUI::selectedRowsChanged(int lastRowSelected)
{
    if (lastRowSelected >= 0 && lastRowSelected < playlistAudio.size())
    {
        currentFileIndex = lastRowSelected;
        auto file = playlistAudio[currentFileIndex];

        playerAudio.loadFile(file);

        if (auto* reader = playerAudio.getFormatManager().createReaderFor(file))
        {
            juce::String info;
            auto& metadata = reader->metadataValues;

            info << "File: " << file.getFileName() << "\n";

            if (metadata.size() > 0)
            {
                info << "\n---- Metadata ----\n";

                for (auto key : metadata.getAllKeys())
                {
                    if (key.equalsIgnoreCase("ISFT"))
                        continue;

                    juce::String readableKey = key;

                    if (key.equalsIgnoreCase("TIT2") || key.equalsIgnoreCase("INAM"))
                        readableKey = "Title";
                    else if (key.equalsIgnoreCase("TPE1") || key.equalsIgnoreCase("IART"))
                        readableKey = "Artist";
                    else if (key.equalsIgnoreCase("TALB") || key.equalsIgnoreCase("IPRD"))
                        readableKey = "Album";
                    else if (key.equalsIgnoreCase("COMM") || key.equalsIgnoreCase("ICMT"))
                        readableKey = "Comment";

                    info << readableKey << ": " << metadata[key] << "\n";
                }

                info << "-------------------\n";
            }
            else
            {
                info << "No metadata found.\n";
            }

            double lengthInSeconds = reader->lengthInSamples / reader->sampleRate;
            info << "Length: " << juce::String(lengthInSeconds, 2) << " sec\n";

            MetaData.setText(info, juce::dontSendNotification);

            delete reader;
        }
    }
}


