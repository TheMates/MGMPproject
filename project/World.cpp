#include "World.h"
#include "imageLoad.h"
using namespace std;

void World::drawTexture(int texture)
{
	glBindTexture(GL_TEXTURE_2D, textures[texture]);

	glBegin(GL_QUADS);	
	{
		glTexCoord2f(0, 0);
		glVertex3f(-textureWidth/2, 1, textureHeight/2);

		glTexCoord2f(1, 0);
		glVertex3f(textureWidth/2, 1, textureHeight/2);

		glTexCoord2f(1, 1);;
		glVertex3f(textureWidth/2, 1, -textureHeight/2);

		glTexCoord2f(0, 1);
		glVertex3f(-textureWidth/2, 1, -textureHeight/2);
	}
	glEnd();
}

void World::generateNewElapseTime(int index)
{
	//should generate faster for greater speeds
	timeElapsedFromLastCarGeneration[index] = minimumFramesElapsed + rand() % 60 + int((initialCarSpeed-currentCarSpeedMask[index])*135.0/0.24) ;	
}

std::vector<Car>::iterator World::findCarById(int id) 
{
	for(auto i = 0;i<cars.size();i++)
	{
		if (cars[i].Id() == id)
			return cars.begin() + i;
	}
	//can eventually throw (?)
}

void World::generateNewMasks()
{
	vector<int> newTexturesMask;
	vector<int> newTimeElapsed;
	vector<float> newCarSpeedMask;
	for (auto i = 0; i < nGeneratedTextures; i++)
	{
		auto textureId = (rand() % (nTextures + 2));		//this boosts probability for road
		textureId = textureId > nTextures - 1 ? 0 : textureId;
		if(i == 0)
			newTexturesMask.push_back(1);	//i always want the first to be grass
		else
			newTexturesMask.push_back(textureId);

		newCarSpeedMask.push_back(initialCarSpeed + float(rand() % 7)*0.04);
		newCarSpeedMask.push_back(initialCarSpeed + float(rand() % 7)*0.04);

		newTimeElapsed.push_back(0);
		newTimeElapsed.push_back(0);
	}
	currentTexturesMask = newTexturesMask;
	currentCarSpeedMask = newCarSpeedMask;
	timeElapsedFromLastCarGeneration = newTimeElapsed;
}

void World::DrawCrown()
{
	glPushMatrix();
	glTranslatef(crownPosition.x, crownPosition.y, crownPosition.z);
	glRotatef(crownAngle, 0, 1, 0);
	MGMPproject::drawBlenderObject(&crownLoader, 0);	
	glPopMatrix();
}

World::World(Player& player):player(&player)
{
}

GLuint* World::Textures(const int index)
{
	return  &textures[index];
}
GLuint* World::Textures()
{
	return textures;
}


void World::LoadTextures()
{
	setTexture("textures/road.bmp", &textures[0], true);
	setTexture("textures/grass.bmp", &textures[1], true);
	setTexture("textures/sand.bmp", &textures[2], true);
	setTexture("textures/dirt.bmp", &textures[3], true);

	generateNewMasks();
}

void World::LoadModels()
{
	trabantLoader.LoadFile("models/trabant.obj");
	crownLoader.LoadFile("models/crown.obj");
}

void World::DrawTextures()
{
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_BLEND);	//not sure
	
	glEnable(GL_TEXTURE_2D);

	glPushMatrix();
	
	glTranslatef(0, -2, 0);

	for(auto i = 0;i<nGeneratedTextures;i++)
	{
		drawTexture(currentTexturesMask[i]);	//draw 3x wide
		glPushMatrix();
		glTranslatef(-40, 0, 0);
		drawTexture(currentTexturesMask[i]);
		glTranslatef(80, 0, 0);
		drawTexture(currentTexturesMask[i]);
		glPopMatrix();
		glTranslatef(0, 0, -20);
	}
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

