#pragma once      // PlayerGUI.h 
#include <JuceHeader.h> 
#include "PlayerAudio.h" 
using namespace juce;
using namespace std;
class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener ,
    public juce::Timer,
    public ListBoxModel
{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();


    int getNumRows() override;
    void paintListBoxItem(int rowNumber, juce::Graphics& g, int width, int height, bool rowIsSelected) override;
    void selectedRowsChanged(int lastRowSelected) override;
    void timerCallback() override;


private:
    PlayerAudio playerAudio;

          TextButton loadButton{ "Load File" };
          TextButton restartButton{ "Restart" };
          TextButton stopButton{ "Stop" };
          TextButton startButton{ "Start" };
          TextButton pauseButton{ "Pause" };
		  TextButton GoToEndButton{ "GoToEnd >|| " };
		  TextButton GoToStartButton{ "GoToStart ||< " };
		  TextButton LoopButton{ "Loop ()" };
          TextButton muteButton{ "mute" };
          Slider volumeSlider;
          Slider speedSlider;
          Label speedlabel;
          double progress = 0.0;
          ProgressBar progressBar{ progress };
          Label timeLabel;
          Label MetaData;

          ListBox playlist;
          Array<File> playlistAudio;
          int currentFileIndex = -1;

          TextButton setAButton{ "Point A" };
          TextButton setBButton{ "Point B" };
          TextButton loopABButton{ "Loop A-B" };

          double pointA = 0.0;
          double pointB = 0.0;
          bool isABLooping = false;


    std::unique_ptr<juce::FileChooser> fileChooser;

    
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    void setSpeedSliderValue(double speed);
    bool isLooping = false;
    bool isMuted = false;

    // Position slider + time labels
    Slider positionSlider;
    Label currentTimeLabel;
    Label totalTimeLabel;
    juce::Timer* positionTimer = nullptr;

    void updatePosition();

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
