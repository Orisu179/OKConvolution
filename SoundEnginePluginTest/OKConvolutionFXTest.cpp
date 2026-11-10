/*******************************************************************************
The content of this file includes portions of the AUDIOKINETIC Wwise Technology
released in source code form as part of the SDK installer package.

Commercial License Usage

Licensees holding valid commercial licenses to the AUDIOKINETIC Wwise Technology
may use this file in accordance with the end user license agreement provided
with the software or, alternatively, in accordance with the terms contained in a
written agreement between you and Audiokinetic Inc.

Apache License Usage

Alternatively, this file may be used under the Apache License, Version 2.0 (the
"Apache License"); you may not use this file except in compliance with the
Apache License. You may obtain a copy of the Apache License at
http://www.apache.org/licenses/LICENSE-2.0.

Unless required by applicable law or agreed to in writing, software distributed
under the Apache License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
OR CONDITIONS OF ANY KIND, either express or implied. See the Apache License for
the specific language governing permissions and limitations under the License.

  Copyright (c) 2026 Audiokinetic Inc.
*******************************************************************************/

#include "../SoundEnginePlugin/ConvolutionEngine.h"

#include <catch_amalgamated.hpp>

// 2. RAII Wrapper to build a valid AkAudioBuffer in a unit test
struct TestAudioBuffer
{
    std::vector<float> contiguousSamples;
    AkAudioBuffer akBuffer;

    TestAudioBuffer(AkUInt32 uChannels, AkUInt16 uFrames)
        : contiguousSamples(uChannels * uFrames, 0.0f)
    {
        AkChannelConfig cfg;
        cfg.SetStandard(uChannels == 1 ? AK_SPEAKER_SETUP_MONO : AK_SPEAKER_SETUP_STEREO);
        
        // Attach our std::vector memory directly to the AkAudioBuffer
        akBuffer.AttachContiguousDeinterleavedData(
            contiguousSamples.data(), uFrames, uFrames, cfg);
    }

    float* GetChannel(AkUInt32 uCh) { return akBuffer.GetChannel(uCh); }
};

TEST_CASE("Basic Test")
{
    ConvolutionEngine engine;
    REQUIRE(true);
}
