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
	Mesh* mesh3();
	Audio* audioDevice();
	Texture* texture(); //load texture
	Overlay* overlay(); //load texture
	Shader* shaderPass();
	Shader* shaderBlur();
	Shader* shaderToon();
	Shader* shaderRim();
	Shader* shaderFogToonRim();
	Shader* shaderExplosion();
	Shader* shaderReflection();
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
	texture.init("..\\res\\stone.jpg"); //load texture


	shaderSkybox.init("..\\res\\shaderSkybox.vert", "..\\res\\shaderSkybox.frag");

	shaderFogToonRim.init("..\\res\\shaderFogToonRim.vert", "..\\res\\shaderFogToonRim.frag");
	shaderExplosion.init("..\\res\\shaderExplosion.vert", "..\\res\\shaderExplosion.geom", "..\\res\\shaderExplosion.frag");
	shaderReflection.init("..\\res\\shaderReflection.vert", "..\\res\\shaderReflection.frag");

	overlay.init("..\\res\\bricks.jpg");

	mesh1.loadModel("..\\res\\monkey3.obj");
	mesh2.loadModel("..\\res\\pin.obj");
	mesh3.loadModel("..\\res\\golem.obj");
	
	myCamera.initCamera(glm::vec3(0, 0, -10.0), 70.0f, (float)_gameDisplay.getWidth()/_gameDisplay.getHeight(), 0.01f, 1000.0f);

	counter = 1.0f;

	vector<std::string> faces
	{
		"..\\res\\skybox\\right.jpg",
		"..\\res\\skybox\\left.jpg",
		"..\\res\\skybox\\top.jpg",
		"..\\res\\skybox\\bottom.jpg",
		"..\\res\\skybox\\front.jpg",
		"..\\res\\skybox\\back.jpg"
	};
	cubemapTexture = skybox.loadCubemap(faces); //Load the cubemap using "faces" into cubemapTextures

	float skyboxVertices[] = {
		// positions          
		-6.0f,  6.0f, -6.0f,
		-6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f, -6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,

		-6.0f, -6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f, -6.0f,  6.0f,
		-6.0f, -6.0f,  6.0f,

		-6.0f,  6.0f, -6.0f,
		6.0f,  6.0f, -6.0f,
		6.0f,  6.0f,  6.0f,
		6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f,  6.0f,
		-6.0f,  6.0f, -6.0f,

		-6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f, -6.0f,
		6.0f, -6.0f, -6.0f,
		-6.0f, -6.0f,  6.0f,
		6.0f, -6.0f,  6.0f
	};

	//use openGL functionality to generate & bind data into buffers
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}

void MainGame::gameLoop()
{
	while (_gameState != GameState::EXIT)
	{
		processInput();
		drawGame();
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

void MainGame::Skybox()
{
	glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
	shaderSkybox.Bind();
	shaderSkybox.setInt("skybox", 0);
	//view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
	shaderSkybox.setMat4("view", myCamera.GetView());
	shaderSkybox.setMat4("projection", myCamera.GetProjection());
	// skybox cube
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS); // set depth function back to default
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
//		//audioDevice.playSound(Source, pos);
//	}
//}

void MainGame::setFogToonRimShader()
{
	glm::vec3 direction = myCamera.getPos() - myCamera.getForward();
	shaderFogToonRim.setVec3("viewDir", direction);
	shaderFogToonRim.setVec3("lightDir", glm::inverse(transform.GetModel()) * glm::vec4(-.9f, 0.0f, 0.0f, 0.0f));
	shaderFogToonRim.setFloat("maxDist", 45.0f);
	shaderFogToonRim.setFloat("minDist", 20.0f);
	shaderFogToonRim.setVec4("fogColor", glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void MainGame::setExplosionShader()
{
	static float timer = 0.0f;
	timer += 0.01f;
	if (timer > 2.0f)
		timer = 0.0f;

	shaderExplosion.setFloat("time", timer);
}

void MainGame::setReflectionShader()
{
	modelView = transform.GetModel() * myCamera.GetView();
	shaderReflection.setMat4("model", modelView);
	shaderReflection.setMat4("view", myCamera.GetView());
	shaderReflection.setMat4("projection", myCamera.GetProjection());
}


void MainGame::drawGame()
{
	_gameDisplay.clearDisplay(0.0f, 0.0f, 0.0f, 1.0f);


	Skybox();

	transform.SetPos(glm::vec3(sinf(counter), 0.5, 1.0));
	transform.SetRot(glm::vec3(0.0, sinf(counter) / 2, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));
	
	shaderReflection.Bind();
	setReflectionShader();
	shaderReflection.Update(transform, myCamera);
	mesh1.draw();
	
	
	transform.SetPos(glm::vec3(4.0, 0.0, 2.5));
	transform.SetRot(glm::vec3(0.0, 0.0, 0.0));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));
	
	shaderExplosion.Bind();
	setExplosionShader();
	shaderExplosion.Update(transform, myCamera);
	texture.Bind(0);
	mesh2.draw();


	transform.SetPos(glm::vec3(-sinf(counter), -sinf(counter) - 2.5, -sinf(counter) * 15 + 15));
	transform.SetRot(glm::vec3(0.0, sinf(counter * 3) / 2 + 0.4, 0.0));
	transform.SetScale(glm::vec3(0.4, 0.4, 0.4));

	shaderFogToonRim.Bind();
	setFogToonRimShader();
	shaderFogToonRim.Update(transform, myCamera);
	mesh3.draw();


	counter = counter + 0.01f;

	glEnableClientState(GL_COLOR_ARRAY);
	glEnd();

	_gameDisplay.swapBuffer();
} 