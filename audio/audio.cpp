#define MINIAUDIO_IMPLEMENTATION
#include "logger.h"
#include "miniaudio.h"

#include "audio.h"

#include <memory>

extern SpylikeLogger LOGGER;

void AudioManager::on_event(Event& e) {
	if (e.type == "AUDIO_PlayMusic") {
		SpylikeEvents::AudioPlayEvent& ap = dynamic_cast<SpylikeEvents::AudioPlayEvent&>(e);
		playMusic(ap.sound, ap.volume, ap.loop);
	}
	else if (e.type == "AUDIO_PauseMusic") {
		pauseMusic();
	}
}

MiniaudioManager::MiniaudioManager(std::string rootPath) : rootPath{rootPath} {
	LOGGER.log("Initalizing audio manager", DEBUG);
	engine = new ma_engine;
	ma_result result = ma_engine_init(NULL, engine);
	if (result != MA_SUCCESS) throw "Error initalizing audio engine.";
}

void MiniaudioManager::playMusic(std::string track, float volume, bool loop) {
	LOGGER.log("Playing music " + track, DEBUG);
	if (playing) stopMusic();
	playing = true;
	cMusic = new ma_sound;
	std::string path = rootPath + "music/" + track;
	const char* path_str = path.c_str();
	ma_result res = ma_sound_init_from_file(engine, path_str, 0, NULL, NULL, cMusic);
	ma_sound_set_volume(cMusic, volume);
	if (loop) {
		ma_sound_set_looping(cMusic, true);
	}
	ma_sound_start(cMusic);
}

void MiniaudioManager::playSound(std::string sound, float volume) {
	const char* path = (rootPath + "sound/" + sound).c_str();
	ma_engine_play_sound(engine, path, NULL);
}

void MiniaudioManager::stopMusic() {
	LOGGER.log("Stopping music", DEBUG);
	if (cMusic && engine && playing) {
		ma_sound_uninit(cMusic);
		delete cMusic;
	}
	cMusic = nullptr;
	playing = false;
}

void MiniaudioManager::pauseMusic() {
	LOGGER.log("Pausing music", DEBUG);
	if (cMusic) ma_sound_stop(cMusic);
}

void MiniaudioManager::resumeMusic() {
	if (cMusic) ma_sound_start(cMusic);
}

void MiniaudioManager::setMusicVolume(float volume) {
	if (cMusic) ma_sound_set_volume(cMusic, volume);
}
