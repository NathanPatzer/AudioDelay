/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class DelayAudioProcessorEditor  : public juce::AudioProcessorEditor
                                   
{
public:
    DelayAudioProcessorEditor (DelayAudioProcessor&);
    ~DelayAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DelayAudioProcessor& audioProcessor;

    juce::Slider delayLengthSlider;
    juce::Label delayLengthLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayLength;

    juce::Slider feedBackSlider;
    juce::Label feedBackLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedBack;

    juce::Slider wetSlider;
    juce::Label wetLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wet;

    juce::Slider drySlider;
    juce::Label dryLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dry;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> on_Off;
    juce::ToggleButton onOffButton;
    juce::Label onOffButtonLabel;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> pp;
    juce::ToggleButton ppButton;
    juce::Label ppLabel;

    juce::Slider ppAmtSlider;
    juce::Label ppAmtLabel;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> ppAmt;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DelayAudioProcessorEditor)
};
