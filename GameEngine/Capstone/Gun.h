#pragma once
#include <iostream>
#include <string>
#include "RayQuery.h"

namespace MazeCrisis
{
	class Player;
	class UserInterface;

	class Gun
	{
	public:
		Gun(Player *player, unsigned int totalAmmo, 
			unsigned int ammoPerClip, unsigned int damagePerShot,
			float timeBetweenShots, float timeToReload,
			const std::string &shootingSoundFilePath, 
			const std::string &reloadSoundFilePath,
			const std::string &emptyClipSoundFilePath,
			bool hasInfiniteAmmo = false);
		bool shoot(Ray *lastRay);	// Returns true if reload initiated by shoot.
		bool reload();				// Returns true if reload still needs to continue.

		Player *getPlayer();
		void setPlayer(Player *p);

		unsigned int getTotalAmmo();
		void setTotalAmmo(unsigned int totalAmmo);

		unsigned int getAmmoPerClip();
		void setAmmoPerClip(unsigned int ammoPerClip);

		unsigned int getCurrentClipAmmo();
		void setCurrentClipAmmo(unsigned int currentClipAmmo);

		unsigned int getDamagePerShot();
		void setDamagePerShot(unsigned int damagePerShot);

		float getTimeBetweenShots();
		void setTimeBetweenShots(float timeBetweenShots);

		float getTimeToReload();
		void setTimeToReload(float timeToReload);

		bool getHasInfiniteAmmo();
		void setHasInfiniteAmmo(bool hasInfiniteAmmo);

		std::string getEmptySoundFilePath();
		void setEmptySoundFilePath(const std::string &emptyClipSoundFilePath);

		std::string getShootingSoundFilePath();
		void setShootingSoundFilePath(const std::string &shootingSoundFilePath);

		std::string getReloadSoundFilePath();
		void setReloadSoundFilePath(const std::string &reloadSoundFilePath);

	protected:
		Player *player;
		UserInterface *ui;
		unsigned int totalAmmo, ammoPerClip, currentClipAmmo, damagePerShot;
		float timeToReload, timeBetweenShots, shootingTimeStart, 
			reloadTimeStart, emptyClipTimeStart, emptyClipLength;
		bool reloadInitiated, hasInfiniteAmmo;
		std::string emptyClipSoundFilePath, shootingSoundFilePath, reloadSoundFilePath;
		bool emptySoundPlaying;
	};
}
