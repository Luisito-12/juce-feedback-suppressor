#pragma once

class NotchFilter {
public:
    NotchFilter();
    ~NotchFilter();

    void prepare(double sampleRate);
    
    void setFrequency(float freqHz);
    void setQFactor(float q);
    void setGain(float gainDb);
    
    float processSample(float sample);
    void reset();

private:
    double sampleRate = 44100.0;
    float frequency = 1000.0f;
    float qFactor = 1.0f;
    float gainDb = 0.0f;
    
    // Coeficientes biquad
    float b0, b1, b2;
    float a1, a2;
    
    // Estado
    float x1 = 0.0f, x2 = 0.0f;
    float y1 = 0.0f, y2 = 0.0f;
    
    void updateCoefficients();
};
