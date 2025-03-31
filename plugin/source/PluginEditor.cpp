#include "YourPluginName/PluginEditor.h"
#include "YourPluginName/PluginProcessor.h"

namespace audio_plugin {

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p) {
    // Set up left channel knobs
    leftLowKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftLowKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    leftLowKnob.setRange(-24.0f, 24.0f, 0.1f);
    leftLowKnob.setValue(0.0f);
    leftLowKnob.setTextValueSuffix(" dB");
    addAndMakeVisible(leftLowKnob);

    leftLowMidKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftLowMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    leftLowMidKnob.setRange(-24.0f, 24.0f, 0.1f);
    leftLowMidKnob.setValue(0.0f);
    leftLowMidKnob.setTextValueSuffix(" dB");
    addAndMakeVisible(leftLowMidKnob);

    leftHighMidKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftHighMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    leftHighMidKnob.setRange(-24.0f, 24.0f, 0.1f);
    leftHighMidKnob.setValue(0.0f);
    leftHighMidKnob.setTextValueSuffix(" dB");
    addAndMakeVisible(leftHighMidKnob);

    leftHighKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftHighKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    leftHighKnob.setRange(-24.0f, 24.0f, 0.1f);
    leftHighKnob.setValue(0.0f);
    leftHighKnob.setTextValueSuffix(" dB");
    addAndMakeVisible(leftHighKnob);

    // Set up right channel knobs
    rightLowKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightLowKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    rightLowKnob.setRange(-24.0f, 24.0f, 0.1f);
    rightLowKnob.setValue(0.0f);
    rightLowKnob.setTextValueSuffix(" dB");
    addAndMakeVisible(rightLowKnob);

    rightLowMidKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightLowMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    rightLowMidKnob.setRange(-24.0f, 24.0f, 0.1f);
    rightLowMidKnob.setValue(0.0f);
    rightLowMidKnob.setTextValueSuffix(" dB");
    addAndMakeVisible(rightLowMidKnob);

    rightHighMidKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightHighMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    rightHighMidKnob.setRange(-24.0f, 24.0f, 0.1f);
    rightHighMidKnob.setValue(0.0f);
    rightHighMidKnob.setTextValueSuffix(" dB");
    addAndMakeVisible(rightHighMidKnob);

    rightHighKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightHighKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    rightHighKnob.setRange(-24.0f, 24.0f, 0.1f);
    rightHighKnob.setValue(0.0f);
    rightHighKnob.setTextValueSuffix(" dB");
    addAndMakeVisible(rightHighKnob);

    // Set up mix knob
    mixKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    mixKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    mixKnob.setRange(0.0f, 100.0f, 1.0f);
    mixKnob.setValue(100.0f);
    mixKnob.setTextValueSuffix(" %");
    addAndMakeVisible(mixKnob);

    // Set up sync button
    syncButton.setButtonText("Sync");
    syncButton.setClickingTogglesState(true);
    addAndMakeVisible(syncButton);

    // Create parameter attachments
    leftLowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "leftLow", leftLowKnob);
    leftLowMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "leftLowMid", leftLowMidKnob);
    leftHighMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "leftHighMid", leftHighMidKnob);
    leftHighAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "leftHigh", leftHighKnob);

    rightLowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "rightLow", rightLowKnob);
    rightLowMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "rightLowMid", rightLowMidKnob);
    rightHighMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "rightHighMid", rightHighMidKnob);
    rightHighAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "rightHigh", rightHighKnob);

    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.apvts, "mix", mixKnob);

    syncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processorRef.apvts, "sync", syncButton);

    // Add listener for parameter changes
    processorRef.apvts.addParameterListener("sync", this);

    setSize(800, 600);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {
}

