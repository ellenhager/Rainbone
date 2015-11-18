#include "Level.h"

Level::Level(const char * objPath, glm::vec4 c) {

    std::cout << "Creating Level..." << std::endl;

    loadOBJ(objPath, mVertices, mNormals);

    mMaterial.color         = c;
    mMaterial.ambient       = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    mMaterial.diffuse       = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    mMaterial.specular      = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mMaterial.specularity   = 50.0f;
    mMaterial.shinyness     = 0.6f;
}


Level::~Level() {

    std::cout << "Destroying Level...";

    mVertices.clear();
    mVertices.shrink_to_fit();

    mNormals.clear();
    mNormals.shrink_to_fit();

    std::cout << "\tDone!\n";
}


void Level::initialize(glm::vec3 lightSourcePosition) {

    std::cout << "\nInitializing Level...";

    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    if(!sgct::ShaderManager::instance()->shaderProgramExists("level")) {
        sgct::ShaderManager::instance()->addShaderProgram( "level",
            "../shaders/phongvertexshader.glsl",
            "../shaders/phongfragmentshader.glsl" );
    }

    sgct::ShaderManager::instance()->bindShaderProgram( "level" );

    MVPLoc                  = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "MVP" );
    MVLoc                   = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "MV" );
    MVLightLoc              = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "MV_Light" );
    NMLoc                   = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "NM" );
    lightPosLoc             = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "lightPosition" );
    colorLoc                = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "color" );
    lightAmbLoc             = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "ambientColor" );    
    lightDifLoc             = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "diffuseColor" );
    lightSpeLoc             = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "specularColor" );
    specularityLoc          = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "specularity" );
    shinynessLoc            = sgct::ShaderManager::instance()->getShaderProgram( "level").getUniformLocation( "shinyness" );

    sgct::ShaderManager::instance()->unBindShaderProgram();

    // Generate the VAO
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    // Generate VBO for vertex positions
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Upload data to GPU
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
    // Upload vertex data to GPU
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

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    std::cout << "\tDone!\n";
}


void Level::render(std::vector<glm::mat4> sceneMatrices) {

     // Enable backface culling and depth test, we dont want to draw unnecessary stuff
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Angle in radians, glm states that the rotation is using angle in degrees, but it's a lie, use radians instead.
    float tilt = M_PI * 27.0f / 180.0f;
    // Create scene transform (animation)
    glm::mat4 levelTransform = glm::rotate( glm::mat4(1.0f), tilt , glm::vec3(-1.0f, 0.0f, 0.0f));
    glm::mat4 levelRotation = glm::rotate( glm::mat4(1.0f), static_cast<float>(mAngle * M_PI / 180.0f) , glm::vec3(0.0f, 1.0f, 0.0f));

    // Apply scene transforms to MVP and MV matrices
    sceneMatrices[I_MVP] = sceneMatrices[I_MVP] * levelTransform * levelRotation;
    sceneMatrices[I_MV]  = sceneMatrices[I_MV]  * levelTransform;

    // Bind shader program
    sgct::ShaderManager::instance()->bindShaderProgram( "level" );

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

    glDisable( GL_CULL_FACE );
    glDisable( GL_DEPTH_TEST );
}