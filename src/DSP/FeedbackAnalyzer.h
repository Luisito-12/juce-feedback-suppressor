#pragma once

#include <JuceHeader.h>
#include <vector>
#include <complex>

struct PeakInfo {
    float frequency;  // Hz
    float magnitude;  // dB
    float bandwidth;  // Hz aproximado
};

class FeedbackAnalyzer {
public:
    FeedbackAnalyzer();
    ~FeedbackAnalyzer();

    void prepare(double sampleRate, int maxBlockSize);
    std::vector<PeakInfo> analyze(const juce::AudioBuffer<float>& buffer);

private:
    static constexpr int FFT_SIZE = 2048;
    
    double sampleRate = 44100.0;
    std::vector<float> window;
    std::vector<float> fftBuffer;
    std::vector<std::complex<float>> spectrum;
    
    void applyHannWindow(std::vector<float>& buffer);
    std::vector<PeakInfo> detectPeaks(const std::vector<float>& magnitude);
    float getMagnitude(int binIndex, const std::vector<float>& magnitude);
    float binToFrequency(int bin) const;
};
