#include "YourPluginName/PluginProcessor.h"
#include "YourPluginName/PluginEditor.h"
#include <cmath>

namespace audio_plugin {

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameterLayout() {
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    
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
    return "LightTone MS-17x";
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

void AudioPluginAudioProcessor::prepareToPlay(double sampleRate, int samplesPerBlock) {
    // Initialize filters
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
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
    *leftLowFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, 100.0f, 0.7f, 1.0f);
    *leftLowMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 400.0f, 0.7f, 1.0f);
    *leftHighMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 2000.0f, 0.7f, 1.0f);
    *leftHighFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 8000.0f, 0.7f, 1.0f);

    *rightLowFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, 100.0f, 0.7f, 1.0f);
    *rightLowMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 400.0f, 0.7f, 1.0f);
    *rightHighMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, 2000.0f, 0.7f, 1.0f);
    *rightHighFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, 8000.0f, 0.7f, 1.0f);
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

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                           juce::MidiBuffer& midiMessages) {
    juce::ignoreUnused(midiMessages);

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear any output channels that don't contain input data
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear(i, 0, buffer.getNumSamples());

    // Create audio block for both channels
    juce::dsp::AudioBlock<float> block(buffer);
    juce::dsp::ProcessContextReplacing<float> context(block);

    // Store original audio for wet/dry mix
    juce::AudioBuffer<float> dryBuffer(buffer.getNumChannels(), buffer.getNumSamples());
    dryBuffer.copyFrom(0, 0, buffer, 0, 0, buffer.getNumSamples());
    dryBuffer.copyFrom(1, 0, buffer, 1, 0, buffer.getNumSamples());

    // Get current parameter values
    float leftLowGain = apvts.getRawParameterValue("leftLow")->load();
    float leftLowMidGain = apvts.getRawParameterValue("leftLowMid")->load();
    float leftHighMidGain = apvts.getRawParameterValue("leftHighMid")->load();
    float leftHighGain = apvts.getRawParameterValue("leftHigh")->load();

    float rightLowGain = apvts.getRawParameterValue("rightLow")->load();
    float rightLowMidGain = apvts.getRawParameterValue("rightLowMid")->load();
    float rightHighMidGain = apvts.getRawParameterValue("rightHighMid")->load();
    float rightHighGain = apvts.getRawParameterValue("rightHigh")->load();

    // Get wet/dry mix value
    float mixValue = apvts.getRawParameterValue("mix")->load() / 100.0f;

    // Check if sync is enabled
    bool syncEnabled = apvts.getRawParameterValue("sync")->load() > 0.5f;
    isSynced = syncEnabled;

    // If sync is enabled, use the last changed value for both channels
    if (syncEnabled) {
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

    // Convert dB values to linear gain
    float leftLowGainLinear = std::pow(10.0f, leftLowGain / 20.0f);
    float leftLowMidGainLinear = std::pow(10.0f, leftLowMidGain / 20.0f);
    float leftHighMidGainLinear = std::pow(10.0f, leftHighMidGain / 20.0f);
    float leftHighGainLinear = std::pow(10.0f, leftHighGain / 20.0f);

    float rightLowGainLinear = std::pow(10.0f, rightLowGain / 20.0f);
    float rightLowMidGainLinear = std::pow(10.0f, rightLowMidGain / 20.0f);
    float rightHighMidGainLinear = std::pow(10.0f, rightHighMidGain / 20.0f);
    float rightHighGainLinear = std::pow(10.0f, rightHighGain / 20.0f);

    // Update filter gains
    *leftLowFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(getSampleRate(), 100.0f, 0.7f, leftLowGainLinear);
    *leftLowMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 400.0f, 0.7f, leftLowMidGainLinear);
    *leftHighMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 2000.0f, 0.7f, leftHighMidGainLinear);
    *leftHighFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(getSampleRate(), 8000.0f, 0.7f, leftHighGainLinear);

    *rightLowFilter.state = *juce::dsp::IIR::Coefficients<float>::makeLowShelf(getSampleRate(), 100.0f, 0.7f, rightLowGainLinear);
    *rightLowMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 400.0f, 0.7f, rightLowMidGainLinear);
    *rightHighMidFilter.state = *juce::dsp::IIR::Coefficients<float>::makePeakFilter(getSampleRate(), 2000.0f, 0.7f, rightHighMidGainLinear);
    *rightHighFilter.state = *juce::dsp::IIR::Coefficients<float>::makeHighShelf(getSampleRate(), 8000.0f, 0.7f, rightHighGainLinear);

    // Process left channel
    auto leftBlock = block.getSingleChannelBlock(0);
    juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
    leftLowFilter.process(leftContext);
    leftLowMidFilter.process(leftContext);
    leftHighMidFilter.process(leftContext);
    leftHighFilter.process(leftContext);

    // Process right channel
    auto rightBlock = block.getSingleChannelBlock(1);
    juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);
    rightLowFilter.process(rightContext);
    rightLowMidFilter.process(rightContext);
    rightHighMidFilter.process(rightContext);
    rightHighFilter.process(rightContext);

    // Apply wet/dry mix
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample) {
            float wetSample = buffer.getSample(channel, sample);
            float drySample = dryBuffer.getSample(channel, sample);
            buffer.setSample(channel, sample, wetSample * mixValue + drySample * (1.0f - mixValue));
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