void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g) {
    // Draw background
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    // Set up text style
    g.setColour(juce::Colours::white);
    g.setFont(16.0f);

    // Draw channel labels
    g.drawText("Left Channel", 0, 20, getWidth()/2, 30, juce::Justification::centred);
    g.drawText("Right Channel", getWidth()/2, 20, getWidth()/2, 30, juce::Justification::centred);

    // Draw frequency band labels for left channel
    g.setFont(14.0f);
    g.drawFittedText("Low", leftLowKnob.getBounds().reduced(10), juce::Justification::centred, 1);
    g.drawFittedText("Low-Mid", leftLowMidKnob.getBounds().reduced(10), juce::Justification::centred, 1);
    g.drawFittedText("High-Mid", leftHighMidKnob.getBounds().reduced(10), juce::Justification::centred, 1);
    g.drawFittedText("High", leftHighKnob.getBounds().reduced(10), juce::Justification::centred, 1);

    // Draw frequency band labels for right channel
    g.drawFittedText("Low", rightLowKnob.getBounds().reduced(10), juce::Justification::centred, 1);
    g.drawFittedText("Low-Mid", rightLowMidKnob.getBounds().reduced(10), juce::Justification::centred, 1);
    g.drawFittedText("High-Mid", rightHighMidKnob.getBounds().reduced(10), juce::Justification::centred, 1);
    g.drawFittedText("High", rightHighKnob.getBounds().reduced(10), juce::Justification::centred, 1);

    // Draw mix and sync labels
    g.setFont(16.0f);
    g.drawText("Mix", mixKnob.getBounds().reduced(10), juce::Justification::centred);
    g.drawText("Sync Channels", syncButton.getBounds().reduced(10), juce::Justification::centred);
}

void AudioPluginAudioProcessorEditor::resized() {
    // Define common sizes
    const int knobSize = 120;
    const int knobSpacing = 20;
    const int labelHeight = 20;
    const int buttonHeight = 30;
    const int buttonWidth = 100;

    // Calculate total width needed for each channel
    const int channelWidth = (knobSize * 4) + (knobSpacing * 3);
    
    // Calculate starting positions
    const int leftStartX = 40;
    const int rightStartX = leftStartX + channelWidth + 100; // 100px gap between channels
    const int startY = 60; // Start below the channel labels

    // Position left channel knobs
    leftLowKnob.setBounds(leftStartX, startY, knobSize, knobSize);
    leftLowMidKnob.setBounds(leftStartX + knobSize + knobSpacing, startY, knobSize, knobSize);
    leftHighMidKnob.setBounds(leftStartX + (knobSize + knobSpacing) * 2, startY, knobSize, knobSize);
    leftHighKnob.setBounds(leftStartX + (knobSize + knobSpacing) * 3, startY, knobSize, knobSize);

    // Position right channel knobs
    rightLowKnob.setBounds(rightStartX, startY, knobSize, knobSize);
    rightLowMidKnob.setBounds(rightStartX + knobSize + knobSpacing, startY, knobSize, knobSize);
    rightHighMidKnob.setBounds(rightStartX + (knobSize + knobSpacing) * 2, startY, knobSize, knobSize);
    rightHighKnob.setBounds(rightStartX + (knobSize + knobSpacing) * 3, startY, knobSize, knobSize);

    // Position mix knob and sync button
    const int bottomY = startY + knobSize + labelHeight + 20; // Use labelHeight for spacing
    const int centerX = (leftStartX + rightStartX + channelWidth) / 2;
    
    mixKnob.setBounds(centerX - knobSize/2, bottomY, knobSize, knobSize);
    syncButton.setBounds(centerX - buttonWidth/2, bottomY + knobSize + labelHeight, buttonWidth, buttonHeight);

    // Update window size to fit all components
    setSize(rightStartX + channelWidth + 40, bottomY + knobSize + buttonHeight + labelHeight + 40);
}

void AudioPluginAudioProcessorEditor::parameterChanged(const juce::String& parameterID, float newValue) {
  if (parameterID == "sync") {
    isSynced = newValue > 0.5f;
    if (isSynced) {
      // Update all knobs to match the last changed values
      leftLowKnob.setValue(processorRef.getLastLowGain(), juce::dontSendNotification);
      leftLowMidKnob.setValue(processorRef.getLastLowMidGain(), juce::dontSendNotification);
      leftHighMidKnob.setValue(processorRef.getLastHighMidGain(), juce::dontSendNotification);
      leftHighKnob.setValue(processorRef.getLastHighGain(), juce::dontSendNotification);

      rightLowKnob.setValue(processorRef.getLastLowGain(), juce::dontSendNotification);
      rightLowMidKnob.setValue(processorRef.getLastLowMidGain(), juce::dontSendNotification);
      rightHighMidKnob.setValue(processorRef.getLastHighMidGain(), juce::dontSendNotification);
      rightHighKnob.setValue(processorRef.getLastHighGain(), juce::dontSendNotification);
    }
  }
}
}  // namespace audio_plugin
