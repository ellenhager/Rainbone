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

    mWords[START].push_back(new Letter("../assets/objs/start.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));

    mWords[FIVE].push_back(new Letter("../assets/objs/5.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));

    mWords[FOUR].push_back(new Letter("../assets/objs/4.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));

    mWords[THREE].push_back(new Letter("../assets/objs/3.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));

    mWords[TWO].push_back(new Letter("../assets/objs/2.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));

    mWords[ONE].push_back(new Letter("../assets/objs/1.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f)));

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

    mWords[FIVE].front()->translate(glm::vec3(0.0f, -1.0f, 0.0f));
    mWords[FOUR].front()->translate(glm::vec3(0.0f, -1.0f, 0.0f));
    mWords[THREE].front()->translate(glm::vec3(0.0f, -1.0f, 0.0f));
    mWords[TWO].front()->translate(glm::vec3(0.0f, -1.0f, 0.0f));
    mWords[ONE].front()->translate(glm::vec3(0.0f, -1.0f, 0.0f));

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

	mCharacter->update(dt);

    if(mInterpolationTimer <= maxInterpolationTime && mInterpolationTimer > 0.0f) {
        mInterpolationTimer += dt;

        interpolateBackground();
    }

    for(std::map<Word, std::vector<Letter *> >::iterator it = mWords.begin(); it != mWords.end(); ++it) {
        for(std::vector<Letter *>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
            if((*it2)->shallRender() && !(*it2)->isComplete())
                (*it2)->interpolateLetter(dt);
        }
    }
}



void Scene::interpolateBackground() {
    if(mSkyState == DAY) { //day
        mSkySphere->setBrightness(mInterpolationTimer / maxInterpolationTime);
    } else if(mSkyState == NIGHT) { //night
        if(mSkySphere->getBrightness() > 0.2) {
            mSkySphere->setBrightness(1 - (mInterpolationTimer / maxInterpolationTime));
        }
	} else if (mSkyState == BLACK) {
		if (mSkySphere->getBrightness() > 0.0) {
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

void Scene::zoomLevels() {
    for (std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        (*it)->zoom();
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


std::vector<float> Scene::getLevelTranslations() {

    std::vector<float> translations;

    for(std::vector<Level *>::iterator it = mLevels.begin(); it != mLevels.end(); ++it)
        translations.push_back((*it)->getLevelTrans());

    return translations;
}


std::vector<std::pair<bool, bool> > Scene::getLetterStates() {

    std::vector<std::pair<bool, bool> > letterStates;

    for(std::map<Word, std::vector<Letter *> >::iterator it = mWords.begin(); it != mWords.end(); ++it) {
        for(std::vector<Letter *>::iterator it2 = (*it).second.begin(); it2 != (*it).second.end(); ++it2) {
            letterStates.push_back(std::make_pair( (*it2)->shallRender(), (*it2)->isComplete() ));
        }
    }
    return letterStates;
}


void Scene::shallRenderLetter(Word word, bool shallRender) {

    for(std::vector<Letter *>::iterator it = mWords[word].begin(); it != mWords[word].end(); ++it)
        (*it)->setRenderState(shallRender);
}

void Scene::setDay() { 
	mInterpolationTimer = 0.01f;
	mSkyState = DAY; 
}

void Scene::setNight() { 
	mInterpolationTimer = 0.01f;
	mSkyState = NIGHT;
};

void Scene::setBlack() { 
	mInterpolationTimer = 0.01f;
	mSkyState = BLACK; 
};
