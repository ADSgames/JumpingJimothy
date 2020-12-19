#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include "Sound.h"

class MusicManager {
 public:
  static Sound* menu_music;
  static Sound* game_music;

  static void load();

  static void destroy();
};

#endif  // MUSICMANAGER_H
