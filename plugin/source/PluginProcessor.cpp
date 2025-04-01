#include "LightToneMS18x/PluginProcessor.h"
#include "LightToneMS18x/PluginEditor.h"
#include <cmath>

namespace audio_plugin {

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
    // Bypass parameter
    layout.add(std::make_unique<juce::AudioParameterBool>("bypass", "Bypass", false));

    // Left channel parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>("leftLow", "Left Low", -24.0f, 24.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("leftLowMid", "Left Low-Mid", -24.0f, 24.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("leftHighMid", "Left High-Mid", -24.0f, 24.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("leftHigh", "Left High", -24.0f, 24.0f, 0.0f));

    // Right channel parameters
    layout.add(std::make_unique<juce::AudioParameterFloat>("rightLow", "Right Low", -24.0f, 24.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("rightLowMid", "Right Low-Mid", -24.0f, 24.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("rightHighMid", "Right High-Mid", -24.0f, 24.0f, 0.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("rightHigh", "Right High", -24.0f, 24.0f, 0.0f));

    // Sync parameter
    layout.add(std::make_unique<juce::AudioParameterBool>("sync", "Sync", false));

    // Wet/Dry mix parameter
    layout.add(std::make_unique<juce::AudioParameterFloat>("mix", "Mix", 0.0f, 100.0f, 100.0f));

    // Noise toggle parameter
    layout.add(std::make_unique<juce::AudioParameterBool>("noiseEnabled", "Noise", true));
    
    // Subharmonics parameters
    layout.add(std::make_unique<juce::AudioParameterBool>("subEnabled", "Subs", true));
    layout.add(std::make_unique<juce::AudioParameterFloat>("subAmount", "Sub Amount", 0.0f, 1.0f, 0.7f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("subMix", "Sub Mix", 0.0f, 1.0f, 0.5f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("subFilter", "Sub Filter", 20.0f, 160.0f, 80.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("subAttack", "Sub Attack", 1.0f, 100.0f, 5.0f));
    layout.add(std::make_unique<juce::AudioParameterFloat>("subRelease", "Sub Release", 10.0f, 500.0f, 150.0f));

    return layout;
}

AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
                        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
      apvts(*this, nullptr, "Parameters", createParameterLayout()) {
}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() {}

const juce::String AudioPluginAudioProcessor::getName() const {
    return "LightTone MS-18x";
}

bool AudioPluginAudioProcessor::acceptsMidi() const {
  return false;
}

bool AudioPluginAudioProcessor::producesMidi() const {
  return false;
}

bool AudioPluginAudioProcessor::isMidiEffect() const {
  return false;
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms() {
    return 1;
}

int AudioPluginAudioProcessor::getCurrentProgram() {
  return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index, const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void AudioPluginAudioProcessor::prepareToPlay(double newSampleRate, int samplesPerBlock) {
    // Initialize filters
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = newSampleRate;
    spec.maximumBlockSize = static_cast<juce::uint32>(samplesPerBlock);
    spec.numChannels = 2; // Process both channels

    // Initialize left channel filters
    leftLowFilter.prepare(spec);
    leftLowMidFilter.prepare(spec);
    leftHighMidFilter.prepare(spec);
    leftHighFilter.prepare(spec);

    // Initialize right channel filters
    rightLowFilter.prepare(spec);
    rightLowMidFilter.prepare(spec);
    rightHighMidFilter.prepare(spec);
    rightHighFilter.prepare(spec);

    // Set up filter coefficients with initial unity gain
    *leftLowFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(newSampleRate, 100.0f, 0.7f, 1.0f);
    *leftLowMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(newSampleRate, 400.0f, 0.7f, 1.0f);
    *leftHighMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(newSampleRate, 2000.0f, 0.7f, 1.0f);
    *leftHighFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(newSampleRate, 8000.0f, 0.7f, 1.0f);

    *rightLowFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(newSampleRate, 100.0f, 0.7f, 1.0f);
    *rightLowMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(newSampleRate, 400.0f, 0.7f, 1.0f);
    *rightHighMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(newSampleRate, 2000.0f, 0.7f, 1.0f);
    *rightHighFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(newSampleRate, 8000.0f, 0.7f, 1.0f);

    // Initialize subharmonic processors
    for (int ch = 0; ch < 2; ++ch)
    {
        subProcessor[ch].reset(static_cast<float>(newSampleRate));
        subProcessor[ch].setDetector(10.0f, 100.0f);
        
        subLowPassFilter[ch].prepare({ newSampleRate, static_cast<juce::uint32>(samplesPerBlock), 1 });
        *subLowPassFilter[ch].state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(
            newSampleRate, 80.0f, 0.7071f);
    }
    
    subBuffer.setSize(2, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources() {
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const {
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;

  return true;
}

float AudioPluginAudioProcessor::generatePinkNoise() {
    // Simple pink noise implementation using Voss-McCartney algorithm
    static const int numPoles = 4;
    static float poleB0[numPoles] = {0.0f, 0.0f, 0.0f, 0.0f};
    static float poleB1[numPoles] = {0.0f, 0.0f, 0.0f, 0.0f};
    static float poleP[numPoles] = {0.0f, 0.0f, 0.0f, 0.0f};
    
    float white = pinkNoiseRandom.nextFloat() * 2.0f - 1.0f;
    float pink = 0.0f;
    
    for (int i = 0; i < numPoles; i++) {
        poleB0[i] = 0.997f * poleB0[i] + 0.029591f * white;
        poleB1[i] = 0.985f * poleB1[i] + 0.032176f * white;
        poleP[i] = 0.99f * poleP[i] + 0.01f * (poleB0[i] - poleB1[i]);
        pink += poleP[i];
    }
    
    return pink * 0.5f; // Scale down to prevent clipping
}

float AudioPluginAudioProcessor::generateDarkNoise() {
    // Generate darker noise by filtering white noise
    static float lastOutput = 0.0f;
    float white = darkNoiseRandom.nextFloat() * 2.0f - 1.0f;
    lastOutput = 0.95f * lastOutput + 0.05f * white;
    return lastOutput * 0.5f; // Scale down to prevent clipping
}

// Helper function to add random deviation to a frequency
float addRandomDeviation(float baseFreq, float deviationPercent) {
    // Create a random deviation between -deviationPercent and +deviationPercent
    float deviation = (juce::Random::getSystemRandom().nextFloat() * 2.0f - 1.0f) * (baseFreq * deviationPercent);
    return baseFreq + deviation;
}

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer& midiMessages) {
  juce::ignoreUnused(midiMessages);
  juce::ScopedNoDenormals noDenormals;
  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Check bypass state
    bool isBypassed = apvts.getRawParameterValue("bypass")->load() > 0.5f;
    if (isBypassed) {
        // When bypassed, just pass through the input
        return;
    }

    // Clear any unused output channels
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
    buffer.clear(i, 0, buffer.getNumSamples());

    // Get parameter values
    float leftLowGain = apvts.getRawParameterValue("leftLow")->load();
    float leftLowMidGain = apvts.getRawParameterValue("leftLowMid")->load();
    float leftHighMidGain = apvts.getRawParameterValue("leftHighMid")->load();
    float leftHighGain = apvts.getRawParameterValue("leftHigh")->load();

    float rightLowGain = apvts.getRawParameterValue("rightLow")->load();
    float rightLowMidGain = apvts.getRawParameterValue("rightLowMid")->load();
    float rightHighMidGain = apvts.getRawParameterValue("rightHighMid")->load();
    float rightHighGain = apvts.getRawParameterValue("rightHigh")->load();

    float mix = apvts.getRawParameterValue("mix")->load() / 100.0f;
    bool sync = apvts.getRawParameterValue("sync")->load() > 0.5f;
    bool noiseEnabled = apvts.getRawParameterValue("noiseEnabled")->load() > 0.5f;
    
    // Subharmonics parameters
    bool isSubEnabled = apvts.getRawParameterValue("subEnabled")->load() > 0.5f;
    float subAmountValue = apvts.getRawParameterValue("subAmount")->load();
    float subMixValue = apvts.getRawParameterValue("subMix")->load();
    float subFilterValue = apvts.getRawParameterValue("subFilter")->load();
    float subAttack = apvts.getRawParameterValue("subAttack")->load();
    float subRelease = apvts.getRawParameterValue("subRelease")->load();
    
    // Update subharmonic settings
    for (int ch = 0; ch < 2; ++ch)
    {
        subProcessor[ch].setDetector(subAttack, subRelease);
        *subLowPassFilter[ch].state = *juce::dsp::IIR::Coefficients<float>::makeLowPass(
            getSampleRate(), subFilterValue, 0.7071f);
    }

    // If sync is enabled, use the last changed value for both channels
    if (sync) {
        // Update last changed values if either channel changed
        if (leftLowGain != rightLowGain) {
            lastLowGain = (leftLowGain != lastLowGain) ? leftLowGain : rightLowGain;
        }
        if (leftLowMidGain != rightLowMidGain) {
            lastLowMidGain = (leftLowMidGain != lastLowMidGain) ? leftLowMidGain : rightLowMidGain;
        }
        if (leftHighMidGain != rightHighMidGain) {
            lastHighMidGain = (leftHighMidGain != lastHighMidGain) ? leftHighMidGain : rightHighMidGain;
        }
        if (leftHighGain != rightHighGain) {
            lastHighGain = (leftHighGain != lastHighGain) ? leftHighGain : rightHighGain;
        }

        // Apply the last changed values to both channels
        leftLowGain = rightLowGain = lastLowGain;
        leftLowMidGain = rightLowMidGain = lastLowMidGain;
        leftHighMidGain = rightHighMidGain = lastHighMidGain;
        leftHighGain = rightHighGain = lastHighGain;

        // Update the parameter values to ensure the UI reflects the changes
        apvts.getParameter("leftLow")->setValueNotifyingHost(apvts.getParameterRange("leftLow").convertTo0to1(lastLowGain));
        apvts.getParameter("leftLowMid")->setValueNotifyingHost(apvts.getParameterRange("leftLowMid").convertTo0to1(lastLowMidGain));
        apvts.getParameter("leftHighMid")->setValueNotifyingHost(apvts.getParameterRange("leftHighMid").convertTo0to1(lastHighMidGain));
        apvts.getParameter("leftHigh")->setValueNotifyingHost(apvts.getParameterRange("leftHigh").convertTo0to1(lastHighGain));

        apvts.getParameter("rightLow")->setValueNotifyingHost(apvts.getParameterRange("rightLow").convertTo0to1(lastLowGain));
        apvts.getParameter("rightLowMid")->setValueNotifyingHost(apvts.getParameterRange("rightLowMid").convertTo0to1(lastLowMidGain));
        apvts.getParameter("rightHighMid")->setValueNotifyingHost(apvts.getParameterRange("rightHighMid").convertTo0to1(lastHighMidGain));
        apvts.getParameter("rightHigh")->setValueNotifyingHost(apvts.getParameterRange("rightHigh").convertTo0to1(lastHighGain));
    }

    // Base frequencies
    const float lowFreq = 80.0f;
    const float lowMidFreq = 300.0f;
    const float highMidFreq = 2500.0f;
    const float highFreq = 10000.0f;
    
    // Maximum deviation percentage (0.02 = 2%)
    const float maxDeviation = 0.02f;

    // Create a copy of the input for subharmonics processing
    subBuffer.makeCopyOf(buffer, true);
    
    // Process each channel
  for (int channel = 0; channel < totalNumInputChannels; ++channel) {
    auto* channelData = buffer.getWritePointer(channel);
        auto* subData = subBuffer.getWritePointer(channel);
        auto numSamples = buffer.getNumSamples();

        // Create a temporary buffer for processing
        juce::AudioBuffer<float> tempBuffer(1, numSamples);
        tempBuffer.copyFrom(0, 0, buffer, channel, 0, numSamples);

        // Store original signal for mix
        juce::AudioBuffer<float> dryBuffer(1, numSamples);
        dryBuffer.copyFrom(0, 0, buffer, channel, 0, numSamples);

        // Process left channel
        if (channel == 0) {
            juce::dsp::AudioBlock<float> block(tempBuffer);
            juce::dsp::ProcessContextReplacing<float> context(block);

            // Apply filters with their respective gains
            if (leftLowGain != 0.0f) {
                *leftLowFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
                    getSampleRate(), 
                    addRandomDeviation(lowFreq, maxDeviation),
                    0.8f,
                    std::pow(10.0f, leftLowGain / 20.0f)
                );
                leftLowFilter.process(context);
                buffer.copyFrom(0, 0, tempBuffer, 0, 0, numSamples);
                if (noiseEnabled) {
                    for (int i = 0; i < numSamples; ++i) {
                        // Calculate input level in dB
                        float inputLevel = 20.0f * std::log10(std::abs(channelData[i]) + 1e-6f);
                        
                        // Compressor-like gain reduction
                        float gainReduction = 0.0f;
                        if (inputLevel > threshold) {
                            float overThreshold = inputLevel - threshold;
                            if (overThreshold < -knee/2.0f) {
                                gainReduction = 0.0f;
                            } else if (overThreshold > knee/2.0f) {
                                gainReduction = (overThreshold) * (1.0f - 1.0f/ratio);
                            } else {
                                float t = (overThreshold + knee/2.0f) / knee;
                                gainReduction = (1.0f - 1.0f/ratio) * t * t / 4.0f;
                            }
                        }
                        
                        // Convert gain reduction to linear scale
                        float linearGain = std::pow(10.0f, -gainReduction/20.0f);
                        
                        // Apply envelope follower
                        float targetEnvelope = std::abs(channelData[i]);
                        if (targetEnvelope > envelope) {
                            envelope = envelope + (targetEnvelope - envelope) * static_cast<float>(1.0f - std::exp(-1.0f / (getSampleRate() * attackTime)));
                        } else {
                            envelope = envelope + (targetEnvelope - envelope) * static_cast<float>(1.0f - std::exp(-1.0f / (getSampleRate() * releaseTime)));
                        }
                        
                        // Add noise with smooth gain control
                        channelData[i] += generatePinkNoise() * leftLowGain * 0.002f * linearGain * envelope;
                    }
                }
            }
            if (leftLowMidGain != 0.0f) {
                *leftLowMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
                    getSampleRate(),
                    addRandomDeviation(lowMidFreq, maxDeviation),
                    1.2f,
                    std::pow(10.0f, leftLowMidGain / 20.0f)
                );
                leftLowMidFilter.process(context);
                buffer.copyFrom(0, 0, tempBuffer, 0, 0, numSamples);
                if (noiseEnabled) {
                    for (int i = 0; i < numSamples; ++i) {
                        float inputLevel = 20.0f * std::log10(std::abs(channelData[i]) + 1e-6f);
                        float gainReduction = 0.0f;
                        if (inputLevel > threshold) {
                            float overThreshold = inputLevel - threshold;
                            if (overThreshold < -knee/2.0f) {
                                gainReduction = 0.0f;
                            } else if (overThreshold > knee/2.0f) {
                                gainReduction = (overThreshold) * (1.0f - 1.0f/ratio);
                            } else {
                                float t = (overThreshold + knee/2.0f) / knee;
                                gainReduction = (1.0f - 1.0f/ratio) * t * t / 4.0f;
                            }
                        }
                        float linearGain = std::pow(10.0f, -gainReduction/20.0f);
                        float targetEnvelope = std::abs(channelData[i]);
                        if (targetEnvelope > envelope) {
                            envelope = envelope + (targetEnvelope - envelope) * static_cast<float>(1.0f - std::exp(-1.0f / (getSampleRate() * attackTime)));
                        } else {
                            envelope = envelope + (targetEnvelope - envelope) * static_cast<float>(1.0f - std::exp(-1.0f / (getSampleRate() * releaseTime)));
                        }
                        channelData[i] += generatePinkNoise() * leftLowMidGain * 0.002f * linearGain * envelope;
                    }
                }
            }
            if (leftHighMidGain != 0.0f) {
                *leftHighMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
                    getSampleRate(),
                    addRandomDeviation(highMidFreq, maxDeviation),
                    0.5f,
                    std::pow(10.0f, leftHighMidGain / 20.0f)
                );
                leftHighMidFilter.process(context);
                buffer.copyFrom(0, 0, tempBuffer, 0, 0, numSamples);
            }
            if (leftHighGain != 0.0f) {
                *leftHighFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
                    getSampleRate(),
                    addRandomDeviation(highFreq, maxDeviation),
                    1.0f,
                    std::pow(10.0f, leftHighGain / 20.0f)
                );
                leftHighFilter.process(context);
                buffer.copyFrom(0, 0, tempBuffer, 0, 0, numSamples);
                if (noiseEnabled) {
                    for (int i = 0; i < numSamples; ++i) {
                        float inputLevel = 20.0f * std::log10(std::abs(channelData[i]) + 1e-6f);
                        float gainReduction = 0.0f;
                        if (inputLevel > threshold) {
                            float overThreshold = inputLevel - threshold;
                            if (overThreshold < -knee/2.0f) {
                                gainReduction = 0.0f;
                            } else if (overThreshold > knee/2.0f) {
                                gainReduction = (overThreshold) * (1.0f - 1.0f/ratio);
                            } else {
                                float t = (overThreshold + knee/2.0f) / knee;
                                gainReduction = (1.0f - 1.0f/ratio) * t * t / 4.0f;
                            }
                        }
                        float linearGain = std::pow(10.0f, -gainReduction/20.0f);
                        float targetEnvelope = std::abs(channelData[i]);
                        if (targetEnvelope > envelope) {
                            envelope = envelope + (targetEnvelope - envelope) * static_cast<float>(1.0f - std::exp(-1.0f / (getSampleRate() * attackTime)));
                        } else {
                            envelope = envelope + (targetEnvelope - envelope) * static_cast<float>(1.0f - std::exp(-1.0f / (getSampleRate() * releaseTime)));
                        }
                        channelData[i] += generateDarkNoise() * leftHighGain * 0.001f * linearGain * envelope;
                    }
                }
            }
        }
        // Process right channel
        else if (channel == 1) {
            juce::dsp::AudioBlock<float> block(tempBuffer);
            juce::dsp::ProcessContextReplacing<float> context(block);

            // Apply filters with their respective gains
            if (rightLowGain != 0.0f) {
                *rightLowFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(
                    getSampleRate(), 
                    addRandomDeviation(lowFreq, maxDeviation),
                    0.8f,
                    std::pow(10.0f, rightLowGain / 20.0f)
                );
                rightLowFilter.process(context);
                buffer.copyFrom(1, 0, tempBuffer, 0, 0, numSamples);
                if (noiseEnabled) {
                    for (int i = 0; i < numSamples; ++i) {
                        float inputLevel = 20.0f * std::log10(std::abs(channelData[i]) + 1e-6f);
                        float gainReduction = 0.0f;
                        if (inputLevel > threshold) {
                            float overThreshold = inputLevel - threshold;
                            if (overThreshold < -knee/2.0f) {
                                gainReduction = 0.0f;
                            } else if (overThreshold > knee/2.0f) {
                                gainReduction = (overThreshold) * (1.0f - 1.0f/ratio);
                            } else {
                                float t = (overThreshold + knee/2.0f) / knee;
                                gainReduction = (1.0f - 1.0f/ratio) * t * t / 4.0f;
                            }
                        }
                        float linearGain = std::pow(10.0f, -gainReduction/20.0f);
                        float targetEnvelope = std::abs(channelData[i]);
                        if (targetEnvelope > envelope) {
                            envelope = envelope + (targetEnvelope - envelope) * static_cast<float>(1.0f - std::exp(-1.0f / (getSampleRate() * attackTime)));
                        } else {
                            envelope = envelope + (targetEnvelope - envelope) * static_cast<float>(1.0f - std::exp(-1.0f / (getSampleRate() * releaseTime)));
                        }
                        channelData[i] += generatePinkNoise() * rightLowGain * 0.002f * linearGain * envelope;
                    }
                }
            }
            if (rightLowMidGain != 0.0f) {
                *rightLowMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
                    getSampleRate(),
                    addRandomDeviation(lowMidFreq, maxDeviation),
                    1.2f,
                    std::pow(10.0f, rightLowMidGain / 20.0f)
                );
                rightLowMidFilter.process(context);
                buffer.copyFrom(1, 0, tempBuffer, 0, 0, numSamples);
                if (noiseEnabled) {
                    for (int i = 0; i < numSamples; ++i) {
                        float inputLevel = 20.0f * std::log10(std::abs(channelData[i]) + 1e-6f);
                        float gainReduction = 0.0f;
                        if (inputLevel > threshold) {
                            float overThreshold = inputLevel - threshold;
                            if (overThreshold < -knee/2.0f) {
                                gainReduction = 0.0f;
                            } else if (overThreshold > knee/2.0f) {
                                gainReduction = (overThreshold) * (1.0f - 1.0f/ratio);
                            } else {
                                float t = (overThreshold + knee/2.0f) / knee;
                                gainReduction = (1.0f - 1.0f/ratio) * t * t / 4.0f;
                            }
                        }
                        float linearGain = std::pow(10.0f, -gainReduction/20.0f);
                        float targetEnvelope = std::abs(channelData[i]);
                        if (targetEnvelope > envelope) {
                            envelope = envelope + (targetEnvelope - envelope) * static_cast<float>(1.0f - std::exp(-1.0f / (getSampleRate() * attackTime)));
                        } else {
                            envelope = envelope + (targetEnvelope - envelope) * static_cast<float>(1.0f - std::exp(-1.0f / (getSampleRate() * releaseTime)));
                        }
                        channelData[i] += generatePinkNoise() * rightLowMidGain * 0.002f * linearGain * envelope;
                    }
                }
            }
            if (rightHighMidGain != 0.0f) {
                *rightHighMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(
                    getSampleRate(),
                    addRandomDeviation(highMidFreq, maxDeviation),
                    0.5f,
                    std::pow(10.0f, rightHighMidGain / 20.0f)
                );
                rightHighMidFilter.process(context);
                buffer.copyFrom(1, 0, tempBuffer, 0, 0, numSamples);
            }
            if (rightHighGain != 0.0f) {
                *rightHighFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(
                    getSampleRate(),
                    addRandomDeviation(highFreq, maxDeviation),
                    1.0f,
                    std::pow(10.0f, rightHighGain / 20.0f)
                );
                rightHighFilter.process(context);
                buffer.copyFrom(1, 0, tempBuffer, 0, 0, numSamples);
                if (noiseEnabled) {
                    for (int i = 0; i < numSamples; ++i) {
                        float inputLevel = 20.0f * std::log10(std::abs(channelData[i]) + 1e-6f);
                        float gainReduction = 0.0f;
                        if (inputLevel > threshold) {
                            float overThreshold = inputLevel - threshold;
                            if (overThreshold < -knee/2.0f) {
                                gainReduction = 0.0f;
                            } else if (overThreshold > knee/2.0f) {
                                gainReduction = (overThreshold) * (1.0f - 1.0f/ratio);
                            } else {
                                float t = (overThreshold + knee/2.0f) / knee;
                                gainReduction = (1.0f - 1.0f/ratio) * t * t / 4.0f;
                            }
                        }
                        float linearGain = std::pow(10.0f, -gainReduction/20.0f);
                        float targetEnvelope = std::abs(channelData[i]);
                        if (targetEnvelope > envelope) {
                            envelope = envelope + (targetEnvelope - envelope) * static_cast<float>(1.0f - std::exp(-1.0f / (getSampleRate() * attackTime)));
                        } else {
                            envelope = envelope + (targetEnvelope - envelope) * static_cast<float>(1.0f - std::exp(-1.0f / (getSampleRate() * releaseTime)));
                        }
                        channelData[i] += generateDarkNoise() * rightHighGain * 0.001f * linearGain * envelope;
                    }
                }
            }
        }

        // Apply subharmonics AFTER all the filter processing is done
        if (isSubEnabled)
        {
            // Process subharmonics
            subProcessor[channel].processBlock(subData, numSamples);
            
            // Filter the subharmonics
            float* subChannelData[] = { subData };
            juce::dsp::AudioBlock<float> subBlock(subChannelData, 1, numSamples);
            juce::dsp::ProcessContextReplacing<float> subContext(subBlock);
            subLowPassFilter[channel].process(subContext);
            
            // Get the appropriate gain value based on the channel
            float lowGain = (channel == 0) ? leftLowGain : rightLowGain;
            
            // Calculate a scaling factor based on the low EQ
            // Make the subharmonics significantly stronger and more responsive to low EQ changes
            float subScaling = 1.5f; // Start with a stronger baseline (was 1.0f)
            if (lowGain > 0.0f) {
                // Increase sub amount when low gain is increased - more dramatic scaling
                subScaling = 1.5f + (lowGain / 12.0f); // Scale from 1.5 to 3.5 based on max gain (was 1.0 to 2.0)
            } else {
                // Maintain more presence even when low gain is cut
                subScaling = std::max(0.8f, 1.5f + (lowGain / 36.0f)); // Maintain more presence at low settings
            }
            
            // Add the subharmonics to the signal (instead of replacing parts of it)
            for (int i = 0; i < numSamples; ++i)
            {
                // Calculate sub amount based on mix parameter
                float subAmountLocal = subMixValue * subAmountValue * subScaling * 2.0f;
                
                // Create much more subtle stereo imaging with very slow changes
                static float leftChannelFactor = 1.0f;
                static float rightChannelFactor = 1.0f;
                static int sampleCounter = 0;
                
                // Only update random factors very rarely - once every ~100ms at 44.1kHz
                if (sampleCounter++ >= 4410) {
                    sampleCounter = 0;
                    // Very subtle random balance with minimal movement
                    float randomBalance = 1.05f + 0.1f * pinkNoiseRandom.nextFloat();
                    // Apply smoothing to prevent sudden changes
                    leftChannelFactor = 0.7f * leftChannelFactor + 0.3f * randomBalance;
                    rightChannelFactor = 0.7f * rightChannelFactor + 0.3f * (2.0f - randomBalance);
                }
                
                // Apply channel-specific factor
                float channelFactor = (channel == 0) ? leftChannelFactor : rightChannelFactor;
                
                // Add the subharmonics to the original signal
                channelData[i] += subData[i] * subAmountLocal * channelFactor;
            }
        }

        // Apply mix at the end, mixing between dry and processed signal
        for (int i = 0; i < numSamples; ++i) {
            channelData[i] = channelData[i] * mix + dryBuffer.getSample(0, i) * (1.0f - mix);
        }
  }
}

bool AudioPluginAudioProcessor::hasEditor() const {
    return true;
}

juce::AudioProcessorEditor* AudioPluginAudioProcessor::createEditor() {
  return new AudioPluginAudioProcessorEditor(*this);
}

void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData) {
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudioPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes) {
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

}  // namespace audio_plugin

// This creates new instances of the plugin.
// This function definition must be in the global namespace.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new audio_plugin::AudioPluginAudioProcessor();
}
