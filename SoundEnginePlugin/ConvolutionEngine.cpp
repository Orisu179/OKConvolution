#include "ConvolutionEngine.h"

AKRESULT ConvolutionEngine::Init(AK::IAkPluginMemAlloc* in_pAllocator, const ConvolverConfig& in_config)
{
    return AK_Success;
}

void ConvolutionEngine::Term(AK::IAkPluginMemAlloc* in_pAllocator)
{
    
}

void ConvolutionEngine::Reset()
{
    
}

AKRESULT ConvolutionEngine::SetIR(const IRDescriptor& in_irDesc)
{
    return AK_Success;
}

void ConvolutionEngine::Process(AkAudioBuffer* io_pBuffer, float in_fWetGain, float in_fDryGain)
{
    
}
