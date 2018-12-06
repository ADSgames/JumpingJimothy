/**
 * Joystick Codes
 * Danny Van Stemp
 * Defines for codes used for joystick
 * 20/11/2018
**/

#ifndef JOYSTICKCODES_H
#define JOYSTICKCODES_H

enum {
  JOY_XBOX_A   = 0,
  JOY_XBOX_B   = 1,
  JOY_XBOX_X   = 2,
  JOY_XBOX_Y   = 3,
  JOY_XBOX_BUMPER_RIGHT    = 4,
  JOY_XBOX_BUMPER_LEFT   = 5,
  JOY_XBOX_BACK    = 8,
  JOY_XBOX_START   = 9,
  JOY_XBOX_PAD_RIGHT   = 10,
  JOY_XBOX_PAD_LEFT    = 11,
  JOY_XBOX_PAD_DOWN    = 12,
  JOY_XBOX_PAD_UP    = 13
};

enum {
  LEFT_STICK_LEFT      = 1,
  LEFT_STICK_RIGHT     = 0,
  LEFT_STICK_DOWN      = 2,
  LEFT_STICK_UP        = 3,
  RIGHT_STICK_LEFT     = 7,
  RIGHT_STICK_RIGHT    = 6,
  RIGHT_STICK_DOWN     = 8,
  RIGHT_STICK_UP       = 9,
  LEFT_TRIGGER         = 12,
  RIGHT_TRIGGER        = 18
};

#endif