#include "Scene.h"

Scene::Scene() {

    std::cout << "Creating Scene..." << std::endl;
    
    addLevel(new Level("../assets/simple.obj", glm::vec4(1.0f)));

    mCharacter = new Character();
}
    

Scene::~Scene() {

    std::cout << "Destroying Scene..." << std::endl;
}


void Scene::initialize() {

    std::cout << "Initializing Scene..." << std::endl;

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        (*it)->initialize();

    mCharacter->initialize();
}

    
void Scene::render() {

    std::cout << "Rendering Scene..." << std::endl;
    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        (*it)->render();

    mCharacter->render();
}


void Scene::addLevelForce(unsigned int i, float f) {
    
    mLevels[i]->addForce(f);
}