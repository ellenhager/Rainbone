#ifndef SCENE_H
#define SCENE_H

#include "Level.h"
#include "Character.h"

class Scene {

public:

    Scene();
    
    ~Scene();

    void initialize();

    void render(std::vector<glm::mat4>);

    void addLevel(Level *l) { mLevels.push_back(l); }

    void addLevelForce(unsigned int, float);

    std::vector<float> getLevelAngles();

    Level *getLevel(unsigned int i) { return mLevels[i]; }

    int getNumberOfLevels() { return mLevels.size(); }

private:

    std::vector<Level *> mLevels;
    
    Character * mCharacter;

    glm::vec3 mLightSourcePosition;
};

#endif // SCENE_H