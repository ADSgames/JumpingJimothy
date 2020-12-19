/**
 * Character
 * Danny Van Stemp
 * The game character
 * 05/05/2017
 **/

#ifndef CHARACTER_H
#define CHARACTER_H

#include "../util/Sound.h"
#include "Sensor.h"

// Number of character images
const int NUM_CHARACTER_IMAGES = 20;

class Character : public Box {
 public:
  // Constructor
  Character(const float x,
            const float y,
            ALLEGRO_BITMAP* image,
            b2World* world);

  // Destructor
  virtual ~Character();

  // Draw
  virtual void draw() override;

  // Update
  virtual void update(b2World* world) override;

  // Get sensor body
  b2Body* getSensorBody();

  // Get type
  virtual int getType() override;

 private:
  // Animation tick
  int tick;

  // Animation frame
  int frame;

  // Sound delay for jump sound
  int timer_sound_delay;

  // Jump delay
  int timer_jump_delay;

  // Sensing counter
  int counter_sensor_contact;

  // Landed on ground
  bool landed;

  // Previous velocity
  float velocity_old;

  // Direciton
  bool direction;

  // Sensor for collisions
  Sensor* sensor_box;

  // Character images
  ALLEGRO_BITMAP* sprites[NUM_CHARACTER_IMAGES];

  // Sample for jump
  Sound jump;

  // Sample for landing
  Sound land;
};

#endif  // CHARACTER_H
