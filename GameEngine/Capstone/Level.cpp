#include "Level.h"

#include "Maze.h"
#include "Clock.h"
#include "Enemy.h"
#include "Player.h"

#include "ShaderVariable.h"
#include "AutomaticCamera.h"

#include "Game.h"

namespace MazeCrisis
{
	using glm::vec3;
	using std::string;
	using std::vector;

	Level::Level(Game *game, string skyboxTextureLocation)
	{
		vec3 startingBlock(1, 1, 1);
		vec3 cameraStart(startingBlock.x * sizePerCube,
			startingBlock.y * sizePerCube,
			startingBlock.z * (GLint)sizePerCube * -1);
		autocam = static_cast<AutomaticCamera*>(game->getCamera());
		autocam->setCameraPosition(cameraStart, false);
		autocam->setSceneCenter(cameraStart - vec3(0, 0, 3), false);
		autocam->setCameraUp(vec3(0, 1, 0), true);
		autocam->clearPathing();
		this->game = game;
		this->skyboxTextureLocation = skyboxTextureLocation;
		initLevel();
		numEnemiesTotal = game->getSceneGraph()->getRootSceneNode()->
			getChildren()->size();
		lastEnemyKilled = 0;
	}

	Level::~Level()
	{
		game->getSceneGraph()->removeAllNodes();
		delete player;
		delete maze;
	}

	void
	Level::initLevel()
	{
		vector<ShaderVariable> mainShaderVars;
		mainShaderVars.push_back(ShaderVariable(ENGINE_VAR::VERTEX_POSITION,
			"position"));
		mainShaderVars.push_back(ShaderVariable(ENGINE_VAR::VERTEX_UV,
			"uv"));
		mainShaderVars.push_back(ShaderVariable(ENGINE_VAR::SAMPLER_2D,
			"textureIn"));
		mainShaderVars.push_back(ShaderVariable(ENGINE_VAR::MODEL_VIEW_MATRIX, 
			"modelViewMatrix"));
		mainShaderVars.push_back(ShaderVariable(ENGINE_VAR::PROJECTION_MATRIX, 
			"projectionMatrix"));

		vector<ShaderVariable> skyboxShaderVars;
		skyboxShaderVars.push_back(ShaderVariable(ENGINE_VAR::VERTEX_POSITION,
			"position"));
		skyboxShaderVars.push_back(ShaderVariable(ENGINE_VAR::SAMPLER_CUBE, 
			"skyboxTexture"));
		skyboxShaderVars.push_back(ShaderVariable(ENGINE_VAR::MODEL_VIEW_MATRIX,
			"viewMatrix"));
		skyboxShaderVars.push_back(ShaderVariable(ENGINE_VAR::PROJECTION_MATRIX, 
			"projectionMatrix"));

		vector<string> textures =
		{
			TEXTURES_PATH + "floor.png",
			TEXTURES_PATH + "Bricks.bmp"
		};

		vector<vector<vector<GLint>>> mazeTextures =
		{
			{
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },

				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
				{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			},
			{
			  // Comments wise, leaving this as is, because formatting is 
			  // really important here
			  // -15 | 15
			  //    x -> 
	          //    0,  30,  60,  90, 120, 150, 180, 210, 240, 270, 300, 330, 360, 390, 420, 450, 480
				{   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   0,   0 }, //    0
/*Starting pos*/{   2,   0,   2,   2,   0,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0 }, //  -30 
				{   2,   0,   2,   2,   0,   2,   2,   2,   0,   0,   2,   0,   0,   0,   0,   0,   0 }, //  -60
				{   2,   0,   2,   2,   2,   2,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0 }, //  -90
				{   2,   0,   0,   0,   0,   0,   2,   2,   0,   0,   2,   0,   0,   0,   0,   0,   0 }, // -120
				{   2,   0,   2,   0,   2,   2,   2,   2,   0,   0,   2,   0,   0,   0,   0,   0,   0 }, // -150
				{   2,   0,   2,   0,   2,   0,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0 }, // -180
				{   2,   0,   2,   0,   2,   0,   2,   2,   0,   0,   2,   0,   0,   0,   0,   0,   0 }, // -210
				{   2,   0,   2,   0,   0,   0,   0,   2,   0,   0,   2,   0,   0,   0,   0,   0,   0 }, // -240  ^
				{   2,   2,   2,   2,   2,   2,   2,   0,   0,   0,   2,   0,   0,   0,   0,   0,   0 }, // -270  z 10

				{   2,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   2,   0,   0,   0,   0,   0 }, // -300  z
				{   2,   2,   2,   2,   2,   2,   2,   0,   0,   0,   2,   2,   2,   2,   2,   2,   2 }, // -330  z
				{   2,   2,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   0,   0,   0,   0,   2 }, // -360  z
				{   2,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2 }, // -390  z
				{   2,   2,   2,   2,   2,   2,   2,   0,   0,   0,   0,   0,   0,   0,   0,   0,   2 }, // -420  z
				{   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2 }, // -450  z
			},
		};

		try
		{
			maze = new Maze(sizePerCube, vec3(0, 0, 0),
				"Cube", "TexturedCube",
				SHADERS_PATH + "TexturedVertex.vert",
				SHADERS_PATH + "TexturedFragment.frag",
				mainShaderVars, textures, mazeTextures);
			skybox = std::make_shared<Skybox>("SkyboxMesh", "SkyboxTexture",
				"SkyboxShader",
				SHADERS_PATH + "SkyboxVertex.vert", 
				SHADERS_PATH + "SkyboxFragment.frag",
				skyboxShaderVars, 
				TEXTURES_PATH + skyboxTextureLocation + "_lf.tga",
				TEXTURES_PATH + skyboxTextureLocation + "_rt.tga",
				TEXTURES_PATH + skyboxTextureLocation + "_dn.tga",
				TEXTURES_PATH + skyboxTextureLocation + "_up.tga",
				TEXTURES_PATH + skyboxTextureLocation + "_ft.tga",
				TEXTURES_PATH + skyboxTextureLocation + "_bk.tga", true);
		}
		catch (std::exception &e)
		{
			throw e;
		}

		std::shared_ptr<SceneNode> skyNode = std::make_shared<SceneNode>();
		skyNode->addRenderable(*skybox);
		game->getSceneGraph()->appendChildNode(skyNode);
		game->getSceneGraph()->appendChildNode(maze->getMazeRoot());
		initGameObjects();
		player = new Player(game->getUserInterface(), WEAPON::PISTOL, 3,
			SOUNDS_PATH + "grunt", 5, "wav", SOUNDS_PATH + "death.wav",
			SOUNDS_PATH + "shot1.wav", SOUNDS_PATH + "shot1.wav",
			SOUNDS_PATH + "shot1.wav", SOUNDS_PATH + "guncock.wav",
			SOUNDS_PATH + "empty.wav", SOUNDS_PATH + "say_reload.wav");
	}

