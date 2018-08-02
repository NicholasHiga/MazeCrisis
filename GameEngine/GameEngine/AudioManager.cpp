#include "AudioManager.h"
#include "Clock.h"

AudioManager* AudioManager::myInstance = nullptr;
FMOD::System* AudioManager::system = nullptr;
FMOD_RESULT AudioManager::result;

using std::shared_ptr;

AudioManager*
AudioManager::getInstance()
{
    if (myInstance == nullptr)
        myInstance = new AudioManager();

    return myInstance;
}

AudioManager::~AudioManager()
{
    ResourceIterator iterator;
    for (ResourceIterator iterator = resources.begin();
        iterator != resources.end(); ++iterator)
    {
        result = iterator->second.get()->sound->release();
        errorCheck();
    }
    result = system->close();
    errorCheck();
    result = system->release();
    errorCheck();
}

AudioManager::AudioManager()
{
    result = FMOD::System_Create(&system);  // Create the main system object.
    errorCheck();
    result = system->init(64, FMOD_INIT_NORMAL, 0);    // Initialize FMOD.
    errorCheck();
    fadingOut = false;
    isStreamPlaying = false;
}

void 
AudioManager::errorCheck()
{
    if (result != FMOD_OK)
        throw std::runtime_error(FMOD_ErrorString(result));
}

bool 
AudioManager::loadSound(const std::string &soundPath)
{
    ResourceIterator it = resources.find(soundPath);
    if (it == resources.end())
    {
        FMOD::Sound *tmp;
        result = system->createSound(soundPath.c_str(), FMOD_DEFAULT, 0, &tmp);
        errorCheck();
        store(soundPath, std::make_shared<SoundWrapper>(tmp));
        return true;
    }
    return false;
}

bool
AudioManager::loadStream(const std::string &streamPath)
{
    ResourceIterator it = resources.find(streamPath);
    if (it == resources.end())
    {
        FMOD::Sound *tmp;
        result = system->createStream(streamPath.c_str(), FMOD_LOOP_NORMAL 
            | FMOD_2D, 0, &tmp);
        errorCheck();
        store(streamPath, std::make_shared<SoundWrapper>(tmp));
        return true;
    }
    return false;
}

void
AudioManager::playSound(const std::string &soundPath, float volume)
{
    SoundWrapper *sw = get(soundPath);
    result = system->playSound(sw->sound, 0, false, &sw->channel);
    errorCheck();
    result = sw->channel->setVolume(volume);
    errorCheck();
}

void
AudioManager::playStream(const std::string &streamPath, float volume)
{
    playSound(streamPath, volume);
    currentlyPlayingStream = streamPath;
    isStreamPlaying = true;
}

void 
AudioManager::playStreamFadeIn(const std::string &streamPath,
    unsigned int fadeDurationMS, float volume)
{
    SoundWrapper *sw = get(streamPath);
    playStream(streamPath, volume);
    result = sw->channel->setPaused(true);
    errorCheck();
    fadeStreamIn(fadeDurationMS, volume);
    result = sw->channel->setPaused(false);
    errorCheck();
}

void 
AudioManager::playNextStream(const std::string &streamPath, 
    unsigned int fadeDurationMS, float volume)
{
    nextStream = streamPath;
    fadeStreamOut(fadeDurationMS / 2);
    fadeOutStartTime = Clock::getMilliseconds();
    fadeOutDuration = fadeDurationMS / 2;
    fadingOut = true;
    fadeInVolume = volume;
    errorCheck();
}

void 
AudioManager::fadeStreamIn(unsigned int fadeDurationMS, float endVolume)
{
    SoundWrapper *sw = get(currentlyPlayingStream);
    unsigned long long dspclock;
    unsigned int fadeDurationSecs = fadeDurationMS / 1000.0f;
    int rate;

    result = system->getSoftwareFormat(&rate, 0, 0); // Get mixer rate
    errorCheck();

    result = sw->channel->getDSPClock(0, &dspclock); // Get the reference clock,
        // which is the parent channel group
    errorCheck();
    result = sw->channel->addFadePoint(dspclock, 0.0f); // Add a fade point at
        // 'now' with full volume.
    errorCheck();
    result = sw->channel->addFadePoint(dspclock + (rate * fadeDurationSecs),
        endVolume); // Add a fade point 5 seconds later at 0 volume.
    errorCheck();
}

void 
AudioManager::fadeStreamOut(unsigned int fadeDurationMS)
{
    SoundWrapper *sw = get(currentlyPlayingStream);
    unsigned long long dspclock;
    unsigned int fadeDurationSecs = fadeDurationMS / 1000.0f;
    int rate;
    float currentVolume;

    if (sw)
    {
        result = system->getSoftwareFormat(&rate, 0, 0); // Get mixer rate
        errorCheck();

        result = sw->channel->getDSPClock(0, &dspclock); // Get the reference
            // clock, which is the parent channel group
        errorCheck();
        result = sw->channel->getVolume(&currentVolume);
        errorCheck();
        result = sw->channel->addFadePoint(dspclock, currentVolume); // Add a 
            // fade point at 'now' with full volume.
        errorCheck();
        result = sw->channel->addFadePoint(dspclock + (rate * fadeDurationSecs),
            0.0f); // Add a fade point 5 seconds later at 0 volume.
        errorCheck();
        result = sw->channel->setDelay(0, dspclock + (rate * fadeDurationSecs),
            true);  // Add a delayed stop command at 5 seconds 
                    // ('stopchannels = true')
        errorCheck();
    }
    isStreamPlaying = false;
}

void
AudioManager::setStreamVolume(float volume)
{
    SoundWrapper *sw = get(currentlyPlayingStream);
    result = sw->channel->setVolume(volume);
    errorCheck();
}

void 
AudioManager::update()
{
    result = system->update();
    errorCheck();

    if (fadingOut && Clock::getMilliseconds() - fadeOutStartTime > 
        fadeOutDuration)
    {
        fadingOut = false;
        playStreamFadeIn(nextStream, fadeOutDuration, fadeInVolume);
    }
}

float 
AudioManager::getSoundLengthMS(const std::string &soundPath)
{
    unsigned int val;
    result = get(soundPath)->sound->getLength(&val, FMOD_TIMEUNIT_MS);
    errorCheck();
    return float(val);
}