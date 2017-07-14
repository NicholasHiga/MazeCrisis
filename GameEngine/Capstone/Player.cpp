#include "Player.h"

#include <time.h> 
#include <stdio.h>
#include <stdlib.h> 
#include "GameObject.h"
#include "AudioManager.h"
#include "UserInterface.h"

namespace MazeCrisis
{
	using std::string;

	Player::Player(UserInterface *ui, WEAPON startingWeapon, int maxHealth,
		const std::string &gruntSoundPrefix, unsigned int numGruntSounds,
		const std::string &gruntSoundFiletype, std::string &deathSoundPath,
		const std::string &pistolSoundPath, const std::string &rifleSoundPath,
		const std::string &sniperSoundPath, const std::string &emptyClipSoundPath,
		const std::string &reloadSoundPath, const std::string &sayReloadSoundFilePath)
	{
		srand(time(NULL));
		setUserInterface(ui);
		setMaxHealth(maxHealth);
		setSayReloadSoundFilePath(sayReloadSoundFilePath);
		setDeathSoundFilePath(deathSoundPath);
		setGruntSoundParameters(gruntSoundPrefix, gruntSoundFiletype, numGruntSounds);
		setCurrentHealth(maxHealth);
		initWeapons(pistolSoundPath, rifleSoundPath,
			sniperSoundPath, emptyClipSoundPath, reloadSoundPath);
		setCurrentWeapon(startingWeapon);
		lastMouseState = GLFW_RELEASE;
		this->numGruntSounds = numGruntSounds;
	}

	int
	Player::getMaxHealth()
	{
		return maxHealth;
	}

	void
	Player::setMaxHealth(int maxHealth)
	{
		this->maxHealth = maxHealth;
	}

	int
	Player::getCurrentHealth()
	{
		return currentHealth;
	}

	void
	Player::setCurrentHealth(int currentHealth)
	{
		if (this->currentHealth > currentHealth)
		{
			if (currentHealth <= 0)
				AudioManager::getInstance()->playSound(deathSoundPath, ui->getGameVolumeFloat());
			else
				playRandomGruntSound();
		}
		else
		{
			// Gets extra health sound
		}

		if (currentHealth <= 0)
			this->currentHealth = 0;
		else if (currentHealth >= maxHealth)
			this->currentHealth = maxHealth;
		else
			this->currentHealth = currentHealth;

		ui->setHealth(currentHealth);
	}

	WEAPON
	Player::getCurrentWeapon()
	{
		return currentWeapon;
	}

	void
	Player::setCurrentWeapon(WEAPON newGun)
	{
		currentWeapon = newGun;
		if (guns[newGun].getCurrentClipAmmo() == 0)
			ui->pulseReloadMessage(1500); // TODO: Make it so this isn't hard coded.
		else
			ui->stopPulsingReloadMessage();
		ui->setWeapon(newGun);
		ui->setAmmoCount(guns[newGun].getCurrentClipAmmo(),
			guns[newGun].getTotalAmmo(), guns[newGun].getHasInfiniteAmmo());
	}

	UserInterface*
	Player::getUserInterface()
	{
		return ui;
	}

	void
	Player::setUserInterface(UserInterface *ui)
	{
		this->ui = ui;
	}

	string
	Player::getSayReloadSoundFilePath()
	{
		return sayReloadSoundPath;
	}

	void
	Player::setSayReloadSoundFilePath(const string &sayReloadSoundFilePath)
	{
		AudioManager::getInstance()->loadSound(sayReloadSoundFilePath);
		this->sayReloadSoundPath = sayReloadSoundFilePath;
	}

	std::string 
	Player::getDeathSoundFilePath()
	{
		return deathSoundPath;
	}

	void 
	Player::setDeathSoundFilePath(const std::string &deathSoundFilePath)
	{
		AudioManager::getInstance()->loadSound(deathSoundFilePath);
		this->deathSoundPath = deathSoundFilePath;
	}

	void
	Player::getGruntSoundParameters(std::string &gruntSoundPrefix, std::string &gruntSoundFiletype, unsigned int &numGruntSounds)
	{
		gruntSoundPrefix = this->gruntSoundPrefix;
		gruntSoundFiletype = this->gruntSoundFiletype;
		numGruntSounds = this->numGruntSounds;
	}

	void
	Player::setGruntSoundParameters(const std::string &gruntSoundPrefix, const std::string &gruntSoundFiletype, unsigned int numGruntSounds)
	{
		this->numGruntSounds = numGruntSounds;
		this->gruntSoundFiletype = gruntSoundFiletype;
		for (size_t i = 0; i < numGruntSounds; i++)
			AudioManager::getInstance()->loadSound(gruntSoundPrefix + std::to_string(i) + "." + gruntSoundFiletype);
		this->gruntSoundPrefix = gruntSoundPrefix;
	}

	void
	Player::playRandomGruntSound()
	{
		int index = rand() % 4;
		AudioManager::getInstance()->playSound(gruntSoundPrefix + std::to_string(index) + "." + gruntSoundFiletype, ui->getGameVolumeFloat());
	}

	Gun 
	Player::getCurrentGun()
	{
		return guns[currentWeapon];
	}

	bool
	Player::getIsShooting()
	{
		return shooting;
	}

	void
	Player::update(float deltaTime)
	{
		if (shooting && !reloading)
			reloading = guns[currentWeapon].shoot(&lastRay);
		if (reloading)
			reloading = guns[currentWeapon].reload();
	}

	void 
	Player::mouseHandler(GLFWwindow* window, int button, int action, int mods, Ray *ray)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			shooting = true;
			lastRay = *ray;
		}

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS
			&& lastMouseState == GLFW_RELEASE)
			lastMouseState = GLFW_PRESS;

		if (action == GLFW_RELEASE)
		{
			lastMouseState = GLFW_RELEASE;
			shooting = false;
		}
	}

	void
	Player::keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_R && (action == GLFW_REPEAT ||
			action == GLFW_PRESS))
			reloading = true;

		if (key == GLFW_KEY_1 && action == GLFW_PRESS)
			setCurrentWeapon(WEAPON::PISTOL);
		else if (key == GLFW_KEY_2 && action == GLFW_PRESS)
			setCurrentWeapon(WEAPON::RIFLE);
	}

	void 
	Player::cursorPosHandler(GLFWwindow* window, double x, double y, Ray *ray)
	{
		if (shooting)
			lastRay = *ray;
	}

	void
	Player::initWeapons(const std::string &pistolSound,
		const std::string &rifleSound, const std::string &sniperSound,
		const std::string &emptyClipSound, const std::string &reloadSoundPath)
	{
		// Total ammo, ammo per clip, damage per shot, time between shots, 
		// time to reload, hasInfiniteAmmo.
		guns.push_back(Gun(this, 6, 6, 30, 500, 500,
			pistolSound, emptyClipSound, reloadSoundPath, true));
		guns.push_back(Gun(this, 90, 30, 15, 60, 2000,
			rifleSound, emptyClipSound, reloadSoundPath, false));
		guns.push_back(Gun(this, 15, 5, 300, 2000, 3000, 
			sniperSound, emptyClipSound, reloadSoundPath, false));
	}
}