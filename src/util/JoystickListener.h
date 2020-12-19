/**
 * Joystick Listener
 * Allan Legemaate
 * Listens for joystick buttons JUST pressed or JUST released
 *   since the last tick
 * 23/01/2017
 **/

#ifndef JOYSTICKLISTENER_H
#define JOYSTICKLISTENER_H

#include <allegro5/allegro.h>

#include "JoystickCodes.h"

class JoystickListener {
 public:
  void on_event(ALLEGRO_EVENT_TYPE event_type, const int keycode);
  void on_event(ALLEGRO_EVENT_TYPE event_type,
                const int stick,
                const int axis,
                const float position);
  void update();

  static const int JOY_MAX_STICKS = _AL_MAX_JOYSTICK_STICKS;
  static const int JOY_MAX_AXES = _AL_MAX_JOYSTICK_AXES;
  static const int JOY_MAX_STICK_BOOLEANS =
      _AL_MAX_JOYSTICK_STICKS * _AL_MAX_JOYSTICK_AXES * 2;
  static const int JOY_MAX_BUTTONS = _AL_MAX_JOYSTICK_BUTTONS;

  static bool button[JOY_MAX_BUTTONS];
  static bool buttonPressed[JOY_MAX_BUTTONS];
  static bool buttonReleased[JOY_MAX_BUTTONS];
  static bool anyButtonPressed;

  static bool stick[JOY_MAX_STICK_BOOLEANS];
  static bool stickMoved[JOY_MAX_STICK_BOOLEANS];
  static bool stickReleased[JOY_MAX_STICK_BOOLEANS];

 protected:
 private:
  static bool lastTicksButton[JOY_MAX_BUTTONS];

  static bool lastTicksStick[JOY_MAX_STICK_BOOLEANS];
};

#endif  // JOYSTICKLISTENER_H
