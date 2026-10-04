#pragma once

#include <JuceHeader.h>
#include "DSP/FeedbackAnalyzer.h"
#include "DSP/PeakTracker.h"
#include "DSP/NotchFilter.h"

class FeedbackSuppressorAudioProcessor : public juce::AudioProcessor
{
public:
    FeedbackSuppressorAudioProcessor();
    ~FeedbackSuppressorAudioProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return "Feedback Suppressor"; }

    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }

    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return "Default"; }
    void changeProgramName(int, const juce::String&) override {}

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    // AudioProcessorValueTreeState
    juce::AudioProcessorValueTreeState apvts;

    // Parámetros
    juce::AudioParameterBool* enabledParam;
    juce::AudioParameterFloat* thresholdParam;    // dB
    juce::AudioParameterFloat* ratioParam;        // ratio de compresión
    juce::AudioParameterFloat* qFactorParam;      // Q de los notches
    juce::AudioParameterInt* numNotchesParam;     // cantidad de notches
    juce::AudioParameterFloat* outputGainParam;   // ganancia de salida

private:
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    
    double sampleRate = 44100.0;
    FeedbackAnalyzer analyzer;
    PeakTracker peakTracker;
    std::array<NotchFilter, 12> notchFilters;
    int activeNotches = 0;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FeedbackSuppressorAudioProcessor)
};
