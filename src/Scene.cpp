#include "Scene.h"

Scene::Scene() {

    std::cout << "\nCreating Scene...\n";
    
    mLightSourcePosition = glm::vec3(0.0f, 0.0f, 0.0f);

    addLevel(new Level("../assets/level1.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));
    addLevel(new Level("../assets/level2.obj", glm::vec4(0.2f, 0.8f, 0.2f, 1.0f)));
    addLevel(new Level("../assets/level3.obj", glm::vec4(0.2f, 0.2f, 0.8f, 1.0f)));
    addLevel(new Level("../assets/level4.obj", glm::vec4(0.8f, 0.8f, 0.2f, 1.0f)));

    mCharacter = new Character();

    std::cout << "\nScene created!\n";
}
    

Scene::~Scene() {

    std::cout << "Destroying Scene...\n";

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        delete (*it);

    mLevels.clear();
    mLevels.shrink_to_fit();

    delete mCharacter;

    std::cout << "Scene destroyed!\n";
}


void Scene::initialize() {

    std::cout << "\nInitializing Scene...\n";

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        (*it)->initialize(mLightSourcePosition);

    mCharacter->initialize(mLightSourcePosition);

    std::cout << "\nScene initialized!\n";
}

    
void Scene::render(std::vector<glm::mat4> sceneMatrices) {

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        (*it)->render(sceneMatrices);

    mCharacter->render(sceneMatrices);
}


//void Scene::addLevelForce(unsigned int i, float f) {
//    
//    mLevels[i]->addForce(f);
//}


std::vector<float> Scene::getLevelAngles() {

    std::vector<float> angles;

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        angles.push_back((*it)->getAngle());

    return angles;
}