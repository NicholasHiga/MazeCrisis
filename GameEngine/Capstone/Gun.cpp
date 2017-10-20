#include "Gun.h"
#include "Clock.h"
#include "AudioManager.h"
#include "Enemy.h"
#include "SceneNode.h"
#include "Player.h"
#include "UserInterface.h"

namespace MazeCrisis
{
	using std::string;
	using std::vector;
	using std::shared_ptr;

	Gun::Gun(Player *player, unsigned int totalAmmo, unsigned int ammoPerClip,
		unsigned int damagePerShot, float timeBetweenShots, float timeToReload,
		const std::string &shootingSoundFilePath,
		const std::string &reloadSoundFilePath,
		const std::string &emptySoundFilePath, bool hasInfiniteAmmo)
	{
		setPlayer(player);
		setTotalAmmo(totalAmmo);
		setAmmoPerClip(ammoPerClip);
		setDamagePerShot(damagePerShot);
		setCurrentClipAmmo(ammoPerClip);
		setTimeBetweenShots(timeBetweenShots);
		setTimeToReload(timeToReload);
		setHasInfiniteAmmo(hasInfiniteAmmo);
		setEmptySoundFilePath(emptySoundFilePath);
		setReloadSoundFilePath(reloadSoundFilePath);
		setShootingSoundFilePath(shootingSoundFilePath);
		shootingTimeStart = Clock::getMilliseconds() - timeBetweenShots;
		reloadTimeStart = 0;
		reloadInitiated = false;
		emptySoundPlaying = false;
		emptyClipLength = AudioManager::getInstance()->getSoundLengthMS(
			emptySoundFilePath);
		ui = player->getUserInterface();
	}

	bool
	Gun::shoot(Ray *lastRay)
	{
		if (currentClipAmmo != 0 && Clock::getMilliseconds() - shootingTimeStart 
			> timeBetweenShots)
		{
			currentClipAmmo--;
			std::cout << currentClipAmmo << " / " << totalAmmo << std::endl;
			player->getUserInterface()->setAmmoCount(currentClipAmmo, totalAmmo,
				hasInfiniteAmmo);
			shootingTimeStart = Clock::getMilliseconds();
			AudioManager::getInstance()->playSound(shootingSoundFilePath, 
				ui->getGameVolumeFloat());

			vector<shared_ptr<SceneNode>> intersections =
				RayQuery::Raycast(*lastRay, true);

			if (!intersections.empty())
			{
				Enemy* enemy = dynamic_cast<Enemy*>((
					*intersections[0]->getRenderables())[0]);

				if (enemy)
				{
					std::cout << enemy->getGameObjectName() << " " <<
						(int)(enemy->getCurrentHealth() - damagePerShot)
						<< " hp" << std::endl;
					if (enemy->getGameObjectName() == "Boss"
						&& enemy->getCurrentHealth() - damagePerShot <= 0)
						Common::gameStates.push(GameState::VICTORY);
					enemy->setCurrentHealth(enemy->getCurrentHealth() 
						- damagePerShot);			
				}
			}

			if (currentClipAmmo == 0)
			{
				AudioManager::getInstance()->playSound(
					player->getSayReloadSoundFilePath(),
					ui->getGameVolumeFloat());
				player->getUserInterface()->pulseReloadMessage(1500);
			}
		}
		/*else if (totalAmmo != 0)
		{
			if (!reloadInitiated)
			{
				reloadTimeStart = Clock::getMilliseconds();
				reloadInitiated = true;
				// If we remove automatic reload, play empty sound here too.
			}
			return true;
		}*/
		else if (!emptySoundPlaying && Clock::getMilliseconds() -
			shootingTimeStart > timeBetweenShots)
		{
			// Empty clip
			AudioManager::getInstance()->playSound(emptyClipSoundFilePath,
				ui->getGameVolumeFloat());
			emptyClipTimeStart = Clock::getMilliseconds();
			emptySoundPlaying = true;
		}
		// Bit of a hacky way to prevent repeating empty clip sound.
		else if (Clock::getMilliseconds() - emptyClipTimeStart > 
			emptyClipLength)
		{
			emptySoundPlaying = false;
		}
		return false;
	}

