#include "Enemy.h"

#include "BoundingBox.h"
#include "SceneNode.h"

namespace MazeCrisis
{
	Enemy::Enemy(int maxHealth) : GameObject() 
	{
		setMaxHealth(maxHealth);
		setCurrentHealth(maxHealth);
	}

	Enemy::Enemy(const std::string &gameObjectName,
		const std::string &modelName, int maxHealth) 
		: GameObject(gameObjectName, modelName)
	{
		setMaxHealth(maxHealth);
		setCurrentHealth(maxHealth);
	}

	int
	Enemy::getMaxHealth()
	{
		return maxHealth;
	}

	void
	Enemy::setMaxHealth(int maxHealth)
	{
		this->maxHealth = maxHealth;
	}

	int
	Enemy::getCurrentHealth()
	{
		return currentHealth;
	}

	void
	Enemy::setCurrentHealth(int currentHealth)
	{
		if (currentHealth <= 0)
		{
			this->currentHealth = 0;
			setIsEnabled(false);
			getParentNode()->getParentNode()->removeChildNode(getParentNode());
		}
		else
			this->currentHealth = currentHealth;
	}
}
