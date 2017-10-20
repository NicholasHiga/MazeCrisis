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
		bool shoot(Ray *lastRay);	// Returns true if reload initiated by 
									// shoot.
		bool reload();				// Returns true if reload still needs to
									// continue.

		Player *getPlayer() const;
		void setPlayer(Player *p);

		unsigned int getTotalAmmo() const;
		void setTotalAmmo(unsigned int totalAmmo);

		unsigned int getAmmoPerClip() const;
		void setAmmoPerClip(unsigned int ammoPerClip);

		unsigned int getCurrentClipAmmo() const;
		void setCurrentClipAmmo(unsigned int currentClipAmmo);

		unsigned int getDamagePerShot() const;
		void setDamagePerShot(unsigned int damagePerShot);

		float getTimeBetweenShots() const;
		void setTimeBetweenShots(float timeBetweenShots);

		float getTimeToReload() const;
		void setTimeToReload(float timeToReload);

		bool getHasInfiniteAmmo() const;
		void setHasInfiniteAmmo(bool hasInfiniteAmmo);

		std::string getEmptySoundFilePath() const;
		void setEmptySoundFilePath(const std::string &emptyClipSoundFilePath);

		std::string getShootingSoundFilePath() const;
		void setShootingSoundFilePath(const std::string &shootingSoundFilePath);

		std::string getReloadSoundFilePath() const;
		void setReloadSoundFilePath(const std::string &reloadSoundFilePath);

	protected:
		Player *player;
		UserInterface *ui;
		unsigned int totalAmmo, ammoPerClip, currentClipAmmo, damagePerShot;
		float timeToReload, timeBetweenShots, shootingTimeStart, 
			reloadTimeStart, emptyClipTimeStart, emptyClipLength;
		bool reloadInitiated, hasInfiniteAmmo;
		std::string emptyClipSoundFilePath, shootingSoundFilePath, 
			reloadSoundFilePath;
		bool emptySoundPlaying;
	};
}
