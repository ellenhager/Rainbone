#include "Level.h"

void myDrawFun();
void myPreSyncFun();
void myInitOGLFun();
void myEncodeFun();
void myDecodeFun();
void myCleanUpFun();

sgct::Engine * gEngine;
Level * level;

//variables to share across cluster
sgct::SharedDouble curr_time(0.0);

int main( int argc, char* argv[] )
{
	gEngine = new sgct::Engine( argc, argv );

	gEngine->setInitOGLFunction( myInitOGLFun );
	gEngine->setDrawFunction( myDrawFun );
	gEngine->setPreSyncFunction( myPreSyncFun );
	gEngine->setCleanUpFunction( myCleanUpFun );

	if( !gEngine->init( sgct::Engine::OpenGL_3_3_Core_Profile ) )
	{
		delete gEngine;
		return EXIT_FAILURE;
	}

	sgct::SharedData::instance()->setEncodeFunction(myEncodeFun);
	sgct::SharedData::instance()->setDecodeFunction(myDecodeFun);

	// Main loop
	gEngine->render();

	// Clean up
	delete gEngine;

	// Exit program
	exit( EXIT_SUCCESS );
}

void myDrawFun()
{

	std::vector<glm::mat4> sceneMatrices;

	sceneMatrices.push_back(gEngine->getCurrentModelViewProjectionMatrix());
	sceneMatrices.push_back(gEngine->getCurrentModelViewMatrix());
	sceneMatrices.push_back(gEngine->getCurrentModelViewMatrix());
	sceneMatrices.push_back(glm::inverseTranspose(glm::mat4(gEngine->getCurrentModelViewMatrix())));

	level->render(sceneMatrices, static_cast<float>(curr_time.getVal()));
}

void myPreSyncFun()
{
	if( gEngine->isMaster() )
	{
		curr_time.setVal( sgct::Engine::getTime() );
	}
}

void myInitOGLFun()
{

	level = new Level("assets/simple.obj", glm::vec4(0.8f, 0.2f, 0.2f, 1.0f));

	glm::vec3 lightPosition = glm::vec3(0.0f, 0.0f, 0.0f);

	level->initialize(lightPosition);
}

void myEncodeFun()
{
	sgct::SharedData::instance()->writeDouble(&curr_time);
}

void myDecodeFun()
{
	sgct::SharedData::instance()->readDouble(&curr_time);
}

void myCleanUpFun() {

}
