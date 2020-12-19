#include "Goat.h"

#include "../util/Globals.h"
#include "../util/Tools.h"

// We'll use this for the goat
Goat::Goat(const float x,
           const float y,
           Character* character,
           ALLEGRO_BITMAP* image,
           b2World* world)
    : Box(x, y, 1.6, 3.2, world) {
  goat_frame = 0;
  goat_tick = 0;

  // Modify body
  body->SetType(b2_dynamicBody);

  gameCharacter = character;

  // Image
  setImage(image);

  // Cut it up
  for (int i = 0; i < 16; i++) {
    goat_images[i] = al_create_sub_bitmap(sprite, i * 32, 0, 32, 64);
  }

  // Sensor
  sensor_box = new Sensor(x, y, getWidth(), getHeight());
  sensor_box->init(world, getBody());
}

// Destructor
Goat::~Goat() {
  delete sensor_box;
}

// Draw box to screen
void Goat::draw() {
  goat_tick++;

  if (goat_tick > 10) {
    goat_frame++;
    goat_tick = 0;
  }
  if (goat_frame > 14) {
    goat_frame = 0;
  }

  // Draw transform
  ALLEGRO_TRANSFORM trans, prevTrans;

  // back up the current transform
  al_copy_transform(&prevTrans, al_get_current_transform());

  // scale using the new transform
  al_identity_transform(&trans);

  al_rotate_transform(&trans, -getAngle());
  al_translate_transform(&trans, getX() * 20, getY() * -20);

  al_use_transform(&trans);

  al_draw_bitmap(goat_images[goat_frame], -(getWidth() / 2) * 20,
                 -(getHeight() / 2) * 20, 0);

  al_use_transform(&prevTrans);
}

bool Goat::getWinCondition() {
  return gameCharacter &&
         sensor_box->isCollidingWithBody(gameCharacter->getBody());
}

// Get box type
int Goat::getType() {
  return GOAT;
}
