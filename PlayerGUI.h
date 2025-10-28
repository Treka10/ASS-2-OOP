#pragma once      // PlayerGUI.h 
#include <JuceHeader.h> 
#include "PlayerAudio.h" 
using namespace juce;
using namespace std;
class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener ,
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

          Label MetaData;

          ListBox playlist;
          Array<File> playlistAudio;
          int currentFileIndex = -1;

    std::unique_ptr<juce::FileChooser> fileChooser;

    
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    bool isLooping = false;
    bool isMuted = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
