#ifndef SCENE_H
#define SCENE_H

#include <map>

#include "Level.h"
#include "Character.h"
#include "SkySphere.h"
#include "Utils.h"
#include "Letter.h"

class Scene {

public:

    Scene(unsigned int);

    ~Scene();

    void initialize();

    void render(std::vector<glm::mat4>);

    void update(float);

    void addLevel(Level *l) { mLevels.push_back(l); }

	void randomizeStartingPositions();

	void resetStartingPositions();

    std::vector<float> getLevelAngles();

    std::vector<glm::vec4> getLevelColors();

    std::vector<float> getLevelTranslations();

    float getSkySphereBrightness() { return mSkySphere->getBrightness(); }

    float getSkySphereAngle() { return mSkySphere->getAngle(); }

    std::vector<std::pair<bool, bool> > getLetterStates();

    float getLevelAngle(unsigned int i) { return mLevels[i]->getAngle(); }

    std::vector<Level *> getLevels() { return mLevels; }

    Level *getLevel(unsigned int i) { return mLevels[i]; }

    Letter *getLetter(Word w) { return mWords[w].front(); }

    int getNumberOfLevels() { return mLevels.size(); }

    int getNumberOfWords() { return mWords.size(); }

    Character *getCharacter() { return mCharacter; }

    Character *getGift() { return mGift; }

    void setLevelAngle(unsigned int i, float a) { mLevels[i]->setAngle(a); }

    void setSkySphereBrightness(float b) { mSkySphere->setBrightness(b); }

    void setSkySphereAngle(float a) { mSkySphere->setAngle(a); }

    void shallRenderLetter(Word, bool);

    bool isLetterRendering(Word w) { return mWords[w].front()->shallRender(); }

    bool isLetterComplete(Word w) { return mWords[w].front()->isComplete(); }

    void setWordStatic(Word w, bool b) { mWords[w].front()->setStatic(b); }

    void setWordComplete(Word w) { mWords[w].front()->setComplete(); }

    void setWordIncomplete(Word w) { mWords[w].front()->setIncomplete(); }

    void toggleBackground();

    void interpolateBackground();

    void zoomLevels();

	void setDay();

	void setNight();

	void setBlack();

private:

	enum SKYSTATE {DAY, NIGHT, BLACK};

	SKYSTATE mSkyState = DAY;

    std::vector<Level *> mLevels;

    std::map<Word, std::vector<Letter *> > mWords;

    Character * mCharacter;

    Character * mGift;

	SkySphere * mSkySphere;

    glm::vec3 mLightSourcePosition;

    //bool mToggledBackground = false;
    float mInterpolationTimer = 0.0f;
    const float maxInterpolationTime = 1.0f;

    const glm::vec4 sColorScale[7] = {
        glm::vec4(0.8f, 0.2f, 0.2f, 1.0f),
        glm::vec4(0.8f, 0.6f, 0.2f, 1.0f),
        glm::vec4(0.8f, 0.8f, 0.2f, 1.0f),
        glm::vec4(0.2f, 0.8f, 0.2f, 1.0f),
        glm::vec4(0.2f, 0.8f, 0.8f, 1.0f),
        glm::vec4(0.2f, 0.2f, 0.8f, 1.0f),
        glm::vec4(0.8f, 0.2f, 0.8f, 1.0f)
    };
};

#endif // SCENE_H
