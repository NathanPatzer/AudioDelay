/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessor::DelayAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParameters())
#endif
{
}

DelayAudioProcessor::~DelayAudioProcessor()
{
}

//==============================================================================
const juce::String DelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DelayAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DelayAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DelayAudioProcessor::getProgramName (int index)
{
    return {};
}

void DelayAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    auto delayBufferSize = sampleRate * 1.0f;
    delayBufferR.setSize(1, (int)delayBufferSize);
    delayBufferL.setSize(1, (int)delayBufferSize);

    //initialize buffer
    for (int i = 0; i < delayBufferSize; i++)
    {
        delayBufferR.setSample(0, i, 0.0f);
        delayBufferL.setSample(0, i, 0.0f);
    }
}

void DelayAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DelayAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{   
    auto delayBufferSize = (int)(getDLength() * getSampleRate());
    //Setting speed of delay
    delayBufferL.setSize(1, delayBufferSize, true, true, true);
    delayBufferR.setSize(1, delayBufferSize, true, true, true);
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

        auto* channelDataL = buffer.getWritePointer(0);
        auto* channelDataR = buffer.getWritePointer(1);
        if (isOff() == false)
        {
            for (int sample = 0; sample < buffer.getNumSamples(); sample++)
            {
                delay(buffer, channelDataR, channelDataL, sample);
            }
        }
        else
        {
            for (int i = 0; i < delayBufferSize; i++)
            {
                delayBufferR.setSample(0, i, 0.0f);
                delayBufferL.setSample(0, i, 0.0f);
            }
        }
}

void DelayAudioProcessor::delay(juce::AudioBuffer<float>& buffer, float* channelDataR, float* channelDataL, int sample)
{
    auto Lsample = channelDataL[sample];
    auto Rsample = channelDataR[sample];
    auto delayBufferSize = delayBufferL.getNumSamples();
    float delayedSampleL = delayBufferL.getSample(0, readPos);
    float delayedSampleR = delayBufferR.getSample(0, readPos);
    if (isPingPong() == false)
    {
        delayBufferL.setSample(0, writePos, (getFeedback() * delayedSampleL) + channelDataL[sample]);
        delayBufferR.setSample(0, writePos, ((getFeedback() * delayedSampleR)) + channelDataR[sample]);
    }
    else
    {
        if (getPingPongAmt() < 0.0f)
        {
            channelDataR[sample] = Rsample * (getPingPongAmt() + 1.0f);
        }
        else if (getPingPongAmt() > 0.0f)
        {
            channelDataL[sample] = Lsample * (1 - getPingPongAmt());
        }
        else
        {
            channelDataL[sample] = Lsample;
            channelDataR[sample] = Rsample;
        }
        
        delayBufferL.setSample(0, writePos, (getFeedback() * delayedSampleR) + channelDataL[sample]);
        delayBufferR.setSample(0, writePos, ((getFeedback() * delayedSampleL)) + channelDataR[sample]);
    }
    readPos = (readPos + 1) % delayBufferSize;
    writePos = (writePos + 1) % delayBufferSize;
    channelDataL[sample] = (getDry() * Lsample) + (getWet() * delayedSampleL);
    channelDataR[sample] = (getDry() * Rsample) + (getWet() * delayedSampleR);
}

//==============================================================================
bool DelayAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DelayAudioProcessor::createEditor()
{
    return new DelayAudioProcessorEditor (*this);
}

//==============================================================================
void DelayAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DelayAudioProcessor();
}

juce::AudioProcessorValueTreeState::ParameterLayout DelayAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>("DELAYLENGTH", "Delay length", .01f, 3.0f, .5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FEEDBACK", "feedback", .01f, .99f, .5f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DRY", "dry", .0f, 1.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("WET", "wet", .0f, 1.0f, 1.0f));
    params.push_back(std::make_unique<juce::AudioParameterBool>("OFF", "off", false));
    params.push_back(std::make_unique<juce::AudioParameterBool>("PINGPONG", "pingpong", false));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("PINGPONGAMT", "Ping Pong Amt", -1.0f, 1.0f, 0.0f));

    return { params.begin(),params.end() };
}

float DelayAudioProcessor::getDLength()
{
    auto g = apvts.getRawParameterValue("DELAYLENGTH");
    float d = g->load();
    return d;
}

float DelayAudioProcessor::getFeedback()
{
    auto g = apvts.getRawParameterValue("FEEDBACK");
    float d = g->load();
    return d;
}

float DelayAudioProcessor::getWet()
{
    auto g = apvts.getRawParameterValue("WET");
    float d = g->load();
    return d;
}

float DelayAudioProcessor::getDry()
{
    auto g = apvts.getRawParameterValue("DRY");
    float d = g->load();
    return d;
}

bool DelayAudioProcessor::isOff()
{
    auto g = apvts.getRawParameterValue("OFF");
    float d = g->load();
    return d;
}

bool DelayAudioProcessor::isPingPong()
{
    auto g = apvts.getRawParameterValue("PINGPONG");
    float d = g->load();
    return d;
}

float DelayAudioProcessor::getPingPongAmt()
{
    auto g = apvts.getRawParameterValue("PINGPONGAMT");
    float d = g->load();
    return d;
}