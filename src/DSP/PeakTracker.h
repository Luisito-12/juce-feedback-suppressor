#pragma once

#include <vector>
#include <queue>

struct TrackedPeak {
    float frequency;
    float magnitude;
    int persistenceCount = 0;
    int id;
};

class PeakTracker {
public:
    PeakTracker();
    ~PeakTracker();

    void prepare(double sampleRate);
    bool track(float frequency, float magnitude);
    void update();
    void reset();

private:
    static constexpr int PERSISTENCE_THRESHOLD = 3;
    static constexpr float FREQUENCY_TOLERANCE = 50.0f; // Hz
    
    double sampleRate = 44100.0;
    std::vector<TrackedPeak> trackedPeaks;
    int nextPeakId = 0;
    
    int findNearestPeak(float frequency);
};
