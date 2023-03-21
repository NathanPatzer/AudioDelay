/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DelayAudioProcessorEditor::DelayAudioProcessorEditor (DelayAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    //Delay Length
    delayLengthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delayLengthSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 100, 25);
    addAndMakeVisible(delayLengthSlider);
    delayLength = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DELAYLENGTH", delayLengthSlider);
    //Label
    addAndMakeVisible(delayLengthLabel);
    delayLengthLabel.setText("Length", juce::dontSendNotification);
    delayLengthLabel.attachToComponent(&delayLengthSlider, true);
    delayLengthSlider.setTextBoxIsEditable(true);
    
    //FeedBack
    feedBackSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    feedBackSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 100, 25);
    addAndMakeVisible(feedBackSlider);
    feedBack = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "FEEDBACK", feedBackSlider);
    //Label
    addAndMakeVisible(feedBackLabel);
    feedBackLabel.setText("Feedback", juce::dontSendNotification);
    feedBackLabel.attachToComponent(&feedBackSlider, true);
    feedBackSlider.setTextBoxIsEditable(true);
    
    //wet
    wetSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    wetSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 100, 25);
    addAndMakeVisible(wetSlider);
    wet = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "WET", wetSlider);
    //Label
    addAndMakeVisible(wetLabel);
    wetLabel.setText("Wet", juce::dontSendNotification);
    wetLabel.attachToComponent(&wetSlider, true);
    wetSlider.setTextBoxIsEditable(true);

    //dry
    drySlider.setSliderStyle(juce::Slider::LinearHorizontal);
    drySlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 100, 25);
    addAndMakeVisible(drySlider);
    dry = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "DRY", drySlider);
    //Label
    addAndMakeVisible(dryLabel);
    dryLabel.setText("Dry", juce::dontSendNotification);
    dryLabel.attachToComponent(&drySlider, true);
    drySlider.setTextBoxIsEditable(true);

    //On/Off
    addAndMakeVisible(onOffButton);
    on_Off = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "OFF", onOffButton);
    setSize(800, 500);
    //Label
    addAndMakeVisible(onOffButtonLabel);
    onOffButtonLabel.setText("On/Off", juce::dontSendNotification);
    onOffButtonLabel.attachToComponent(&onOffButton, true);

    //pingpong
    addAndMakeVisible(ppButton);
    pp = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(audioProcessor.apvts, "PINGPONG", ppButton);
    //Label
    addAndMakeVisible(ppLabel);
    ppLabel.setText("Ping Pong", juce::dontSendNotification);
    ppLabel.attachToComponent(&ppButton, true);
    

    //pingpongamt
    ppAmtSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    ppAmtSlider.setTextBoxStyle(juce::Slider::TextBoxLeft, true, 100, 25);
    addAndMakeVisible(ppAmtSlider);
    ppAmt = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.apvts, "PINGPONGAMT", ppAmtSlider);
    //Label
    addAndMakeVisible(ppAmtLabel);
    ppAmtLabel.setText("Pan", juce::dontSendNotification);
    ppAmtLabel.attachToComponent(&ppAmtSlider, true);
    ppAmtSlider.setTextBoxIsEditable(true);


    setSize(800, 500);
}

DelayAudioProcessorEditor::~DelayAudioProcessorEditor()
{
}

//==============================================================================
void DelayAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(juce::Colours::darkslateblue);

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    //g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
}

void DelayAudioProcessorEditor::resized()
{
    feedBackSlider.setBounds(100, 50, 500, 25);
    delayLengthSlider.setBounds(100, 100, 500, 25);
    wetSlider.setBounds(100, 150, 500, 25);
    drySlider.setBounds(100, 200, 500, 25);
    onOffButton.setBounds(100, 250, 25, 25);
    ppButton.setBounds(100, 300, 25, 25);
    ppAmtSlider.setBounds(100, 350, 500, 25);
}

