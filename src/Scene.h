#ifndef SCENE_H
#define SCENE_H

#include "Level.h"
#include "Character.h"
#include "SkySphere.h"

class Scene {

public:

    Scene(unsigned int);
    
    ~Scene();

    void initialize();

    void render(std::vector<glm::mat4>);

    void update(float);

    void addLevel(Level *l) { mLevels.push_back(l); }

    void addLevelForce(unsigned int, float);

    std::vector<float> getLevelAngles();

    std::vector<glm::vec4> getLevelColors();

    float getLevelAngle(unsigned int i) { return mLevels[i]->getAngle(); }

    std::vector<Level *> getLevels() { return mLevels; }

    Level *getLevel(unsigned int i) { return mLevels[i]; }

    int getNumberOfLevels() { return mLevels.size(); }

    Character *getCharacter() { return mCharacter; }

    void setLevelAngle(unsigned int i, float a) { mLevels[i]->setAngle(a); }

private:

    std::vector<Level *> mLevels;
    
    Character * mCharacter;
	SkySphere * mSkySphere;

    glm::vec3 mLightSourcePosition;

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