	void
	Level::initGameObjects()
	{
		game->getMeshManager()->loadCubeModel("Enemy1", "Cube",
			"CrateEnemy", "TexturedCube", TEXTURES_PATH + "aCrate.bmp");
		game->getMeshManager()->loadCubeModel("Enemy2", "Cube",
			"CrateEnemy2", "TexturedCube", TEXTURES_PATH + "bCrate.jpg");
		game->getMeshManager()->loadCubeModel("Enemy3", "Cube", "ZombieEnemy",
			"TexturedCube", TEXTURES_PATH + "zombie.png");
		game->getMeshManager()->loadCubeModel("Blacksmith", "Cube", 
			"Blacksmith", "TexturedCube", TEXTURES_PATH + "blacksmith.png");
		game->getMeshManager()->loadCubeModel("Footballer", "Cube",
			"Footballer", "TexturedCube", TEXTURES_PATH + "football.png");
		game->getMeshManager()->loadCubeModel("Boss", "Cube", "Boss",
			"TexturedCube", TEXTURES_PATH + "boss.png");
		game->getMeshManager()->loadCubeModel("Buckets", "Cube", "Buckets",
			"TexturedCube", TEXTURES_PATH + "buckets.png");

		enemies.push_back(std::make_shared<Enemy>("E1", "Enemy1", 30));
		enemies.push_back(std::make_shared<Enemy>("E2", "Enemy2", 40));
		enemies.push_back(std::make_shared<Enemy>("E3", "Enemy3", 70));
		enemies.push_back(std::make_shared<Enemy>("E4", "Footballer", 90));
		enemies.push_back(std::make_shared<Enemy>("E5", "Blacksmith", 150));
		enemies.push_back(std::make_shared<Enemy>("E6", "Buckets", 70));
		enemies.push_back(std::make_shared<Enemy>("E7", "Enemy2", 30));
		enemies.push_back(std::make_shared<Enemy>("E8", "Footballer", 90));
		enemies.push_back(std::make_shared<Enemy>("E9", "Enemy3", 70));
		enemies.push_back(std::make_shared<Enemy>("E10", "Enemy3", 70));
		enemies.push_back(std::make_shared<Enemy>("E11", "Footballer", 100));
			// 11

		enemies.push_back(std::make_shared<Enemy>("E12", "Enemy3", 70));
		enemies.push_back(std::make_shared<Enemy>("E13", "Footballer", 90));
		enemies.push_back(std::make_shared<Enemy>("E14", "Buckets", 70));
		enemies.push_back(std::make_shared<Enemy>("E15", "Buckets", 70));
		enemies.push_back(std::make_shared<Enemy>("E16", "Blacksmith", 150));
		enemies.push_back(std::make_shared<Enemy>("E17", "Enemy3", 70));
		enemies.push_back(std::make_shared<Enemy>("Boss", "Boss", 300));

		/*enemies.push_back(std::make_shared<Enemy>("Enemy1", 30));
		enemies.push_back(std::make_shared<Enemy>("Enemy2", 30));
		enemies.push_back(std::make_shared<Enemy>("Enemy3", 3));
		enemies.push_back(std::make_shared<Enemy>("Footballer", 3));
		enemies.push_back(std::make_shared<Enemy>("Blacksmith", 3));
		enemies.push_back(std::make_shared<Enemy>("Buckets", 3));
		enemies.push_back(std::make_shared<Enemy>("Enemy2", 3));
		enemies.push_back(std::make_shared<Enemy>("Footballer", 3));
		enemies.push_back(std::make_shared<Enemy>("Enemy3", 3));
		enemies.push_back(std::make_shared<Enemy>("Enemy3", 3));
		enemies.push_back(std::make_shared<Enemy>("Footballer", 3)); // 11

		enemies.push_back(std::make_shared<Enemy>("Enemy3", 3));
		enemies.push_back(std::make_shared<Enemy>("Footballer", 3));
		enemies.push_back(std::make_shared<Enemy>("Buckets", 3));
		enemies.push_back(std::make_shared<Enemy>("Buckets", 3));
		enemies.push_back(std::make_shared<Enemy>("Blacksmith", 3));
		enemies.push_back(std::make_shared<Enemy>("Enemy3", 3));   // 17

		enemies.push_back(std::make_shared<Enemy>("Boss", 3));*/

		moveSpeed = { 0.008f, 0.008f, 0.003f, 0.030f };

		for (int i = 0; i < 18; ++i)
		{
			nodes.push_back(std::make_shared<SceneNode>());
			nodes[i]->addRenderable(*enemies[i]);

			// NOT BOSS
			//if (i != 10)
				game->getSceneGraph()->appendChildNode(nodes[i]);
		}

		// Z is coming toward the player, x is on the horizontal, y vertical

		nodes[0]->setPosition(vec3(30, 18, -120));
		nodes[0]->setScale(vec3(10, 10, 10));
		/*scripts.push_back(
			[=](SceneNode* n, double deltaTime)
		{
			vec3 tmp = n->getPosition();
			if (tmp.x < 20 || tmp.x > 40)
			{
				moveSpeed[0] *= -1;
				if (tmp.x < 20)
					tmp.x = 20;
				else
					tmp.x = 40;
			}
			n->setPosition(tmp + vec3(moveSpeed[0] * deltaTime, 0, 0));
		});
		enemies[0].get()->setScript(scripts[0]);*/

		nodes[1]->setPosition(vec3(30, 23, -140));
		nodes[1]->setScale(vec3(20, 5, 10));

		// Slow zombie
		nodes[2]->setPosition(vec3(20, 23, -120));
		nodes[2]->setScale(vec3(10, 20, 5));

		// Footballer
		nodes[3]->setPosition(vec3(210, 23, -120));
		nodes[3]->setScale(vec3(15, 20, 15));

		// Blacksmith
		nodes[4]->setPosition(vec3(160, 23, -160));
		nodes[4]->setScale(vec3(13, 20, 13));

		// Buckets
		nodes[5]->setPosition(vec3(180, 23, -180));
		nodes[5]->setScale(vec3(10, 20, 10));

		// Crate
		nodes[6]->setPosition(vec3(240, 30, -170));
		nodes[6]->setScale(vec3(13, 13, 13));

		// Footballer
		nodes[7]->setPosition(vec3(240, 23, -280));
		nodes[7]->setScale(vec3(15, 20, 15));

		// Slow
		nodes[8]->setPosition(vec3(261, 23, -60));
		nodes[8]->setScale(vec3(13, 20, 13));

		// Slow
		nodes[9]->setPosition(vec3(240, 23, -90));
		nodes[9]->setScale(vec3(13, 20, 13));

		// Footballer
		nodes[10]->setPosition(vec3(255, 23, -30));
		nodes[10]->setScale(vec3(15, 20, 15));

		///

		// Enemy3
		nodes[11]->setPosition(vec3(210, 23, -300));
		nodes[11]->setScale(vec3(13, 20, 13));

		// Footballer update
		nodes[12]->setPosition(vec3(270, 23, -420));
		nodes[12]->setScale(vec3(15, 20, 15));

		// Buckets 
		nodes[13]->setPosition(vec3(300, 23, -300));
		nodes[13]->setScale(vec3(10, 20, 10));

		// Buckets update
		nodes[14]->setPosition(vec3(240, 23, -270));
		nodes[14]->setScale(vec3(10, 20, 10));

		// Blacksmith update 
		nodes[15]->setPosition(vec3(315, 23, -360));
		nodes[15]->setScale(vec3(13, 20, 13));

		// Enemy3 update
		nodes[16]->setPosition(vec3(245, 23, -330));
		nodes[16]->setScale(vec3(10, 20, 10));

		// Boss
		nodes[17]->setPosition(vec3(450, 50, -390));
		nodes[17]->setScale(vec3(60, 90, 60));
	}

