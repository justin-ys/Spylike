#ifndef SPYLIKE_AUDIO_H
#define SPYLIKE_AUDIO_H

#include "miniaudio.h"
#include "event.h"
#include <memory>

class AudioManager : public EventHandler {
	protected:
		bool playing = false;
	public:
		virtual void playMusic(std::string track, float volume=50, bool loop=true) = 0;
		virtual void playSound(std::string sound, float volume=50) = 0;
		virtual void stopMusic() = 0;
		virtual void pauseMusic() = 0;
		virtual void resumeMusic() = 0;
		virtual void setMusicVolume(float volume) = 0;
		void on_event(Event& e);
		bool isPlaying() { return playing; }
};

class MiniaudioManager : public AudioManager {
	std::string rootPath;
	ma_engine* engine = nullptr;
	ma_sound* cMusic = nullptr;
	public:
        MiniaudioManager(std::string rootPath);
		void playMusic(std::string track, float volume=0.5, bool loop=true) override;
		void playSound(std::string sound, float volume=0.5) override;
		void stopMusic() override;
        void pauseMusic() override;
		void resumeMusic() override;
		void setMusicVolume(float volume) override;
};

#endif
