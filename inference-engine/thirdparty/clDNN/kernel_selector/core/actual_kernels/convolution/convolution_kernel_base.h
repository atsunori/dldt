/*
// Copyright (c) 2016 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
*/

#pragma once

#include "weight_bias_kernel_base.h"
#include "convolution_params.h"

namespace kernel_selector 
{
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // ConvolutionKernelBase
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class ConvolutionKernelBase : public WeightBiasKernelBase
    {
    public:
        using WeightBiasKernelBase::WeightBiasKernelBase;
        virtual ~ConvolutionKernelBase() {}

        struct DispatchData : public CommonDispatchData
        {
            struct CLDNNStyle
            {
                size_t blockWidth, blockHeight; // used for kernels processing blocks
                size_t prefetch;
                size_t inputBlockArraySize;     // Number of elements in array of UNIT_TYPE that must be specified in kernel to store/cache input block.
                size_t inputBlockWidth;         // Number of elements in X dimension stored/cached in input block.
            };

            struct GEMMStyle
            {
                size_t subBlockDimM;
                size_t subBlockDimK;
                size_t subBlockDimN;
                size_t globalWorkSizeDX;
                size_t globalWorkSizeDY;
                size_t globalWorkSizeDZ;
            };

            union
            {
                CLDNNStyle cldnnStyle;
                GEMMStyle  gemmStyle;
            };
        };
    
    protected:
        virtual std::vector<WeightsLayout> GetSupportedWeightLayouts(const convolution_params&) const = 0;
        virtual std::string GetKernelName(const convolution_params&) const { return kernelName; }
        virtual bool NeedPaddedInput() const { return false; }
        virtual bool Validate(const Params& p, const optional_params& o) const override;
        virtual JitConstants GetJitConstants(const convolution_params& params, const DispatchData& kd) const;
        virtual DispatchData SetDefault(const convolution_params& params, int autoTuneIndex = -1) const;
        static bool CheckWorkGroups(const DispatchData&);
        static bool CheckPitchForSplitOnly(const convolution_params& params);
        KernelsData GetCommonKernelsData(const Params& params, const optional_params& options, const std::string exeMode = ROUND_ROBIN, int autoTuneIndex = -1) const;
    };
}
