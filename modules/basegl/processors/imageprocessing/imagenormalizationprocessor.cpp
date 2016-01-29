/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2014-2015 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 *********************************************************************************/

#include "imagenormalizationprocessor.h"
#include <inviwo/core/datastructures/image/layerram.h>
#include <modules/opengl/texture/textureutils.h>
#include <modules/opengl/shader/shader.h>

namespace inviwo {

const ProcessorInfo ImageNormalizationProcessor::processorInfo_{
    "org.inviwo.ImageNormalization",  // Class identifier
    "Image Normalization",            // Display name
    "Image Operation",                // Category
    CodeState::Experimental,          // Code state
    Tags::GL,                         // Tags
};
const ProcessorInfo ImageNormalizationProcessor::getProcessorInfo() const {
    return processorInfo_;
}

ImageNormalizationProcessor::ImageNormalizationProcessor()
    : ImageGLProcessor("img_normalize.frag")
    , minMaxInvalid_(true)
    , eachChannelsIndividually_("eachChannelsIndividually","Normalize Channels Individually") 
    , zeroAtPoint5_("zeroAtPoint5", "Negative numbers below 0.5",false)
    , minS_("min","Min value","")
    , maxS_("max", "Max value","")
    , min_(0.0)
    , max_(1.0)
{

    minS_.setInvalidationLevel(InvalidationLevel::Valid);
    maxS_.setInvalidationLevel(InvalidationLevel::Valid);
    minS_.setReadOnly(true);
    maxS_.setReadOnly(true);

    addProperty(eachChannelsIndividually_);
    addProperty(zeroAtPoint5_);
    addProperty(minS_);
    addProperty(maxS_);

    inport_.onChange(this, &ImageNormalizationProcessor::invalidateMinMax);
    eachChannelsIndividually_.onChange(this,&ImageNormalizationProcessor::invalidateMinMax);

    setAllPropertiesCurrentStateAsDefault();
}

ImageNormalizationProcessor::~ImageNormalizationProcessor() {}


void ImageNormalizationProcessor::preProcess() {
    if (minMaxInvalid_) updateMinMax();
    
    dvec3 min = min_.rgb();
    dvec3 max = max_.rgb();

    if (zeroAtPoint5_) {
        max = glm::max(glm::abs(min), glm::abs(max));
        min = -max;
    }

    if (eachChannelsIndividually_.get()) {
        shader_.setUniform("min_", static_cast<vec4>(dvec4(min,0.0)));
        shader_.setUniform("max_", static_cast<vec4>(dvec4(max,1.0)));
    } else {
        double minV = std::min(std::min(min.x,min.y),min.z);
        double maxV = std::max(std::max(max.x,max.y),max.z);
        shader_.setUniform("min_", vec4(minV, minV, minV,0.0f));
        shader_.setUniform("max_", vec4(maxV, maxV, maxV,1.0f));
    }
}


void ImageNormalizationProcessor::updateMinMax() {
    minMaxInvalid_ = true;
}

void ImageNormalizationProcessor::invalidateMinMax() {
    if (!inport_.hasData()) return;
    const LayerRAM* img = inport_.getData()->getColorLayer()->getRepresentation<LayerRAM>();

    uvec2 dim = img->getDimensions();
    uvec2 pixel(0, 0);
    min_ = dvec4(std::numeric_limits<double>::max());
    max_ = dvec4(std::numeric_limits<double>::min());
    for (pixel.y = 0; pixel.y < dim.y; pixel.y++)
        for (pixel.x = 0; pixel.x < dim.x; pixel.x++) {
            dvec4 pixelValue = img->getValueAsVec4Double(pixel);
            if (!(glm::any(glm::isinf(pixelValue)) || glm::any(glm::isnan(pixelValue)))) {
                min_ = glm::min(min_, pixelValue);
                max_ = glm::max(max_, pixelValue);
            }
        }

    minS_.set(toString(min_));
    maxS_.set(toString(max_));

    min_.a = 0.0;  // never normalize alpha
    max_.a = 1.0;
    minMaxInvalid_ = false;
}

} // namespace


