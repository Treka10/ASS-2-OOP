
#include "PlayerAudio.h"

PlayerAudio::PlayerAudio()
{
    formatManager.registerBasicFormats();  
    formatManager.registerFormat(new juce::MP3AudioFormat(), true);  
    resampleSource = std::make_unique<juce::ResamplingAudioSource>(&transportSource, false);
}


PlayerAudio::~PlayerAudio()
{
}

void PlayerAudio::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource->prepareToPlay(samplesPerBlockExpected, sampleRate); 

}

void PlayerAudio::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
    // transportSource.getNextAudioBlock(bufferToFill);
    resampleSource->getNextAudioBlock(bufferToFill); 
}

void PlayerAudio::releaseResources()
{
    transportSource.releaseResources();
    resampleSource->releaseResources();
}

bool PlayerAudio::loadFile(const juce::File& file)
{
    if (file.existsAsFile())
    {
        if (auto* reader = formatManager.createReaderFor(file))
        {
            transportSource.stop();
            transportSource.setSource(nullptr);
            readerSource.reset();

            readerSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);

            transportSource.setSource(readerSource.get(), 0,nullptr,  reader->sampleRate);
            transportSource.setPosition(0.0);
            transportSource.start();
            return true;



        }
    }

    return false;
}

void PlayerAudio::start()
{
    transportSource.start();
}

void PlayerAudio::stop()
{
    transportSource.stop();
}

void PlayerAudio::setGain(float gain)
{
    transportSource.setGain(gain);
}

void PlayerAudio::setPosition(double pos)
{
    transportSource.setPosition(pos);
}

double PlayerAudio::getPosition() const
{
    return transportSource.getCurrentPosition();
}

double PlayerAudio::getLength() const
{
    return transportSource.getLengthInSeconds();
}  
void PlayerAudio::setLooping(bool shouldLoop)
{
    if (readerSource != nullptr)
        readerSource->setLooping(shouldLoop);
}
void PlayerAudio::setSpeed(double ratio)
{
	currentSpeed = ratio;
    if(resampleSource)
    {
        resampleSource->setResamplingRatio(currentSpeed);
	}
}



