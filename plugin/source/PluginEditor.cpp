#include "LightToneMS18x/PluginEditor.h"
#include "LightToneMS18x/PluginProcessor.h"

namespace audio_plugin {

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p), apvts(p.apvts) {
    // Set up left channel knobs
    leftLowKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftLowKnob.setRange(-24.0f, 24.0f, 0.1f);
    leftLowKnob.setValue(0.0f);
    leftLowKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    leftLowKnob.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    leftLowKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    leftLowKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    addAndMakeVisible(leftLowKnob);

    leftLowMidKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftLowMidKnob.setRange(-24.0f, 24.0f, 0.1f);
    leftLowMidKnob.setValue(0.0f);
    leftLowMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    leftLowMidKnob.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    leftLowMidKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    leftLowMidKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    addAndMakeVisible(leftLowMidKnob);

    leftHighMidKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftHighMidKnob.setRange(-24.0f, 24.0f, 0.1f);
    leftHighMidKnob.setValue(0.0f);
    leftHighMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    leftHighMidKnob.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    leftHighMidKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    leftHighMidKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    addAndMakeVisible(leftHighMidKnob);

    leftHighKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    leftHighKnob.setRange(-24.0f, 24.0f, 0.1f);
    leftHighKnob.setValue(0.0f);
    leftHighKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    leftHighKnob.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    leftHighKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    leftHighKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    addAndMakeVisible(leftHighKnob);

    // Set up right channel knobs
    rightLowKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightLowKnob.setRange(-24.0f, 24.0f, 0.1f);
    rightLowKnob.setValue(0.0f);
    rightLowKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    rightLowKnob.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    rightLowKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    rightLowKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    addAndMakeVisible(rightLowKnob);

    rightLowMidKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightLowMidKnob.setRange(-24.0f, 24.0f, 0.1f);
    rightLowMidKnob.setValue(0.0f);
    rightLowMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    rightLowMidKnob.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    rightLowMidKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    rightLowMidKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    addAndMakeVisible(rightLowMidKnob);

    rightHighMidKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightHighMidKnob.setRange(-24.0f, 24.0f, 0.1f);
    rightHighMidKnob.setValue(0.0f);
    rightHighMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    rightHighMidKnob.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    rightHighMidKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    rightHighMidKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    addAndMakeVisible(rightHighMidKnob);

    rightHighKnob.setSliderStyle(juce::Slider::RotaryHorizontalVerticalDrag);
    rightHighKnob.setRange(-24.0f, 24.0f, 0.1f);
    rightHighKnob.setValue(0.0f);
    rightHighKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
    rightHighKnob.setColour(juce::Slider::thumbColourId, juce::Colours::white);
    rightHighKnob.setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::white);
    rightHighKnob.setColour(juce::Slider::rotarySliderOutlineColourId, juce::Colours::white);
    addAndMakeVisible(rightHighKnob);

    // Set up mix knob
    mixKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    mixKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    mixKnob.setRange(0.0, 100.0, 1.0);
    mixKnob.setValue(100.0);
    addAndMakeVisible(mixKnob);
    mixLabel.setText("Mix", juce::dontSendNotification);
    mixLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(mixLabel);
    
    // Set up sub mix knob
    subMixKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    subMixKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    subMixKnob.setRange(0.0, 1.0, 0.01);
    subMixKnob.setValue(0.5);
    addAndMakeVisible(subMixKnob);
    subMixLabel.setText("Sub Mix", juce::dontSendNotification);
    subMixLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(subMixLabel);
    
    // Set up sub filter knob
    subFilterKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
    subFilterKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 70, 20);
    subFilterKnob.setRange(20.0, 160.0, 1.0);
    subFilterKnob.setValue(80.0);
    addAndMakeVisible(subFilterKnob);
    subFilterLabel.setText("Sub Filter", juce::dontSendNotification);
    subFilterLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(subFilterLabel);

    // Set up sync button
    syncButton.setButtonText("Sync");
    syncButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    syncButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::white);
    syncButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    syncButton.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    syncButton.setClickingTogglesState(true);
    addAndMakeVisible(syncButton);

    // Set up noise button
    noiseButton.setButtonText("Noise");
    noiseButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    noiseButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::white);
    noiseButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    noiseButton.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    noiseButton.setClickingTogglesState(true);
    noiseButton.setToggleState(true, juce::dontSendNotification);

    // Set up bypass button
    bypassButton.setButtonText("Bypass");
    bypassButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    bypassButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::red);
    bypassButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    bypassButton.setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    bypassButton.setClickingTogglesState(true);
    bypassButton.setToggleState(false, juce::dontSendNotification);
    addAndMakeVisible(bypassButton);

    // Set up subharmonics button
    subButton.setButtonText("Sub");
    subButton.setColour(juce::TextButton::buttonColourId, juce::Colours::darkgrey);
    subButton.setColour(juce::TextButton::buttonOnColourId, juce::Colours::white);
    subButton.setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    subButton.setColour(juce::TextButton::textColourOnId, juce::Colours::black);
    subButton.setClickingTogglesState(true);
    subButton.setToggleState(false, juce::dontSendNotification);
    addAndMakeVisible(subButton);

    // Create parameter attachments
    leftLowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "leftLow", leftLowKnob);
    leftLowMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "leftLowMid", leftLowMidKnob);
    leftHighMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "leftHighMid", leftHighMidKnob);
    leftHighAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "leftHigh", leftHighKnob);

    rightLowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "rightLow", rightLowKnob);
    rightLowMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "rightLowMid", rightLowMidKnob);
    rightHighMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "rightHighMid", rightHighMidKnob);
    rightHighAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "rightHigh", rightHighKnob);

    mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "mix", mixKnob);
    subMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "subMix", subMixKnob);
    subFilterAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(apvts, "subFilter", subFilterKnob);
    syncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "sync", syncButton);
    noiseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "noiseEnabled", noiseButton);
    subAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "subEnabled", subButton);
    bypassAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(apvts, "bypass", bypassButton);

    // Add listeners
    syncButton.addListener(this);
    noiseButton.addListener(this);
    subButton.addListener(this);
    bypassButton.addListener(this);
    apvts.addParameterListener("sync", this);
    apvts.addParameterListener("noiseEnabled", this);
    apvts.addParameterListener("subEnabled", this);

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

    // Position mix knob and buttons
    const int bottomY = startY + knobSize + labelHeight + 20; // Use labelHeight for spacing
    const int centerX = (leftStartX + rightStartX + channelWidth) / 2;
    
    // Position mix knob centered
    mixKnob.setBounds(centerX - knobSize/2, bottomY, knobSize, knobSize);
    mixLabel.setBounds(centerX - knobSize/2, bottomY + knobSize, knobSize, labelHeight);
    
    // Position sub knobs side by side below the mix knob
    const int smallKnobSize = 80;
    const int subKnobsY = bottomY + knobSize + labelHeight + 10;
    subMixKnob.setBounds(centerX - smallKnobSize - 10, subKnobsY, smallKnobSize, smallKnobSize);
    subMixLabel.setBounds(centerX - smallKnobSize - 10, subKnobsY + smallKnobSize, smallKnobSize, labelHeight);
    
    subFilterKnob.setBounds(centerX + 10, subKnobsY, smallKnobSize, smallKnobSize);
    subFilterLabel.setBounds(centerX + 10, subKnobsY + smallKnobSize, smallKnobSize, labelHeight);
    
    // Position buttons below sub knobs
    const int buttonsY = subKnobsY + smallKnobSize + labelHeight + 10;
    syncButton.setBounds(centerX - buttonWidth - 5, buttonsY, buttonWidth, buttonHeight);
    noiseButton.setBounds(centerX + 5, buttonsY, buttonWidth, buttonHeight);
    subButton.setBounds(centerX - buttonWidth/2, buttonsY + buttonHeight + 10, buttonWidth, buttonHeight);
    bypassButton.setBounds(centerX - buttonWidth/2, buttonsY + buttonHeight * 2 + 20, buttonWidth, buttonHeight);

    // Update window size to fit all components
    setSize(rightStartX + channelWidth + 40, buttonsY + buttonHeight * 3 + 50);
}

