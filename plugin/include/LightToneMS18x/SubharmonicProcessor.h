#ifndef SUBHARMONICPROCESSOR_H_INCLUDED
#define SUBHARMONICPROCESSOR_H_INCLUDED

#include <juce_core/juce_core.h>
#include "LevelDetector.h"

class SubharmonicProcessor
{
public:
    SubharmonicProcessor() {}

    void reset(float sampleRate)
    {
        rising = true;
        switchCount = 0;
        output = 1.0f;
        lastX = 0.0f;
        subOctave = 1.0f;
        subCount = 0;

        detector.reset(sampleRate);
    }

    void setDetector(float attackMs, float releaseMs)
    {
        detector.setAttack(attackMs);
        detector.setRelease(releaseMs);
    }

    void processBlock(float* buffer, const int numSamples)
    {
        for (int n = 0; n < numSamples; ++n)
            buffer[n] = processSample(buffer[n]);
    }

    inline float processSample(float x)
    {
        float level = detector.processSample(x);

        if (rising == true && x < lastX)
        {
            switchCount++;
            rising = false;
        }
        else if (rising == false && x > lastX)
        {
            switchCount++;
            rising = true;
        }

        if (switchCount == 2)
        {
            output *= -1.0f;
            switchCount = 0;
            
            subCount++;
            if (subCount == 2)
            {
                subOctave *= -1.0f;
                subCount = 0;
            }
        }

        lastX = x;
        return level * (output * 0.7f + subOctave * 0.3f);
    }

private:
    bool rising = true;
    int switchCount = 0;
    float output = 1.0f;
    float lastX = 0.0f;
    
    float subOctave = 1.0f;
    int subCount = 0;

    LevelDetector detector;
};

#endif // SUBHARMONICPROCESSOR_H_INCLUDED 