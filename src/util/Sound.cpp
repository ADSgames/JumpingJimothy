#include "Sound.h"
#include "Config.h"
#include "Tools.h"

// Ctor
Sound::Sound() {
  sample = nullptr;
  sample_id = nullptr;

  is_wav = false;
  is_playing = false;
}

// Dtor
Sound::~Sound() {
  al_destroy_sample(sample);
}

// Get sample ID
ALLEGRO_SAMPLE_ID* Sound::getSampleId() {
  return sample_id;
}

// Get sample
ALLEGRO_SAMPLE* Sound::getSample() {
  return sample;
}

// Load WAV from file
void Sound::load_wav(const std::string& path) {
  is_wav = true;
  sample = tools::load_sample_ex(path);
}

// Load OGG frim file
void Sound::load_ogg(const std::string& path) {
  is_wav = false;
  sample = tools::load_sample_ex(path);
}

// Play sound
void Sound::play() {
  this->play(1.0f);
}

// Play sound at volume
void Sound::play(const float volume) {
  if (!sample)
    return;

  if (is_wav && Config::getBooleanValue("sfx_enabled")) {
    is_playing = true;
    al_play_sample(sample, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, nullptr);
  } else if (!is_wav && Config::getBooleanValue("music_enabled")) {
    is_playing = true;
    al_play_sample(sample, volume, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, sample_id);
  }
}

// Play at sample random frequency
void Sound::play_random_frequency(const int newMin, const int newMax) {
  if (!sample)
    return;

  if (is_wav && Config::getBooleanValue("sfx_enabled")) {
    al_play_sample(sample, 1.0, 0.0,
                   (float)tools::random_int(newMin, newMax) / 100,
                   ALLEGRO_PLAYMODE_ONCE, nullptr);
  }
}

// Stop sound
void Sound::stop() {
  if (!sample)
    return;

  if (!is_wav) {
    al_stop_samples();
  }

  is_playing = false;
}