void AudioPluginAudioProcessorEditor::buttonClicked(juce::Button* button) {
    if (button == &syncButton) {
        auto syncParam = processorRef.apvts.getParameter("sync");
        syncParam->setValueNotifyingHost(button->getToggleState() ? 1.0f : 0.0f);
    } else if (button == &noiseButton) {
        auto noiseParam = processorRef.apvts.getParameter("noiseEnabled");
        noiseParam->setValueNotifyingHost(button->getToggleState() ? 1.0f : 0.0f);
    } else if (button == &subButton) {
        auto subParam = processorRef.apvts.getParameter("subEnabled");
        subParam->setValueNotifyingHost(button->getToggleState() ? 1.0f : 0.0f);
    } else if (button == &bypassButton) {
        auto bypassParam = processorRef.apvts.getParameter("bypass");
        bypassParam->setValueNotifyingHost(button->getToggleState() ? 1.0f : 0.0f);
    }
}

void AudioPluginAudioProcessorEditor::parameterChanged(const juce::String& parameterID, float newValue) {
    if (parameterID == "sync") {
        syncButton.setToggleState(newValue > 0.5f, juce::dontSendNotification);
    } else if (parameterID == "noiseEnabled") {
        noiseButton.setToggleState(newValue > 0.5f, juce::dontSendNotification);
    } else if (parameterID == "subEnabled") {
        subButton.setToggleState(newValue > 0.5f, juce::dontSendNotification);
    } else if (parameterID == "bypass") {
        bypassButton.setToggleState(newValue > 0.5f, juce::dontSendNotification);
    }
}
}  // namespace audio_plugin
