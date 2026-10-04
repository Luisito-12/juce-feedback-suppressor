#include "PluginProcessor.h"
#include "PluginEditor.h"

FeedbackSuppressorAudioProcessor::FeedbackSuppressorAudioProcessor()
{
    // Parámetros
    addParameter(enabledParam = new juce::AudioParameterBool(
        "enabled", "Enabled", true));
    
    addParameter(thresholdParam = new juce::AudioParameterFloat(
        "threshold", "Threshold", 
        juce::NormalisableRange<float>(-60.0f, 0.0f, 0.1f), 
        -20.0f));
    
    addParameter(ratioParam = new juce::AudioParameterFloat(
        "ratio", "Ratio",
        juce::NormalisableRange<float>(1.0f, 10.0f, 0.1f),
        4.0f));
    
    addParameter(qFactorParam = new juce::AudioParameterFloat(
        "qFactor", "Q Factor",
        juce::NormalisableRange<float>(0.5f, 10.0f, 0.1f),
        1.0f));
    
    addParameter(numNotchesParam = new juce::AudioParameterInt(
        "numNotches", "Num Notches",
        1, 12, 4));
    
    addParameter(outputGainParam = new juce::AudioParameterFloat(
        "outputGain", "Output Gain",
        juce::NormalisableRange<float>(-12.0f, 12.0f, 0.1f),
        0.0f));
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
    
    if (!enabledParam->get())
        return;

    float threshold = thresholdParam->get();
    float ratio = ratioParam->get();
    float qFactor = qFactorParam->get();
    int numNotches = numNotchesParam->get();
    float outputGain = outputGainParam->get();
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
