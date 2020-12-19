#include "DynamicBox.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "../util/Globals.h"
#include "../util/Tools.h"

DynamicBox::DynamicBox(const float x,
                       const float y,
                       const float velX,
                       const float velY,
                       b2World* world)
    : Box(x, y, 1.55f, 1.55f, world) {
  color = al_map_rgb(255, 0, 0);

  // Modify body
  body->SetType(b2_dynamicBody);
  body->SetLinearVelocity(b2Vec2(velX, velY));
}

// Draw box to screen
void DynamicBox::draw() {
  // Allegro transform
  ALLEGRO_TRANSFORM trans, prevTrans;

  // back up the current transform
  al_copy_transform(&prevTrans, al_get_current_transform());

  // Scale rotate and translate
  al_identity_transform(&trans);
  al_rotate_transform(&trans, -getAngle());
  al_translate_transform(&trans, getX() * 20, getY() * -20);
  al_use_transform(&trans);

  // Velocity colouring
  b2Vec2 draw_velocity = b2Vec2(0, 0);

  if (isPaused) {
    paused_velocity = b2Vec2(paused_velocity.x, paused_velocity.y);
  } else {
    paused_velocity =
        b2Vec2(body->GetLinearVelocity().x, body->GetLinearVelocity().y);
  }

  // Draw colour
  al_draw_filled_rectangle(
      -(getWidth() / 2) * 20 + 1, -(getHeight() / 2) * 20 + 1,
      (getWidth() / 2) * 20 - 1, (getHeight() / 2) * 20 - 1,
      al_map_rgb(tools::clamp(0, 255, int(draw_velocity.y * -10)),
                 tools::clamp(0, 255, 255 - int(draw_velocity.y * -10)), 0));

  // Draw image
  al_draw_bitmap(sprite, -(getWidth() / 2) * 20, -(getHeight() / 2) * 20, 0);

  // restore the old transform
  al_use_transform(&prevTrans);
}

// Update does nothing
void DynamicBox::update(b2World* world){};

// Get box type
int DynamicBox::getType() {
  return BOX;
}

// Is pausable
bool DynamicBox::isPausable() {
  return true;
}
