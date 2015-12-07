#include "Scene.h"

Scene::Scene(unsigned int n) {

    std::cout << "\nCreating Scene...\n";
    
    mLightSourcePosition = glm::vec3(0.0f, 0.0f, 0.0f);

    if(n > 7) {
        std::cout << "\nYou tried to create more levels than possible, 7 is the maximum amount!\n";
        std::cout << "\n\nUsing 7 levels instead!\n\n";
        n = 7;
    }

    for(unsigned int i = 1; i <= n; i++) {
        addLevel(new Level(("../assets/objs/_level" + std::to_string(i) + ".obj").c_str(), sColorScale[i-1]));        
    }

    mCharacter = new Character("../assets/objs/char_tmp.obj", "../assets/textures/debug_texture.png");
	
    mSkySphere = new SkySphere(35);

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

    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        (*it)->initialize(mLightSourcePosition);

    mCharacter->initialize(mLightSourcePosition);

	mSkySphere->initialize();

    std::cout << "\nScene initialized!\n";
}

    
void Scene::render(std::vector<glm::mat4> sceneMatrices) {

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        (*it)->render(sceneMatrices);

    mCharacter->render(sceneMatrices);
	mSkySphere->render(sceneMatrices);
}


void Scene::update(float dt) {

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        (*it)->update(dt);

	mSkySphere->update(dt);
}


std::vector<float> Scene::getLevelAngles() {

    std::vector<float> angles;

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        angles.push_back((*it)->getAngle());

    return angles;
}


std::vector<glm::vec4> Scene::getLevelColors() {

    std::vector<glm::vec4> colors;

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        colors.push_back((*it)->getColor());

    return colors;
}