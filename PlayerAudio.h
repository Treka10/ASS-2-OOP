#pragma once       // PlayerAudio.h 
#include <JuceHeader.h> 
using namespace juce;
class PlayerAudio
{
public:
    PlayerAudio();
    ~PlayerAudio();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill);
    void releaseResources();

    bool loadFile(const juce::File& file);
    void start();
    void stop();
    void setGain(float gain);
    void setPosition(double pos);
    double getPosition() const;
    double getLength() const;
    void setLooping(bool shouldLoop);
	void setSpeed(double ratio);

	AudioFormatManager& getFormatManager() { return formatManager; }

private:
    juce::AudioFormatManager formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
	std::unique_ptr<juce::ResamplingAudioSource> resampleSource;
	double currentSpeed = 1.0;
     JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlayerAudio) 
};

