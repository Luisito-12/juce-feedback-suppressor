#include "PluginProcessor.h"
#include "PluginEditor.h"

juce::AudioProcessorValueTreeState::ParameterLayout FeedbackSuppressorAudioProcessor::createParameterLayout()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back(std::make_unique<juce::AudioParameterBool>(
        juce::ParameterID("enabled", 1), "Enabled", true));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("threshold", 1), "Threshold",
        juce::NormalisableRange<float>(-60.0f, 0.0f, 0.1f),
        -20.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("ratio", 1), "Ratio",
        juce::NormalisableRange<float>(1.0f, 10.0f, 0.1f),
        4.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("qFactor", 1), "Q Factor",
        juce::NormalisableRange<float>(0.5f, 10.0f, 0.1f),
        1.0f));
    
    params.push_back(std::make_unique<juce::AudioParameterInt>(
        juce::ParameterID("numNotches", 1), "Num Notches",
        1, 12, 4));
    
    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID("outputGain", 1), "Output Gain",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f),
        0.0f));
    
    return { params.begin(), params.end() };
}

FeedbackSuppressorAudioProcessor::FeedbackSuppressorAudioProcessor()
    : apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    enabledParam = dynamic_cast<juce::AudioParameterBool*>(apvts.getParameter("enabled"));
    thresholdParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("threshold"));
    ratioParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("ratio"));
    qFactorParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("qFactor"));
    numNotchesParam = dynamic_cast<juce::AudioParameterInt*>(apvts.getParameter("numNotches"));
    outputGainParam = dynamic_cast<juce::AudioParameterFloat*>(apvts.getParameter("outputGain"));
}

FeedbackSuppressorAudioProcessor::~FeedbackSuppressorAudioProcessor()
{
}

void FeedbackSuppressorAudioProcessor::prepareToPlay(double sr, int samplesPerBlock)
{
    sampleRate = sr;
    analyzer.prepare(sampleRate, samplesPerBlock);
    peakTracker.prepare(sampleRate);
    
    for (auto& nf : notchFilters)
        nf.prepare(sampleRate);
}

void FeedbackSuppressorAudioProcessor::releaseResources()
{
}

void FeedbackSuppressorAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, 
                                                    juce::MidiBuffer& midiMessages)
{
    juce::ignoreUnused(midiMessages);
    
    if (!enabledParam || !enabledParam->get())
        return;

    float threshold = thresholdParam ? thresholdParam->get() : -20.0f;
    float ratio = ratioParam ? ratioParam->get() : 4.0f;
    float qFactor = qFactorParam ? qFactorParam->get() : 1.0f;
    int numNotches = numNotchesParam ? numNotchesParam->get() : 4;
    float outputGain = outputGainParam ? outputGainParam->get() : 0.0f;
    float outputGainLinear = juce::Decibels::decibelsToGain(outputGain);

    // Análisis de feedback
    auto peaks = analyzer.analyze(buffer);
    
    // Seguimiento de picos y actualización de notches
    for (const auto& peak : peaks) {
        bool tracked = peakTracker.track(peak.frequency, peak.magnitude);
        
        if (tracked && activeNotches < numNotches) {
            // Crear nuevo notch
            notchFilters[activeNotches].setFrequency(peak.frequency);
            notchFilters[activeNotches].setQFactor(qFactor);
            notchFilters[activeNotches].setGain(-6.0f); // -6dB atenuación
            activeNotches++;
        }
    }

    // Aplicar notches
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch) {
        auto* samples = buffer.getWritePointer(ch);
        
        for (int i = 0; i < buffer.getNumSamples(); ++i) {
            float sample = samples[i];
            
            // Aplicar todos los notches activos
            for (int n = 0; n < activeNotches; ++n) {
                sample = notchFilters[n].processSample(sample);
            }
            
            // Aplicar ganancia de salida
            samples[i] = sample * outputGainLinear;
        }
    }

    // Limpiar notches viejos si es necesario
    activeNotches = std::min(activeNotches, numNotches);
}

juce::AudioProcessorEditor* FeedbackSuppressorAudioProcessor::createEditor()
{
    return new FeedbackSuppressorAudioProcessorEditor(*this);
}

void FeedbackSuppressorAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    auto state = apvts.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void FeedbackSuppressorAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}

// Exportar plugin
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FeedbackSuppressorAudioProcessor();
}
