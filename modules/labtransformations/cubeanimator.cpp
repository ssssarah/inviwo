/*********************************************************************
 *  Author  : Himangshu Saikia
 *  Init    : Thursday, October 12, 2017 - 11:11:30
 *
 *  Project : KTH Inviwo Modules
 *
 *  License : Follows the Inviwo BSD license model
 *********************************************************************
 */

#include <labtransformations/cubeanimator.h>

namespace inviwo
{

// The Class Identifier has to be globally unique. Use a reverse DNS naming scheme
const ProcessorInfo CubeAnimator::processorInfo_{
    "org.inviwo.CubeAnimator",// Class identifier
    "Cube Animator",          // Display name
    "VisGra",                 // Category
    CodeState::Experimental,  // Code state
    Tags::None,               // Tags
};

const ProcessorInfo CubeAnimator::getProcessorInfo() const
{
    return processorInfo_;
}

int currXpoz = 2.5;
int currYPoz = 3;
int currZPoz = 0;

CubeAnimator::CubeAnimator()
    : Processor()
      // Ports
      ,
      meshIn_("meshIn"), meshOut_("meshOut")
      // Properties
      // For a FloatProperty
      // variablename(identifier, display name, init value, minvalue, maxvalue)
      ,
      radius_("radius", "Radius", 6, 1, 8), 
      moveAroundTower("moveAroundTower", "Position Around Tower", 0, 0, 1000)
{
    // Add ports
    addPort(meshIn_);
    addPort(meshOut_);

    // Add properties
    addProperty(radius_);
    addProperty(moveAroundTower);
}

void CubeAnimator::process()
{
    // Clone the input mesh
    if (!meshIn_.getData())
        return;

    auto mesh = meshIn_.getData()->clone();

    // Get the matrix that defines where the mesh is currently
    auto matrix = mesh->getWorldMatrix();

    // Transform the mesh (TODO)
    //moveAroundTower = moveAroundTower * 1000;

    float cosX = (cos(((3.14 * 2) / 100) * moveAroundTower) * radius_.get());
    float cosY = (radius_.get() * (1 - cos((3.14 * 2) / 100 * moveAroundTower)));
        
	matrix = 
		
        glm::translate(vec3((radius_.get() + cos(fmod(((3.14 * 2) / 1000) * moveAroundTower, 0.78) * 8)) * (sin((3.14 * 2) / 1000 * moveAroundTower)),
                             cos(((3.14 * 2) / 1000) * moveAroundTower) * (radius_.get() + cos(fmod(((3.14 * 2) / 1000) * moveAroundTower, 0.78) * 8)),
                             0))
		* glm::rotate((float)(((3.14 * 2) / 1000) * moveAroundTower ), vec3(0, 0, 1)) 
		* glm::translate(vec3(currXpoz, currYPoz, currZPoz))
				
		//translate bun
		 
		 
		* matrix;

    //matrix *= ;

    // Update
    mesh->setWorldMatrix(matrix);

    // Set output
    meshOut_.setData(mesh);
}

}// namespace inviwo
