#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>
#include "LightToneMS18x/SubharmonicProcessor.h"

namespace audio_plugin {
class AudioPluginAudioProcessor : public juce::AudioProcessor {
public:
  AudioPluginAudioProcessor();
  ~AudioPluginAudioProcessor() override;

  void prepareToPlay(double sampleRate, int samplesPerBlock) override;
  void releaseResources() override;

  bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

  void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
  using AudioProcessor::processBlock;

  juce::AudioProcessorEditor* createEditor() override;
  bool hasEditor() const override;

  const juce::String getName() const override;

  bool acceptsMidi() const override;
  bool producesMidi() const override;
  bool isMidiEffect() const override;
  double getTailLengthSeconds() const override;

  int getNumPrograms() override;
  int getCurrentProgram() override;
  void setCurrentProgram(int index) override;
  const juce::String getProgramName(int index) override;
  void changeProgramName(int index, const juce::String& newName) override;

  void getStateInformation(juce::MemoryBlock& destData) override;
  void setStateInformation(const void* data, int sizeInBytes) override;

  // Public getters for last gain values
  float getLastLowGain() const { return lastLowGain; }
  float getLastLowMidGain() const { return lastLowMidGain; }
  float getLastHighMidGain() const { return lastHighMidGain; }
  float getLastHighGain() const { return lastHighGain; }

  juce::AudioProcessorValueTreeState apvts;

private:
  // Left channel filters
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> leftLowFilter;
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> leftLowMidFilter;
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> leftHighMidFilter;
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> leftHighFilter;

  // Right channel filters
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> rightLowFilter;
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> rightLowMidFilter;
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> rightHighMidFilter;
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> rightHighFilter;

  // Sync parameter
  std::atomic<bool> isSynced{false};

  // Track last changed values for each frequency band
  float lastLowGain{0.0f};
  float lastLowMidGain{0.0f};
  float lastHighMidGain{0.0f};
  float lastHighGain{0.0f};

  // Random number generators for different noise types
  juce::Random pinkNoiseRandom;
  juce::Random darkNoiseRandom;
  float pinkNoiseBuffer[2] = {0.0f, 0.0f};
  float darkNoiseBuffer[2] = {0.0f, 0.0f};

  // Helper functions for noise generation
  float generatePinkNoise();
  float generateDarkNoise();

  juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();

  // Compressor-like parameters
  float attackTime = 0.005f;  // 5ms attack
  float releaseTime = 0.1f;   // 100ms release
  float threshold = -30.0f;   // -30dB threshold
  float ratio = 4.0f;         // 4:1 ratio
  float knee = 6.0f;          // 6dB knee

  // Envelope follower state
  float envelope = 0.0f;
  float sampleRate = 44100.0f;

  // Noise generation
  float b0, b1, b2, b3, b4, b5, b6;  // Pink noise state
  float lastDarkNoise = 0.0f;        // Dark noise state

  // Subharmonics
  SubharmonicProcessor subProcessor[2];
  juce::AudioBuffer<float> subBuffer;
  juce::dsp::ProcessorDuplicator<juce::dsp::IIR::Filter<float>, juce::dsp::IIR::Coefficients<float>> subLowPassFilter[2];
  float subAmount = 0.5f;
  bool subEnabled = false;

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessor)
};
}  // namespace audio_plugin
