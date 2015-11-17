#include "Character.h"

Character::Character() {

    std::cout << "Creating Character..." << std::endl;
}


Character::~Character() {

    std::cout << "Destroying Character..." << std::endl;

    mVertices.clear();
    mVertices.shrink_to_fit();

    mNormals.clear();
    mNormals.shrink_to_fit();

    mUvs.clear();
    mUvs.shrink_to_fit();
}


void Character::initialize() {

    std::cout << "Initializing Character..." << std::endl;
}


void Character::render() {

    std::cout << "Rendering Character..." << std::endl;
}