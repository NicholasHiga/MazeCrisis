#pragma once
#include <vector>
#include <GL/glew.h>

#include "GLFW\glfw3.h"
#include "RayQuery.h"
#include "Gun.h"
#include "Common.h"

class AbstractRenderer;

namespace MazeCrisis
{
	class UserInterface;

	class Player
	{
	public:
		Player(UserInterface *ui, WEAPON startingWeapon, int maxHealth,
			const std::string &gruntSoundPrefix, unsigned int numGruntSounds, 
			const std::string &gruntSoundFiletype, std::string &deathSoundPath,
			const std::string &pistolSoundPath, const std::string &rifleSoundPath, 
			const std::string &sniperSoundPath,	const std::string &emptyClipSoundPath, 
			const std::string &reloadSoundPath, const std::string &sayReloadSoundFilePath);

		int getMaxHealth();
		void setMaxHealth(int maxHealth);

		int getCurrentHealth();
		void setCurrentHealth(int currentHealth);

		WEAPON getCurrentWeapon();
		void setCurrentWeapon(WEAPON newGun);

		UserInterface *getUserInterface();
		void setUserInterface(UserInterface *ui);

		std::string getSayReloadSoundFilePath();
		void setSayReloadSoundFilePath(const std::string &sayReloadSoundFilePath);
			
		std::string getDeathSoundFilePath();
		void setDeathSoundFilePath(const std::string &deathSoundFilePath);

		void getGruntSoundParameters(std::string &gruntSoundPrefix, std::string &gruntSoundFiletype, unsigned int &numGruntSounds);
		void setGruntSoundParameters(const std::string &gruntSoundPrefix, const std::string &gruntSoundFiletype, unsigned int numGruntSounds);

		void playRandomGruntSound();

		Gun getCurrentGun();
		bool getIsShooting();

		void update(float deltaTime);
		void mouseHandler(GLFWwindow* window, int button, int action, int mods, Ray *ray);
		void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
		void cursorPosHandler(GLFWwindow* window, double x, double y, Ray *ray);

	private:
		int currentHealth, maxHealth, lastMouseState;
		bool shooting, reloading;
		WEAPON currentWeapon;
		std::vector<Gun> guns;
		Ray lastRay;
		UserInterface *ui;

		// Sounds
		std::string sayReloadSoundPath, gruntSoundPrefix, gruntSoundFiletype, deathSoundPath;
		unsigned int numGruntSounds;

		void initWeapons(const std::string &pistolSound,
			const std::string &rifleSound, const std::string &sniperSound,
			const std::string &emptyClipSound, const std::string &reloadSoundPath);
	};
}