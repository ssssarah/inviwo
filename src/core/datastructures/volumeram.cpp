#include <inviwo/core/datastructures/volumeram.h>

namespace inviwo {

    VolumeRAM::VolumeRAM()
        : VolumeRepresentation(uvec3(128,128,128), "UINT8"), data_(0)
    {}

    VolumeRAM::VolumeRAM(uvec3 dimensions)
        : VolumeRepresentation(dimensions, "UINT8"), data_(0)
    {}

    VolumeRAM::~VolumeRAM() {}

    void VolumeRAM::initialize() {}

    void VolumeRAM::deinitialize() {}

    void* VolumeRAM::getData() {
        return data_;
    }

} // namespace
