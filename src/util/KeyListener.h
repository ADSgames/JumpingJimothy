/**
 * Key Listener
 * Allan Legemaate
 * Listens for keys JUST pressed or JUST released
 *   since the last tick
 * 18/01/2017
 **/

#ifndef KEYLISTENER_H
#define KEYLISTENER_H

#include <allegro5/allegro.h>

class KeyListener {
 public:
  static bool anyKeyPressed;

  void on_event(ALLEGRO_EVENT_TYPE event_type, const int keycode);
  void update();

  static bool key[ALLEGRO_KEY_MAX];
  static bool keyPressed[ALLEGRO_KEY_MAX];
  static bool keyReleased[ALLEGRO_KEY_MAX];

  static int lastKeyPressed;
  static int lastKeyReleased;

 protected:
 private:
  static bool lastTicksKey[ALLEGRO_KEY_MAX];
};

#endif  // KEYLISTENER_H
