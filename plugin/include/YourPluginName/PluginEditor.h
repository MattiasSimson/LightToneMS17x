#pragma once

#include "YourPluginName/PluginProcessor.h"

namespace audio_plugin {

class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor {
public:
  explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
  ~AudioPluginAudioProcessorEditor() override;

  void paint(juce::Graphics&) override;
  void resized() override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  AudioPluginAudioProcessor& processorRef;

  // Left channel knobs
  juce::Slider leftLowKnob;
  juce::Slider leftLowMidKnob;
  juce::Slider leftHighMidKnob;
  juce::Slider leftHighKnob;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> leftLowAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> leftLowMidAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> leftHighMidAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> leftHighAttachment;

  // Right channel knobs
  juce::Slider rightLowKnob;
  juce::Slider rightLowMidKnob;
  juce::Slider rightHighMidKnob;
  juce::Slider rightHighKnob;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rightLowAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rightLowMidAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rightHighMidAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rightHighAttachment;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
}  // namespace audio_plugin
