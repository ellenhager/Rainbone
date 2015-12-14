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

    mCharacter = new Character("../assets/objs/character.obj", "../assets/textures/char_texture.png");

    mSkySphere = new SkySphere(35);

    //std::vector<Letter *> L;
    //L.push_back(new Letter("../assets/objs/5.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));

    mWords[FIVE].push_back(new Letter("../assets/objs/5.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));
    /*mWords[FOUR].push_back(new Letter("../assets/objs/4.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));
    mWords[THREE].push_back(new Letter("../assets/objs/3.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));
    mWords[TWO].push_back(new Letter("../assets/objs/2.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));
    mWords[ONE].push_back(new Letter("../assets/objs/1.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));*/


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

    for(std::map<Word, std::vector<Letter *> >::iterator it = mWords.begin(); it != mWords.end(); ++it)
        for(std::vector<Letter *>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2)
            (*it2)->initialize(mLightSourcePosition);

    mCharacter->initialize(mLightSourcePosition);

	mSkySphere->initialize();

    std::cout << "\nScene initialized!\n";
}


void Scene::render(std::vector<glm::mat4> sceneMatrices) {

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        (*it)->render(sceneMatrices);

    for(std::map<Word, std::vector<Letter *> >::iterator it = mWords.begin(); it != mWords.end(); ++it) {
        for(std::vector<Letter *>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
            if((*it2)->shallRender())
                (*it2)->render(sceneMatrices);
        }
    }

    mCharacter->render(sceneMatrices);
	mSkySphere->render(sceneMatrices);
}


void Scene::update(float dt) {

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        (*it)->update(dt);

	mSkySphere->update(dt);

    if(mInterpolationTimer <= maxInterpolationTime && mInterpolationTimer > 0.0f) {
        mInterpolationTimer += dt;

        interpolateBackground();
    }
}

void Scene::interpolateBackground() {
    if(mToggledBackground == false) { //day
        mSkySphere->setBrightness(mInterpolationTimer / maxInterpolationTime);
    } else if(mToggledBackground == true) { //night
        if(mSkySphere->getBrightness() > 0.15) {
            mSkySphere->setBrightness(1 - (mInterpolationTimer / maxInterpolationTime));
        }
    }

}


void Scene::randomizeStartingPositions() {

	for (std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it) {
		if (it != mLevels.begin()){
			float angle = (*it)->randomizeAngle(20.0f, 270.0f);
			(*it)->setStartingAngle(angle);
		}
	}

}

void Scene::resetStartingPositions() {
	for (std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
		(*it)->setStartingAngle(0.0f);
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

void Scene::shallRenderLetter(Word word, bool shallRender) {

    for(std::vector<Letter *>::iterator it = mWords[word].begin(); it != mWords[word].end(); ++it)
        (*it)->setRenderState(shallRender);
}

void Scene::toggleBackground() {
    mInterpolationTimer = 0.1f;

    if(mToggledBackground) {
        mToggledBackground = false;
    } else {
        mToggledBackground = true;
    }
}
