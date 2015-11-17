#include "Character.h"

Character::Character() {

    std::cout << "Creating Character..." << std::endl;
}


Character::~Character() {

    std::cout << "Destroying Character...";

    mVertices.clear();
    mVertices.shrink_to_fit();

    mNormals.clear();
    mNormals.shrink_to_fit();

    mUvs.clear();
    mUvs.shrink_to_fit();

    std::cout << "\tDone!\n";
}


void Character::initialize(glm::vec3 lightSourcePosition) {

    std::cout << "\nInitializing Character...";

    std::cout << "\tDone!\n";
}


void Character::render(std::vector<glm::mat4> sceneMatrices) {

}