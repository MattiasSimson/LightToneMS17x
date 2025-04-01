#ifndef LEVELDETECTOR_H_INCLUDED
#define LEVELDETECTOR_H_INCLUDED

#include <juce_core/juce_core.h>

/** Simple peak level detector */
class LevelDetector
{
public:
    LevelDetector() {}

    void setAttack(float attMs)
    {
        if (attMs != attackMs)
        {
            attackMs = attMs;
            a1_a = std::exp(-1.0f / (fs * attackMs / 1000.0f));
            b0_a = 1.0f - a1_a;
        }
    }

    void setRelease(float relMs)
    {
        if (relMs != releaseMs)
        {
            releaseMs = relMs;
            a1_r = std::exp(-1.0f / (fs * releaseMs / 1000.0f));
            b0_r = 1.0f - a1_r;
        }
    }

    void reset(float sampleRate)
    {
        fs = sampleRate;
        levelEst = 0.0f;

        setAttack(attackMs);
        setRelease(releaseMs);
    }

    inline float processSample(float x)
    {
        if (std::fabs(x) > levelEst)
            levelEst += b0_a * (std::fabs(x) - levelEst);
        else
            levelEst += b0_r * (std::fabs(x) - levelEst);

        return levelEst;
    }

private:
    float attackMs = 1.0f;
    float a1_a = 0.0f;
    float b0_a = 1.0f;

    float releaseMs = 50.0f;
    float a1_r = 0.0f;
    float b0_r = 1.0f;

    float levelEst = 0.0f;
    float fs = 48000.0f;
};

#endif // LEVELDETECTOR_H_INCLUDED 