void World::ClearBuffers()
{
	glClearColor(0.7, 0.7, 0.9, 0.0);
	glClearDepth(1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void World::LoadNewLevel()
{
	cars.clear();
	generateNewMasks();
}

void World::Animate()
{
	if (!shouldAnimate)
		return;
	GenerateCars();
	for(auto i = 0;i<cars.size();i++)
	{
		cars[i].Animate();
	}
	for (auto i = 0; i < timeElapsedFromLastCarGeneration.size(); i++)
		--timeElapsedFromLastCarGeneration[i];	//odečítají se i nečinný
	crownAngle += 0.5;
}

void World::SetShouldAnimate(bool value)
{
	shouldAnimate = value;
}

void World::KeyboardInput(unsigned char key)
{
	switch (key)
	{
	case 'L':
	case 'l': 	LoadNewLevel();
		break;
	default: break;
	}
}

void World::DrawCars()
{
	for (auto i = 0; i < cars.size(); i++)
	{
		if (cars[i].IsOutOfLimit())
		{
			DestroyCar(cars[i].Id());
			if (i == 0 || i == cars.size())	//now one smaller

				continue;
		}
		cars[i].DrawCar();
		
	}
}

void World::GenerateCars()
{
	for(auto i = 0;i<nGeneratedTextures;i++)
	{
		if(currentTexturesMask[i]==0)	//road	//i want the cars to drive at 5 and 15 of the road texture
		{
			if(timeElapsedFromLastCarGeneration[2*i] < 0)
			{
				AddCar(-textureWidth, 0, -textureHeight/2 -20 * i + 5,currentCarSpeedMask[2*i] ,90);
				generateNewElapseTime(2 * i);
			}
			if (timeElapsedFromLastCarGeneration[2 * i + 1] < 0)
			{
				AddCar(textureWidth, 0, -textureHeight / 2 -20 * i + 15, currentCarSpeedMask[2 * i],-90);
				generateNewElapseTime(2 * i+1);
			}
		}

	}

}

void World::DestroyCar(int id)
{
	cars.erase(findCarById(id));
}

void World::CheckCollisions()
{
	for(auto i = 0;i<cars.size();i++)
	{
		if(cv::norm(cars[i].getFrontCollisionPoint() - player->GetPosition()) < carCollisionDistance)
		{
			if(--lives == 0)
			{
				levelsCompleted = 0;
				lives = 3;
			}
			
			player->ResetPosition();
		}
		else if(cv::norm(cars[i].getRearCollisionPoint() - player->GetPosition()) < carCollisionDistance)
		{
			if (--lives == 0)
			{
				levelsCompleted = 0;
				lives = 3;
			}
			player->ResetPosition();
		}
	}
	//X BORDER
	if(player->GetPosition().x >worldBorders.PositiveXBorder)
	{
		player->SetPosition(worldBorders.PositiveXBorder, player->GetPosition().y, player->GetPosition().z);
	}
	else if(player->GetPosition().x < worldBorders.NegativeXBorder)
	{
		player->SetPosition(worldBorders.NegativeXBorder, player->GetPosition().y, player->GetPosition().z);
	}
	//Y BORDER
	if (player->GetPosition().y > worldBorders.TopBorder)
	{
		player->SetPosition(player->GetPosition().x, worldBorders.TopBorder, player->GetPosition().z);
	}
	else if (player->GetPosition().y < worldBorders.BottomBorder)
	{
		player->SetPosition(player->GetPosition().x, worldBorders.BottomBorder, player->GetPosition().z);
	}
	//Z BORDER
	if (player->GetPosition().z > worldBorders.PositiveZBorder)
	{
		player->SetPosition(player->GetPosition().x, player->GetPosition().y, worldBorders.PositiveZBorder);
	}
	else if (player->GetPosition().z < worldBorders.NegativeZBorder)
	{
		player->SetPosition(player->GetPosition().x, player->GetPosition().y, worldBorders.NegativeZBorder);
	}

	//CROWN - GET TO THE NEXT LEVEL
	if(cv::norm(cv::Mat( player->GetPosition()),cv::Mat(crownPosition))<crownActivationDistance)	//simple check the distance from the crown
	{
		++levelsCompleted;
		if(levelsCompleted %4 == 0 && levelsCompleted !=0)
		{
			++lives;
		}
		player->ResetPosition();
		LoadNewLevel();
	}
}

//LOGIC
int World::GetLives() const
{
	return lives;
}
int World::GetLevelsCompleted() const
{
	return levelsCompleted;
}
int World::GetNumCars() const
{
	return cars.size();
}


void World::AddCar(float xPosition, float yPosition, float zPosition, float speed, float deltaAzimuth)
{
	cars.emplace_back(carsIds++, xPosition, yPosition,zPosition,speed, trabantLoader);
	cars[cars.size() - 1].RotateAngle(deltaAzimuth, 0, 0);
}
