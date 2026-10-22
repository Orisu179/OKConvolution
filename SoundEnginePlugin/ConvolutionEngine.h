#pragma once
#include <vector>
#include <AK/SoundEngine/Common/AkCommonDefs.h>
#include <AK/SoundEngine/Common/IAkPluginMemAlloc.h>
#include <AK/Tools/Common/AkArray.h>

struct IRDescriptor
{
    // 2 for now
    static constexpr AkUInt32 kMaxIRChannels = 2;

    const float* pChannelSamples[kMaxIRChannels] = { nullptr }; 
    AkUInt32 uNumChannels = 0;
    AkUInt32 uNumSamplesPerChannel = 0;
    AkUInt32 uSampleRate = 48000;
    AkUInt32 uIRId = 0;

    // Helper for Mono IRs
    void SetMono(const float* in_pMonoSamples, AkUInt32 in_uNumSamples, AkUInt32 in_uSampleRate, AkUInt32 in_uId)
    {
        pChannelSamples[0] = in_pMonoSamples;
        uNumChannels = 1;
        uNumSamplesPerChannel = in_uNumSamples;
        uSampleRate = in_uSampleRate;
        uIRId = in_uId;
    }

    // Helper for Stereo IRs (two separate channel pointers, or offset into one contiguous buffer)
    void SetStereo(const float* in_pLeft, const float* in_pRight, AkUInt32 in_uNumSamplesPerCh, AkUInt32 in_uSampleRate, AkUInt32 in_uId)
    {
        pChannelSamples[0] = in_pLeft;
        pChannelSamples[1] = in_pRight;
        uNumChannels = 2;
        uNumSamplesPerChannel = in_uNumSamplesPerCh;
        uSampleRate = in_uSampleRate;
        uIRId = in_uId;
    }
};

// Configuration passed once during plugin Init()
struct ConvolverConfig
{
    AkUInt32 uSampleRate = 48000;
    AkUInt32 uBlockSize = 512;          // Matches io_rFormat / uMaxFramesPerBuffer from Wwise
    AkUInt32 uMaxIRLengthSamples = 96000; // Pre-allocate for e.g. max 2.0 seconds @ 48kHz
    AkUInt32 uMaxChannels = 2;
    AkUInt32 uCrossfadeFrames = 2;      // Number of audio buffers to crossfade on IR swap
};

class ConvolutionEngine
{
public:
    AKRESULT Init(AK::IAkPluginMemAlloc* in_pAllocator, const ConvolverConfig& in_config);
    void Term(AK::IAkPluginMemAlloc* in_pAllocator);
    
    // Clears internal delay lines and overlap buffers (e.g., when voice/bus restarts)
    void Reset();
    
    AKRESULT SetIR(const IRDescriptor& in_irDesc);
    [[nodiscard]] AkUInt32 GetActiveIRId() const { return m_uActiveIRId; }
    void Process(AkAudioBuffer* io_pBuffer, float in_fWetGain, float in_fDryGain);
    
private:
    
    // Ping-pong slots: One is active, the other receives new IRs and crossfades in
    // IRPartitionSet m_irSlots[2];
    AkUInt32 m_uActiveSlot = 0;
    AkUInt32 m_uStagingSlot = 1;
    AkUInt32 m_uActiveIRId = 0;

    // Crossfade state (0.0 = 100% Active Slot, 1.0 = 100% Staging Slot -> then swap indices)
    bool m_bIsCrossfading = false;
    float m_fCrossfadeAlpha = 0.0f;
    float m_fCrossfadeStep = 0.0f;

    // Shared Input Frequency Delay Line (FDL) so both old and new IRs convolve the exact same input history!
    float* m_pInputFDLReal = nullptr;
    float* m_pInputFDLImag = nullptr;
    AkUInt32 m_uFDLWriteIndex = 0;
    
    ConvolverConfig m_pConfig;
};
