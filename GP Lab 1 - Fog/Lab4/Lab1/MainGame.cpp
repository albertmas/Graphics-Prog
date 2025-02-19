#include "MainGame.h"
#include "Camera.h"
#include <iostream>
#include <string>


Transform transform;

MainGame::MainGame()
{
	_gameState = GameState::PLAY;
	Display* _gameDisplay = new Display(); //new display
    Mesh* mesh1();
	Mesh* mesh2();
	Shader fog();
	//Audio* audioDevice();
}

MainGame::~MainGame()
{
}

void MainGame::run()
{
	initSystems(); 
	gameLoop();
}

void MainGame::initSystems()
{
	_gameDisplay.initDisplay(); 
	//whistle = audioDevice.loadSound("..\\res\\bang.wav");
	//backGroundMusic = audioDevice.loadSound("..\\res\\background.wav");
	
	mesh1.loadModel("..\\res\\monkey3.obj");
	mesh2.loadModel("..\\res\\monkey3.obj");
	fog.Init("..\\res\\fog");
	
	myCamera.initCamera(glm::vec3(0, 0, -5), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;
}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
		collision(mesh1.getSpherePos(), mesh1.getSphereRadius(), mesh2.getSpherePos(), mesh2.getSphereRadius());
		//playAudio(backGroundMusic, glm::vec3(0.0f,0.0f,0.0f));
	}
}

void MainGame::processInput()
{
	SDL_Event evnt;

	while(SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
			case SDL_QUIT:
				_gameState = GameState::EXIT;
				break;
		}
	}
	
}


bool MainGame::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = glm::sqrt((m2Pos.x - m1Pos.x)*(m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y)*(m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z)*(m2Pos.z - m1Pos.z));

	if (distance < (m1Rad + m2Rad))
	{
		//audioDevice.setlistener(myCamera.getPos(), m1Pos); //add bool to mesh
		//playAudio(whistle, m1Pos);
		return true;
	}
	else
	{
		return false;
	}
}

void MainGame::setFogShaderVariables()
{
	//fog.setVec3("lightDir", glm::vec3(0.5, 0.5, 0.5));

	//fog.setMat4("u_vm", myCamera.GetView());
	//fog.setMat4("u_pm", myCamera.GetProjection());

	fog.setVec3("fogColor", glm::vec3(0.0, 0.0, 0.0));
	fog.setFloat("minDist", 0.0f);
	fog.setFloat("maxDist", 20.0f);

}

//void MainGame::playAudio(unsigned int Source, glm::vec3 pos)
//{
//	
//	ALint state; 
//	alGetSourcei(Source, AL_SOURCE_STATE, &state);
//	/*
//	Possible values of state
//	AL_INITIAL
//	AL_STOPPED
//	AL_PLAYING
//	AL_PAUSED
//	*/
//	if (AL_PLAYING != state)
//	{
//		audioDevice.playSound(Source, pos);
//	}
//}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	setFogShaderVariables();
	
	Texture texture("..\\res\\bricks.jpg"); //load texture
	Texture texture1("..\\res\\water.jpg"); //load texture
	
	transform.SetPos(glm::vec3(sinf(counter), 0.5, 0.0));
	transform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	fog.Bind();
	fog.Update(transform, myCamera);
	texture.Bind(0);
	mesh1.draw();
	mesh1.updateSphereData(*transform.GetPos(), 0.62f);
	

	transform.SetPos(glm::vec3(-sinf(counter), -0.5, 10 -sinf(counter)*15));
	transform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	fog.Update(transform, myCamera);

	texture.Bind(0);
	mesh2.draw();
	mesh2.updateSphereData(*transform.GetPos(), 0.62f);
	counter = counter + 0.05f;

				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
} 