#ifndef SCENE_H
#define SCENE_H


#include "Level.h"
#include "Character.h"

class Scene {

public:

    Scene();
    
    ~Scene();

    void initialize();

    void render();

    void addLevel(Level *l) { mLevels.push_back(l); }

    void addLevelForce(unsigned int, float);

    std::vector<float> getLevelAngles();

private:

    std::vector<Level *> mLevels;
    
    Character * mCharacter;

    glm::vec3 lightSourcePosition;
};

#endif // SCENE_H