	void
	Level::update(float deltaTime)
	{
		player->update(deltaTime);
		autocam->update(deltaTime);

		// When an enemy is shot, this occurs as an effect of 
		// setCurrentHealth() in Enemy.
		if (numEnemiesTotal != game->getSceneGraph()->getRootSceneNode()
			->getChildren()->size())
		{
			numEnemiesTotal = game->getSceneGraph()->getRootSceneNode()
				->getChildren()->size();
			lastEnemyKilled++;

			// Section 1.
			if (lastEnemyKilled == 3)
			{
				autocam->autoMoveStraight(30 * 3, 2100);
				autocam->autoRotate(-90, 1000);
				currentSection++;
			}

			// Section 2. Footballer 1
			if (lastEnemyKilled == 4)
			{
				autocam->autoMoveStraight(30 * 2, 1400);
				autocam->autoRotate(90, 1000);
				autocam->autoMoveStraight(30 * 4, 2800);
				autocam->autoRotate(-90, 1000);
				currentSection++;
			}

			// Section 3. Blacksmith
			if (lastEnemyKilled == 5)
			{
				autocam->autoMoveStraight(30 * 2, 1400);
				autocam->autoRotate(-90, 1000);
				currentSection++;
			}

			// Section 4. Buckethead
			if (lastEnemyKilled == 6)
			{
				autocam->autoMoveStraight(30 * 2, 1400);
				autocam->autoRotate(90, 1000);
				currentSection++;
			}

			// Section 5. Footballer 2 / crate
			if (lastEnemyKilled == 8)
			{
				autocam->autoMoveStraight(30 * 3 + 15, 2100);
				autocam->autoRotate(-90, 1000);
				currentSection++;
				//game->getSceneGraph()->appendChildNode(nodes[8]);
			}

			// Section 6.
			if (lastEnemyKilled == 11)
			{
				autocam->autoRotate(180, 2000);
				autocam->autoMoveStraight(30 * 7 + 5, 8000); // 5 is a bit
															 // of correction
				autocam->autoRotate(-90, 2000);
				currentSection++;
				//Common::gameStates.push(GameState::VICTORY);
			}

			// Section 7.
			/*if (lastEnemyKilled == 12)
			{
				Common::gameStates.push(GameState::VICTORY);
			}*/
		}

		// First section
		if (currentSection == 1)
		{
			vec3 tmp = nodes[0]->getPosition();

			if (tmp.x < 20)
			{
				tmp.x = 20 - moveSpeed[0] * deltaTime;
				moveSpeed[0] *= -1;
			}
			else if (tmp.x > 40)
			{
				tmp.x = 40 - moveSpeed[0] * deltaTime;
				moveSpeed[0] *= -1;
			}
			else
				tmp.x += moveSpeed[0] * deltaTime;

			nodes[0]->setPosition(tmp);

			tmp = nodes[1]->getPosition();
			if (tmp.y < 20)
			{
				tmp.y = 20 - moveSpeed[0] * deltaTime;
				moveSpeed[0] *= -1;
			}
			else if (tmp.x > 40)
			{
				tmp.y = 40 - moveSpeed[0] * deltaTime;
				moveSpeed[0] *= -1;
			}
			else
				tmp.y += moveSpeed[0] * deltaTime;
			nodes[1]->setPosition(tmp);

			nodes[2]->setPosition(nodes[2]->getPosition()
				+ vec3(0, 0, moveSpeed[2] * deltaTime)); // moveSpeed[2]
		}

		// Second section
		if (currentSection == 2)
			nodes[3]->setPosition(nodes[3]->getPosition()
				+ vec3(-moveSpeed[3] * deltaTime, 0, 0));

		if (currentSection == 3)
		{
			if (moveStateBlackSmith == 0)
			{
				nodes[4]->setPosition(nodes[4]->getPosition() 
					+ vec3(0, 0, -0.015f * deltaTime));
				if (nodes[4]->getPosition().z < -240)
					moveStateBlackSmith++;
			}
			else if (moveStateBlackSmith == 1)
			{
				nodes[4]->setPosition(nodes[4]->getPosition()
					+ vec3(-0.010f * deltaTime, 0, 0));
			}
		}

		if (currentSection == 4)
		{
			nodes[5]->setPosition(nodes[5]->getPosition()
				+ vec3(-0.003f * deltaTime, 0, 0));
		}

		if (currentSection == 5)
		{
			if (moveStateFooterballer == 0)
			{
				nodes[7]->setPosition(nodes[7]->getPosition() 
					+ vec3(0, 0, 0.04f * deltaTime));
				if (nodes[7]->getPosition().z > -180)
					moveStateFooterballer++;
			}
			else if (moveStateFooterballer == 1)
			{
				nodes[7]->setPosition(nodes[7]->getPosition()
					+ vec3(-0.04f * deltaTime, 0, 0));
			}
		}

		if (currentSection == 6)
		{
			nodes[8]->setPosition(nodes[8]->getPosition()
				+ vec3(0, 0, -0.005f * deltaTime));
			nodes[9]->setPosition(nodes[9]->getPosition()
				+ vec3(0, 0, -0.003f * deltaTime));
			nodes[10]->setPosition(nodes[10]->getPosition()
				+ vec3(0, 0, -0.027f * deltaTime));
		}

		if (currentSection == 7)
		{
			// nodes[11]->setPosition(nodes[11]->getPosition() 
			//	+ vec3(0, 0, 0.005f * deltaTime));
			nodes[12]->setPosition(nodes[12]->getPosition()
				+ vec3(0, 0, 0.027f * deltaTime));
			// nodes[13]->setPosition(nodes[13]->getPosition() 
			//  + vec3(0, 0, 0.027f * deltaTime));
			nodes[14]->setPosition(nodes[14]->getPosition()
				+ vec3(0, 0, 0.005f * deltaTime));
			nodes[15]->setPosition(nodes[15]->getPosition()
				+ vec3(0, 0, 0.003f * deltaTime));
			nodes[16]->setPosition(nodes[16]->getPosition()
				+ vec3(0, 0, 0.005f * deltaTime));
			nodes[17]->setPosition(nodes[17]->getPosition()
				+ vec3(-0.006f * deltaTime, 0, 0)); // Boss
		}

		std::vector<unsigned int> collisionIndices;
		std::vector<std::shared_ptr<SceneNode>> cameraCollisions =
			BoundingBox::doesCollide(nodes, *autocam->getBoundingBox(), 
				&collisionIndices);

		// When enemy collides with player.
		if (!cameraCollisions.empty())
		{
			vector<Renderable*> *renderables = cameraCollisions[0]
				->getRenderables();
			if (GameObject *go = dynamic_cast<GameObject*>((*renderables)[0]))
			{
#ifdef _DEBUG
				std::cout << "Player hit by " <<
					go->getGameObjectName() << std::endl;
#endif
				game->getSceneGraph()->removeNodeWithRenderable(*go);
				if (go->getGameObjectName() == "Boss")
					player->setCurrentHealth(0);
				else 
					player->setCurrentHealth(player->getCurrentHealth() - 1);
				go->setIsEnabled(false);

				if (player->getCurrentHealth() == 0)
					Common::gameStates.push(GameState::GAME_OVER);
			}
		}
	}

