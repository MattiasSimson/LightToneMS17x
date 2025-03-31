#include "YourPluginName/PluginEditor.h"
#include "YourPluginName/PluginProcessor.h"

namespace audio_plugin {
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p) {
  // Set up left channel knobs
  leftLowKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  leftLowKnob.setRange(-24.0f, 24.0f, 0.1f);
  leftLowKnob.setValue(0.0f);
  leftLowKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
  addAndMakeVisible(leftLowKnob);

  leftLowMidKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  leftLowMidKnob.setRange(-24.0f, 24.0f, 0.1f);
  leftLowMidKnob.setValue(0.0f);
  leftLowMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
  addAndMakeVisible(leftLowMidKnob);

  leftHighMidKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  leftHighMidKnob.setRange(-24.0f, 24.0f, 0.1f);
  leftHighMidKnob.setValue(0.0f);
  leftHighMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
  addAndMakeVisible(leftHighMidKnob);

  leftHighKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  leftHighKnob.setRange(-24.0f, 24.0f, 0.1f);
  leftHighKnob.setValue(0.0f);
  leftHighKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
  addAndMakeVisible(leftHighKnob);

  // Set up right channel knobs
  rightLowKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  rightLowKnob.setRange(-24.0f, 24.0f, 0.1f);
  rightLowKnob.setValue(0.0f);
  rightLowKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
  addAndMakeVisible(rightLowKnob);

  rightLowMidKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  rightLowMidKnob.setRange(-24.0f, 24.0f, 0.1f);
  rightLowMidKnob.setValue(0.0f);
  rightLowMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
  addAndMakeVisible(rightLowMidKnob);

  rightHighMidKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  rightHighMidKnob.setRange(-24.0f, 24.0f, 0.1f);
  rightHighMidKnob.setValue(0.0f);
  rightHighMidKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
  addAndMakeVisible(rightHighMidKnob);

  rightHighKnob.setSliderStyle(juce::Slider::RotaryVerticalDrag);
  rightHighKnob.setRange(-24.0f, 24.0f, 0.1f);
  rightHighKnob.setValue(0.0f);
  rightHighKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 20);
  addAndMakeVisible(rightHighKnob);

  // Set up knob attachments
  leftLowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "leftLow", leftLowKnob);
  leftLowMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "leftLowMid", leftLowMidKnob);
  leftHighMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "leftHighMid", leftHighMidKnob);
  leftHighAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "leftHigh", leftHighKnob);

  rightLowAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "rightLow", rightLowKnob);
  rightLowMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "rightLowMid", rightLowMidKnob);
  rightHighMidAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "rightHighMid", rightHighMidKnob);
  rightHighAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processorRef.apvts, "rightHigh", rightHighKnob);

  setSize(800, 400);
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() {}

void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g) {
  g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

  g.setColour(juce::Colours::white);
  g.setFont(15.0f);

  // Draw channel labels
  g.drawText("Left Channel", getWidth() / 4 - 50, 20, 100, 20, juce::Justification::centred);
  g.drawText("Right Channel", 3 * getWidth() / 4 - 50, 20, 100, 20, juce::Justification::centred);

  // Draw frequency band labels for left channel
  g.drawFittedText("Low", leftLowKnob.getBounds().reduced(10), juce::Justification::centred, 1);
  g.drawFittedText("Low-Mid", leftLowMidKnob.getBounds().reduced(10), juce::Justification::centred, 1);
  g.drawFittedText("High-Mid", leftHighMidKnob.getBounds().reduced(10), juce::Justification::centred, 1);
  g.drawFittedText("High", leftHighKnob.getBounds().reduced(10), juce::Justification::centred, 1);

  // Draw frequency band labels for right channel
  g.drawFittedText("Low", rightLowKnob.getBounds().reduced(10), juce::Justification::centred, 1);
  g.drawFittedText("Low-Mid", rightLowMidKnob.getBounds().reduced(10), juce::Justification::centred, 1);
  g.drawFittedText("High-Mid", rightHighMidKnob.getBounds().reduced(10), juce::Justification::centred, 1);
  g.drawFittedText("High", rightHighKnob.getBounds().reduced(10), juce::Justification::centred, 1);
}

void AudioPluginAudioProcessorEditor::resized() {
  auto area = getLocalBounds();
  auto knobWidth = area.getWidth() / 8;
  auto knobHeight = area.getHeight() / 2;

  // Position left channel knobs
  leftLowKnob.setBounds(area.removeFromLeft(knobWidth).removeFromTop(knobHeight));
  leftLowMidKnob.setBounds(area.removeFromLeft(knobWidth).removeFromTop(knobHeight));
  leftHighMidKnob.setBounds(area.removeFromLeft(knobWidth).removeFromTop(knobHeight));
  leftHighKnob.setBounds(area.removeFromLeft(knobWidth).removeFromTop(knobHeight));

  // Reset area for right channel
  area = getLocalBounds();
  area.removeFromLeft(knobWidth * 4); // Skip left channel area

  // Position right channel knobs
  rightLowKnob.setBounds(area.removeFromLeft(knobWidth).removeFromTop(knobHeight));
  rightLowMidKnob.setBounds(area.removeFromLeft(knobWidth).removeFromTop(knobHeight));
  rightHighMidKnob.setBounds(area.removeFromLeft(knobWidth).removeFromTop(knobHeight));
  rightHighKnob.setBounds(area.removeFromLeft(knobWidth).removeFromTop(knobHeight));
}
}  // namespace audio_plugin
