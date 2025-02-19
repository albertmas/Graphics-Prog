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
	Shader geoTextShader();
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
	//fogShader.init("..\\res\\fogShader.vert", "..\\res\\fogShader.frag"); //Lab1 shader
	//toonShader.init("..\\res\\shaderToon.vert", "..\\res\\shaderToon.frag"); //Lab2 shader
	fogToonShader.init("..\\res\\shaderFogToon.vert", "..\\res\\shaderFogToon.frag"); //Lab1 + Lab2 shader
	//rimLightingShader.init("..\\res\\shaderRimLighting.vert", "..\\res\\shaderRimLighting.frag"); //Lab3 shader
	//fogToonRimShader.init("..\\res\\shaderFogToonRim.vert", "..\\res\\shaderFogToonRim.frag"); //Lab1 + Lab2 + Lab3 shader
	geoTextShader.init("..\\res\\shaderGeoText.vert", "..\\res\\shaderGeoText.geom", "..\\res\\shaderGeoText.frag"); //Lab5 shader
	explosionShader.init("..\\res\\shaderExplosion.vert", "..\\res\\shaderExplosion.geom", "..\\res\\shaderExplosion.frag"); //Lab5 shader

	
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

void MainGame::linkFogShader()
{
	//fogShader.setMat4("u_pm", myCamera.getProjection());
	//fogShader.setMat4("u_vm", myCamera.getProjection());
	fogShader.setFloat("maxDist", 20.0f);
	fogShader.setFloat("minDist", 0.0f);
	fogShader.setVec3("fogColor", glm::vec3(0.0f, 0.0f, 0.0f));
}

void MainGame::linkToonShader()
{
	toonShader.setVec3("lightDir", glm::vec3(0.3f, 0.3f, 0.3f));
}

void MainGame::linkFogToonShader()
{
	fogToonShader.setVec3("lightDir", glm::vec3(0.3f, 0.3f, 0.3f));
	fogToonShader.setFloat("maxDist", 20.0f);
	fogToonShader.setFloat("minDist", 0.0f);
	fogToonShader.setVec3("fogColor", glm::vec3(0.0f, 0.0f, 0.0f));
}

void MainGame::linkRimLightingShader()
{
	glm::vec3 direction = myCamera.getPos() - myCamera.getForward();
	rimLightingShader.setVec3("viewDir", direction);
}

void MainGame::linkFogToonRimShader()
{
	glm::vec3 direction = myCamera.getPos() - myCamera.getForward();
	fogToonRimShader.setVec3("viewDir", direction);
	fogToonRimShader.setVec3("lightDir", glm::vec3(0.3f, 0.3f, 0.3f));
	fogToonRimShader.setFloat("maxDist", 20.0f);
	fogToonRimShader.setFloat("minDist", 0.0f);
	fogToonRimShader.setVec4("fogColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void MainGame::linkGeoTextShader()
{
	geoTextShader.setFloat("time", counter);
	geoTextShader.setFloat("randColourX", rand() % 10 * 0.1f);
	geoTextShader.setFloat("randColourY", rand() % 10 * 0.1f);
	geoTextShader.setFloat("randColourZ", rand() % 10 * 0.1f);
}

void MainGame::linkExplosionShader()
{
	explosionShader.setFloat("time", counter);
}

void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);

	Texture texture("..\\res\\bricks.jpg"); //load texture
	Texture texture1("..\\res\\water.jpg"); //load texture
	
	transform.SetPos(glm::vec3(sinf(counter), 1.0, 1.5));
	transform.SetRot(glm::vec3(0.0, 0.0, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	explosionShader.Bind(); // Bind shader
	linkExplosionShader();
	explosionShader.Update(transform, myCamera); // Apply shader to mesh 1
	texture.Bind(0);
	mesh1.draw();
	mesh1.updateSphereData(*transform.GetPos(), 0.62f);
	

	transform.SetPos(glm::vec3(-sinf(counter), -0.5, 10.0 + (-sinf(counter) * 8)));
	transform.SetRot(glm::vec3(0.0, 0.0, counter * 5));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	fogToonShader.Bind(); // Bind shader
	linkFogToonShader();
	fogToonShader.Update(transform, myCamera); // Apply shader to mesh 2
	mesh2.draw();
	mesh2.updateSphereData(*transform.GetPos(), 0.62f);
	counter = counter + 0.02f;

				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	_gameDisplay.swapBuffer();
}
