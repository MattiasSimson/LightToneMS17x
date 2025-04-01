#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <juce_gui_extra/juce_gui_extra.h>
#include "LightToneMS18x/PluginProcessor.h"

namespace audio_plugin {

class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor,
                                      public juce::Button::Listener,
                                      public juce::AudioProcessorValueTreeState::Listener {
public:
  explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
  ~AudioPluginAudioProcessorEditor() override;

  void paint(juce::Graphics&) override;
  void resized() override;
  void buttonClicked(juce::Button* button) override;
  void parameterChanged(const juce::String& parameterID, float newValue) override;

private:
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  AudioPluginAudioProcessor& processorRef;
  juce::AudioProcessorValueTreeState& apvts;

  // Left channel knobs
  juce::Slider leftLowKnob;
  juce::Slider leftLowMidKnob;
  juce::Slider leftHighMidKnob;
  juce::Slider leftHighKnob;
  juce::Label leftLowLabel;
  juce::Label leftLowMidLabel;
  juce::Label leftHighMidLabel;
  juce::Label leftHighLabel;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> leftLowAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> leftLowMidAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> leftHighMidAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> leftHighAttachment;

  // Right channel knobs
  juce::Slider rightLowKnob;
  juce::Slider rightLowMidKnob;
  juce::Slider rightHighMidKnob;
  juce::Slider rightHighKnob;
  juce::Label rightLowLabel;
  juce::Label rightLowMidLabel;
  juce::Label rightHighMidLabel;
  juce::Label rightHighLabel;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rightLowAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rightLowMidAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rightHighMidAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> rightHighAttachment;

  // Mix knob
  juce::Slider mixKnob;
  juce::Label mixLabel;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;

  // Sub knobs
  juce::Slider subMixKnob;
  juce::Slider subFilterKnob;
  juce::Label subMixLabel;
  juce::Label subFilterLabel;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> subMixAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> subFilterAttachment;

  // Sync button
  juce::TextButton syncButton{"Sync"};
  juce::TextButton noiseButton{"Noise"};
  juce::TextButton subButton{"Sub"};
  juce::TextButton bypassButton{"Bypass"};
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> syncAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> noiseAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> subAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> bypassAttachment;
  bool isSynced = false;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
}  // namespace audio_plugin
