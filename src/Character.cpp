#include "Character.h"


Character::Character(glm::vec3 p, const char *objPath, glm::vec4 c)
    : mPosition(p) {

    // Load obj file
    loadOBJ(objPath, mVerts, mNormals);

    // Set material
    mMaterial.color         = c;
    mMaterial.ambient       = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
    mMaterial.diffuse       = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
    mMaterial.specular      = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mMaterial.specularity   = 50.0f;
    mMaterial.shinyness     = 0.6f;

}


Character::~Character() {

    mVerts.clear();
    mVerts.shrink_to_fit();

    mNormals.clear();
    mNormals.shrink_to_fit();
}


void Character::initialize(glm::vec3 lightPosition) {

    //Set up backface culling
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW); //our polygon winding is counter clockwise

    sgct::ShaderManager::instance()->addShaderProgram( "character",
            "shaders/SimpleVertexShader.vertexshader",
            "shaders/SimpleFragmentShader.fragmentshader" );

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

    sgct::ShaderManager::instance()->unBindShaderProgram();

    // Generate the VAO
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    // Generate VBO for vertex positions
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    
    // Upload data to GPU
    glBufferData(GL_ARRAY_BUFFER, mVerts.size() * sizeof(glm::vec3), &mVerts[0], GL_STATIC_DRAW);
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
        1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
        3,                  // size
        GL_FLOAT,           // type
        GL_FALSE,           // normalized?
        0,                  // stride
        reinterpret_cast<void*>(0) // array buffer offset
    );

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Character::render(std::vector<glm::mat4> matrices) {

    // Enable backface culling and depth test, we dont want to draw unnecessary stuff
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );

    // Angle in radians, glm states that the rotation is using angle in degrees, but it's a lie, use radians instead.
    float tilt = M_PI * 27.0f / 180.0f;
    // Create scene transform (animation)
    glm::mat4 scene_mat = glm::rotate( glm::mat4(1.0f), tilt , glm::vec3(-1.0f, 0.0f, 0.0f));
    //glm::mat4 rot = glm::rotate( glm::mat4(1.0f), static_cast<float>(mAngle * M_PI / 180.0f) , glm::vec3(0.0f, 1.0f, 0.0f));
    //glm::mat4 scene_mat = glm::rotate(glm::mat4(1.0f), );

    // Apply scene transforms to MVP and MV matrices
    matrices[I_MVP] = matrices[I_MVP] * scene_mat;
    matrices[I_MV]  = matrices[I_MV]  * scene_mat;

    // Bind shader program
    sgct::ShaderManager::instance()->bindShaderProgram( "character" );

    // Set uniform values, so that we have some data to work with in the shaders
    glUniformMatrix4fv(MVPLoc, 1, GL_FALSE, &matrices[I_MVP][0][0]);
    glUniformMatrix4fv(MVLoc, 1, GL_FALSE, &matrices[I_MV][0][0]);
    glUniformMatrix4fv(MVLightLoc, 1, GL_FALSE, &matrices[I_MV_LIGHT][0][0]);
    glUniformMatrix4fv(NMLoc, 1, GL_FALSE, &matrices[I_NM][0][0]);
    glUniform4f(colorLoc, mMaterial.color[0], mMaterial.color[1], mMaterial.color[2], mMaterial.color[3]);
    glUniform4f(lightAmbLoc, mMaterial.ambient[0], mMaterial.ambient[1], mMaterial.ambient[2], mMaterial.ambient[3]);
    glUniform4f(lightDifLoc, mMaterial.diffuse[0], mMaterial.diffuse[1], mMaterial.diffuse[2], mMaterial.diffuse[3]);
    glUniform4f(lightSpeLoc, mMaterial.specular[0], mMaterial.specular[1], mMaterial.specular[2], mMaterial.specular[3]);
    glUniform1f(specularityLoc, mMaterial.specularity);
    glUniform1f(shinynessLoc, mMaterial.shinyness);

    // Rebind the buffer data, since our vertices are now updated
    glBindVertexArray(vertexArray);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, mVerts.size() * sizeof(glm::vec3), &mVerts[0], GL_STATIC_DRAW);

    // Rebind the buffer data, normals are now updated
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, mNormals.size() * sizeof(glm::vec3), &mNormals[0], GL_STATIC_DRAW);

    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, mVerts.size());

    // Unbind vertex array
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    sgct::ShaderManager::instance()->unBindShaderProgram();

    glDisable( GL_CULL_FACE );
    glDisable( GL_DEPTH_TEST );
}


glm::vec3 Character::convertToSpherical(glm::vec3 position) {

    mPosition.x = sqrt((position.x * position.x) + (position.y * position.y) + (position.z * position.z));
    mPosition.y = acos(mPosition.z / (mPosition.x));
    mPosition.z = atan2(mPosition.x, mPosition.y);

    return glm::vec3(0.0f);
}

/*glm::vec3 Character::convertToCartesian() {

}*/