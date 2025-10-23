#pragma once      // PlayerGUI.h 
#include <JuceHeader.h> 
#include "PlayerAudio.h" 
using namespace juce;
using namespace std;
class PlayerGUI : public juce::Component,
    public juce::Button::Listener,
    public juce::Slider::Listener

{
public:
    PlayerGUI();
    ~PlayerGUI() override;

    void resized() override;
    void paint(juce::Graphics& g) override;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

private:
    PlayerAudio playerAudio;

    // GUI elements 
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


    std::unique_ptr<juce::FileChooser> fileChooser;

    // Event handlers 
    void buttonClicked(juce::Button* button) override;
    void sliderValueChanged(juce::Slider* slider) override;
    bool isLooping = false;
    bool isMuted = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerGUI)
};
