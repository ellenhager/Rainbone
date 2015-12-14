#include "Character.h"

Character::Character(const char * objPath, std::string tn)
    : mTextureName(tn) {

    std::cout << "Creating Character..." << std::endl;

    loadObj(objPath, mVertices, mUvs, mNormals);

    mMaterial.color         = glm::vec4(0.3f, 0.7f, 0.7f, 1.0f);
    mMaterial.ambient       = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    mMaterial.diffuse       = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    mMaterial.specular      = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mMaterial.specularity   = 50.0f;
    mMaterial.shinyness     = 0.6f;

}


Character::~Character() {

    std::cout << "Destroying Character...";

    mVertices.clear();
    mVertices.shrink_to_fit();

    mNormals.clear();
    mNormals.shrink_to_fit();

    mUvs.clear();
    mUvs.shrink_to_fit();

    std::cout << "\tDone!\n";
}


void Character::initialize(glm::vec3 lightSourcePosition) {

    std::cout << "\nInitializing Character...";

    sgct::TextureManager::instance()->setAnisotropicFilterSize(8.0f);
    sgct::TextureManager::instance()->setCompression(sgct::TextureManager::S3TC_DXT);
    sgct::TextureManager::instance()->loadTexure("characterTexture", "../assets/textures/char_texture.png", true);

    if(!sgct::ShaderManager::instance()->shaderProgramExists("character")) {
        sgct::ShaderManager::instance()->addShaderProgram( "character",
            "../shaders/texturedphongvertexshader.glsl",
            "../shaders/texturedphongfragmentshader.glsl" );
    }

    sgct::ShaderManager::instance()->bindShaderProgram( "character" );

    MVPLoc                  = sgct::ShaderManager::instance()->getShaderProgram( "character").getUniformLocation( "MVP" );
    MVLoc                   = sgct::ShaderManager::instance()->getShaderProgram( "character").getUniformLocation( "MV" );
    MVLightLoc              = sgct::ShaderManager::instance()->getShaderProgram( "character").getUniformLocation( "MV_Light" );
    NMLoc                   = sgct::ShaderManager::instance()->getShaderProgram( "character").getUniformLocation( "NM" );
    lightPosLoc             = sgct::ShaderManager::instance()->getShaderProgram( "character").getUniformLocation( "lightPosition" );
    colorLoc                = sgct::ShaderManager::instance()->getShaderProgram( "character").getUniformLocation( "color" );
    lightAmbLoc             = sgct::ShaderManager::instance()->getShaderProgram( "character").getUniformLocation( "ambientColor" );
    lightDifLoc             = sgct::ShaderManager::instance()->getShaderProgram( "character").getUniformLocation( "diffuseColor" );
    lightSpeLoc             = sgct::ShaderManager::instance()->getShaderProgram( "character").getUniformLocation( "specularColor" );
    specularityLoc          = sgct::ShaderManager::instance()->getShaderProgram( "character").getUniformLocation( "specularity" );
    shinynessLoc            = sgct::ShaderManager::instance()->getShaderProgram( "character").getUniformLocation( "shinyness" );
    TexLoc                  = sgct::ShaderManager::instance()->getShaderProgram( "character").getUniformLocation( "tex" );

    glUniform4f(lightPosLoc, lightSourcePosition.x, lightSourcePosition.y, lightSourcePosition.z, 1.0f);

    sgct::ShaderManager::instance()->unBindShaderProgram();

    // Generate the VAO
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    // Generate VBO for vertex positions
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Upload vertex data to GPU
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), &mVertices[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        reinterpret_cast<void*>(0)
    );

    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    // Upload normal data to GPU
    glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), &mNormals[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        3,
        GL_FLOAT,
        GL_FALSE,
        0,
        reinterpret_cast<void*>(0)
    );

    glGenBuffers(1, &textureBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
    // Upload uv data to GPU
    glBufferData(GL_ARRAY_BUFFER, mUvs.size() * sizeof(glm::vec2), &mUvs[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(
        2,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        reinterpret_cast<void*>(0)
    );

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::cout << "\tDone!\n";
}


void Character::render(std::vector<glm::mat4> sceneMatrices) {

    // Enable backface culling and depth test, we dont want to draw unnecessary stuff
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Dome tilt
    float tilt = M_PI * 27.0f / 180.0f;
    glm::mat4 characterTransform = glm::rotate(glm::mat4(1.0f), tilt, glm::vec3(-1.0f, 0.0f, 0.0f));
	
    // Character transform (spherical coordinates: https://en.wikipedia.org/wiki/Spherical_coordinate_system)
    characterTransform = glm::rotate(characterTransform, static_cast<float>(M_PI * mPhi / 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    characterTransform = glm::rotate(characterTransform, static_cast<float>(M_PI * -mTheta / 180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    characterTransform = glm::translate(characterTransform, glm::vec3(0.0f, 0.0f, mRadius));

    // Align to center of dome
    characterTransform = glm::rotate(characterTransform, static_cast<float>(M_PI * 90.0f / 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    characterTransform = glm::rotate(characterTransform, static_cast<float>(M_PI * 90.0f / 180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    characterTransform = glm::rotate(characterTransform, static_cast<float>(M_PI * 90.0f / 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	characterTransform = glm::scale(characterTransform, glm::vec3(1.0f, 1.0f, -1.0f));
    // Apply scene transforms to MVP and MV matrices
    sceneMatrices[I_MVP] = sceneMatrices[I_MVP] * characterTransform;
    sceneMatrices[I_MV]  = sceneMatrices[I_MV]  * characterTransform;

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sgct::TextureManager::instance()->getTextureId("characterTexture"));

    // Bind shader program
    sgct::ShaderManager::instance()->bindShaderProgram("character");

    // Set uniform values, so that we have some data to work with in the shaders
    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &sceneMatrices[I_MVP][0][0]);
    glUniformMatrix4fv(MVLoc, 1, GL_FALSE, &sceneMatrices[I_MV][0][0]);
    glUniformMatrix4fv(MVLightLoc, 1, GL_FALSE, &sceneMatrices[I_MV_LIGHT][0][0]);
    glUniformMatrix4fv(NMLoc, 1, GL_FALSE, &sceneMatrices[I_NM][0][0]);
    glUniform4f(colorLoc, mMaterial.color[0], mMaterial.color[1], mMaterial.color[2], mMaterial.color[3]);
    glUniform4f(lightAmbLoc, mMaterial.ambient[0], mMaterial.ambient[1], mMaterial.ambient[2], mMaterial.ambient[3]);
    glUniform4f(lightDifLoc, mMaterial.diffuse[0], mMaterial.diffuse[1], mMaterial.diffuse[2], mMaterial.diffuse[3]);
    glUniform4f(lightSpeLoc, mMaterial.specular[0], mMaterial.specular[1], mMaterial.specular[2], mMaterial.specular[3]);
    glUniform1f(specularityLoc, mMaterial.specularity);
    glUniform1f(shinynessLoc, mMaterial.shinyness);

    // Rebind the buffer data, since our vertices are now updated
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size() * sizeof(glm::vec3), &mVertices[0], GL_STATIC_DRAW);

    // Rebind the buffer data, normals are now updated
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), &mNormals[0], GL_STATIC_DRAW);

    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, mVertices.size());

    // Unbind vertex array
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    sgct::ShaderManager::instance()->unBindShaderProgram();

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
}