	void
	Level::mouseHandler(GLFWwindow* window, int button, int action, int mods)
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		Ray r = RayQuery::getRayFromMouseClick((int)xpos, (int)ypos);
		player->mouseHandler(window, button, action, mods, &r);

		if (action == GLFW_RELEASE)
			lastMouseState = GLFW_RELEASE;
	}

	void
	Level::keyHandler(GLFWwindow* window, int key, int scancode, int action,
		int mods)
	{
#ifdef _DEBUG
		// Move forward
		if (key == GLFW_KEY_W &&
			(action == GLFW_PRESS || action == GLFW_REPEAT))
			autocam->moveForward(1);

		// Move backward
		if (key == GLFW_KEY_S &&
			(action == GLFW_PRESS || action == GLFW_REPEAT))
			autocam->moveBackward(1);

		// Strafe right
		if (key == GLFW_KEY_D &&
			(action == GLFW_PRESS || action == GLFW_REPEAT))
			autocam->moveRight(1);

		// Strafe left
		if (key == GLFW_KEY_A &&
			(action == GLFW_PRESS || action == GLFW_REPEAT))
			autocam->moveLeft(1);

		if (key == GLFW_KEY_Q &&
			(action == GLFW_PRESS || action == GLFW_REPEAT))
			autocam->moveUp(1);

		if (key == GLFW_KEY_E &&
			(action == GLFW_PRESS || action == GLFW_REPEAT))
			game->getCamera()->moveDown(1);

		if (key == GLFW_KEY_LEFT && action == GLFW_PRESS
			&& lastStateLeft == GLFW_RELEASE) {
			autocam->autoRotate(90, 1000);
			lastStateLeft = GLFW_PRESS;
		}

		if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS
			&& lastStateRight == GLFW_RELEASE) {
			autocam->autoRotate(-90, 1000);
			lastStateRight = GLFW_PRESS;
		}

		if (key == GLFW_KEY_UP && action == GLFW_PRESS
			&& lastStateUp == GLFW_RELEASE) {
			autocam->autoMoveStraight(30, 700);
			lastStateUp = GLFW_PRESS;
		}

		if (key == GLFW_KEY_DOWN && action == GLFW_PRESS
			&& lastStateDown == GLFW_RELEASE) {
			autocam->autoRotate(180, 2000);
			autocam->autoMoveStraight(30, 700);
			lastStateDown = GLFW_PRESS;
		}

		if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
			lastStateLeft = GLFW_RELEASE;

		if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
			lastStateRight = GLFW_RELEASE;

		if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
			lastStateUp = GLFW_RELEASE;

		if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
			lastStateDown = GLFW_RELEASE;
#endif
		player->keyHandler(window, key, scancode, action, mods);
	}

	void 
	Level::cursorPosHandler(GLFWwindow* window, double x, double y)
	{
		Ray r = RayQuery::getRayFromMouseClick((int)x, (int)y);
		player->cursorPosHandler(window, x, y, &r);
	}
}
