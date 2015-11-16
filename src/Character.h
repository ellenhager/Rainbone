#ifndef CHARACTER_H
#define CHARACTER_H

#define I_MVP 0
#define I_MV 1
#define I_MV_LIGHT 2
#define I_NM 3

#include <iostream>
#include <math.h>
#include <sgct.h>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "objloader.hpp"

class Character {

public:

    Character(glm::vec3, const char *, glm::vec4);
    ~Character();

    void initialize(glm::vec3);
    void render(std::vector<glm::mat4>);

private:

    glm::vec3 convertToCartesian();
    glm::vec3 convertToSpherical(glm::vec3);

    //glm::mat4 mTransform;

    // Position is in spherical coordinates, i.e. (radius, theta, phi)
    glm::vec3 mPosition;

    // Lists containing vertices and normals
    std::vector<glm::vec3> mVerts;
    std::vector<glm::vec3> mNormals;

    // Shader data
    GLuint vertexArray;
    GLuint vertexBuffer;
    GLuint normalBuffer;

    GLint MVPLoc;           // MVP matrix
    GLint MVLoc;            // MV matrix
    GLint MVLightLoc;       // MVLight matrix
    GLint NMLoc;            // NM matrix
    GLint lightPosLoc;      // Light position
    GLint colorLoc;         // Color
    GLint lightAmbLoc;      // Ambient light
    GLint lightDifLoc;      // Diffuse light
    GLint lightSpeLoc;      // Specular light
    GLint specularityLoc;   // Specular constant
    GLint shinynessLoc;     // How much specularity (magnitude)

    struct Material {
        glm::vec4 color;
        glm::vec4 ambient;
        glm::vec4 diffuse;
        glm::vec4 specular;
        float specularity;
        float shinyness;
    } mMaterial;

};

#endif // CHARACTER_H