	bool
	Gun::reload()
	{
		// Prevent reload on full clip.
		if (ammoPerClip != currentClipAmmo
			&& totalAmmo != 0)
		{
			if (!reloadInitiated)
			{
				AudioManager::getInstance()->playSound(reloadSoundFilePath, 
					ui->getGameVolumeFloat());
				reloadTimeStart = Clock::getMilliseconds();
				reloadInitiated = true;		
				player->getUserInterface()->stopPulsingReloadMessage();
			}

			if (Clock::getMilliseconds() - reloadTimeStart > timeToReload)
			{
				if (totalAmmo >= ammoPerClip)
				{
					if (!hasInfiniteAmmo)
						totalAmmo -= (ammoPerClip - currentClipAmmo);
					setCurrentClipAmmo(ammoPerClip);
				}
				else
				{
					setCurrentClipAmmo(totalAmmo);
					totalAmmo = 0;
				}

				reloadTimeStart = 0;
				reloadInitiated = false;
				std::cout << "Reload finished" << std::endl;
				std::cout << currentClipAmmo << " / " << totalAmmo << std::endl;
				player->getUserInterface()->setAmmoCount(currentClipAmmo,
					totalAmmo, hasInfiniteAmmo);
				return false; 
			}
			return true;
		}
		std::cout << "Ammo already full" << std::endl;
		return false;
	}

	Player*
	Gun::getPlayer() const
	{
		return player;
	}

	void 
	Gun::setPlayer(Player *p)
	{
		this->player = p;
	}

	unsigned int
	Gun::getTotalAmmo() const
	{
		return totalAmmo;
	}

	void
	Gun::setTotalAmmo(unsigned int totalAmmo)
	{
		this->totalAmmo = totalAmmo;
	}

	unsigned int
	Gun::getAmmoPerClip() const
	{
		return ammoPerClip;
	}

	void
	Gun::setAmmoPerClip(unsigned int ammoPerClip)
	{
		this->ammoPerClip = ammoPerClip;
	}

	unsigned int
	Gun::getCurrentClipAmmo() const
	{
		return currentClipAmmo;
	}

	void
	Gun::setCurrentClipAmmo(unsigned int currentClipAmmo)
	{
		this->currentClipAmmo = currentClipAmmo;
	}

	unsigned int
	Gun::getDamagePerShot() const
	{
		return damagePerShot;
	}

	void
	Gun::setDamagePerShot(unsigned int damagePerShot)
	{
		this->damagePerShot = damagePerShot;
	}

	float
	Gun::getTimeToReload() const
	{
		return timeToReload;
	}

	float 
	Gun::getTimeBetweenShots() const
	{
		return timeBetweenShots;
	}

	void
	Gun::setTimeBetweenShots(float timeBetweenShots)
	{
		this->timeBetweenShots = timeBetweenShots;
	}

	void
	Gun::setTimeToReload(float timeToReload)
	{
		this->timeToReload = timeToReload;
	}

	bool 
	Gun::getHasInfiniteAmmo() const
	{
		return hasInfiniteAmmo;
	}

	void 
	Gun::setHasInfiniteAmmo(bool hasInfiniteAmmo)
	{
		this->hasInfiniteAmmo = hasInfiniteAmmo;
	}

	string 
	Gun::getEmptySoundFilePath() const
	{
		return emptyClipSoundFilePath;
	}

	void 
	Gun::setEmptySoundFilePath(const string &emptyClipSoundFilePath)
	{
		AudioManager::getInstance()->loadSound(emptyClipSoundFilePath);
		this->emptyClipSoundFilePath = emptyClipSoundFilePath;
	}

	string 
	Gun::getShootingSoundFilePath() const
	{
		return shootingSoundFilePath;
	}

	void 
	Gun::setShootingSoundFilePath(const string &shootingSoundFilePath)
	{
		AudioManager::getInstance()->loadSound(shootingSoundFilePath);
		this->shootingSoundFilePath = shootingSoundFilePath;
	}

	string 
	Gun::getReloadSoundFilePath() const
	{
		return reloadSoundFilePath;
	}

	void 
	Gun::setReloadSoundFilePath(const string &reloadSoundFilePath)
	{
		AudioManager::getInstance()->loadSound(reloadSoundFilePath);
		this->reloadSoundFilePath = reloadSoundFilePath;
	}
}