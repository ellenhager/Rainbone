#include "Level.h"

Level::Level(const char * objPath, glm::vec4 c) {

    std::cout << "Creating Level..." << std::endl;

    loadOBJ(objPath, mVertices, mNormals);

    mMaterial.color         = c;
    mMaterial.ambient       = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    mMaterial.diffuse       = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    mMaterial.specular      = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mMaterial.specularity   = 50.0f;
    mMaterial.shinyness     = 0.6f;
}


Level::~Level() {

    std::cout << "Destroying Level..." << std::endl;

    mVertices.clear();
    mVertices.shrink_to_fit();

    mNormals.clear();
    mNormals.shrink_to_fit();
}


void Level::initialize() {

    std::cout << "Initializing Level..." << std::endl;
}


void Level::render() {

    std::cout << "Rendering Level..." << std::endl;
}