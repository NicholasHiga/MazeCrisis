#pragma once
#include "GameObject.h"

namespace MazeCrisis
{
	class Enemy : public GameObject
	{
	public:
		Enemy(int maxHealth);
		Enemy(const std::string &gameObjectName,
			const std::string &modelName, int maxHealth);

		int getMaxHealth();
		void setMaxHealth(int maxHealth);

		int getCurrentHealth();
		void setCurrentHealth(int currentHealth);

	private:
		int currentHealth;
		int maxHealth;
	};
}
