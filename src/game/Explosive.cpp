#include "Explosive.h"

#include <allegro5/allegro_primitives.h>
#include <vector>

#include "../util/Globals.h"

// Constructor
Explosive::Explosive(const float x,
                     const float y,
                     const bool affectCharacter,
                     Character* character,
                     b2World* world)
    : Box(x, y, 1.55f, 1.55f, world) {
  blastRadius = 10;
  blastPower = 1000;

  gameCharacter = character;
  affect_character = affectCharacter;

  color = al_map_rgb(255, 0, 0);

  // Modify body
  body->SetType(b2_kinematicBody);
}

// Destructor
Explosive::~Explosive(){};

// subclass b2QueryCallback for proximity query callback
class MyQueryCallback : public b2QueryCallback {
 public:
  // All bodies found by query
  std::vector<b2Body*> foundBodies;

  bool ReportFixture(b2Fixture* fixture) {
    foundBodies.push_back(fixture->GetBody());
    return true;  // keep going to find all fixtures in the query area
  }
};

void Explosive::update(b2World* world) {
  // Center of blast
  b2Vec2 center = body->GetPosition();

  // Find all fixtures within blast radius AABB
  MyQueryCallback queryCallback;

  // AABB
  b2AABB aabb;
  aabb.lowerBound = center - b2Vec2(blastRadius, blastRadius);
  aabb.upperBound = center + b2Vec2(blastRadius, blastRadius);
  world->QueryAABB(&queryCallback, aabb);

  is_exploding = false;

  // Check which of these have their center of mass within the blast radius
  for (unsigned int i = 0; i < queryCallback.foundBodies.size(); i++) {
    // Temp body
    b2Body* newBody = queryCallback.foundBodies.at(i);

    // Center of world
    b2Vec2 bodyCom = newBody->GetWorldCenter();

    // Ignore bodies outside the blast range
    if ((bodyCom - center).Length() >= blastRadius) {
      continue;
    } else {
      applyBlastImpulse(
          newBody, center, bodyCom,
          blastPower * 0.05f);  // scale blast power to roughly match results of
                                // other methods at 32 rays
    }
  }
}

// this is the same for proximity and raycast methods so we can put it in a
// common function
void Explosive::applyBlastImpulse(b2Body* newBody,
                                  b2Vec2 blastCenter,
                                  b2Vec2 applyPoint,
                                  const float blastPower) {
  // ignore the grenade itself, and any non-dynamic bodies
  // Fricking feet, how do they work?
  if (newBody == body || newBody->GetType() != b2_dynamicBody ||
      ((newBody == gameCharacter->getBody() ||
        newBody == gameCharacter->getSensorBody()) &&
       !affect_character)) {
    return;
  }

  // Blast direction
  b2Vec2 blastDir = applyPoint - blastCenter;

  // Distance
  const float distance = blastDir.Normalize();

  // Ignore bodies exactly at the blast point - blast direction is undefined
  if (distance < 0.01f) {
    return;
  }

  // Distance
  const float invDistance = 1 / distance;

  // Magnitude
  float impulseMag = blastPower * invDistance * invDistance;

  impulseMag = b2Min(impulseMag, 500.0f);

  is_exploding = true;

  if (orientation == 0) {
    newBody->ApplyLinearImpulse(impulseMag * blastDir, applyPoint, true);
  } else {
    // Magnitude
    const float magnitude = 0.2;

    // Direction of impulse
    b2Vec2 new_direction;

    if (orientation == 1)
      new_direction = b2Vec2(0, magnitude);
    else if (orientation == 3)
      new_direction = b2Vec2(0, -magnitude);
    else if (orientation == 2)
      new_direction = b2Vec2(magnitude, 0);
    else
      new_direction = b2Vec2(-magnitude, 0);

    newBody->ApplyLinearImpulse(impulseMag * new_direction, applyPoint, true);
  }
}

void Explosive::draw() {
  // Transform for drawing
  ALLEGRO_TRANSFORM trans, prevTrans;

  // back up the current transform
  al_copy_transform(&prevTrans, al_get_current_transform());

  // scale using the new transform
  al_identity_transform(&trans);

  al_rotate_transform(&trans, -getAngle());
  al_translate_transform(&trans, getX() * 20, getY() * -20);

  al_use_transform(&trans);

  if (is_exploding) {
    // al_draw_filled_circle(0,0,200,al_map_rgba(255,0,0,1));
    // one day
  }

  if (affect_character) {
    al_draw_filled_rectangle(-(getWidth() / 2) * 20 + 1,
                             -(getHeight() / 2) * 20 + 1,
                             (getWidth() / 2) * 20 - 1,
                             (getHeight() / 2) * 20 - 1, al_map_rgb(255, 0, 0));
  } else {
    al_draw_filled_rectangle(-(getWidth() / 2) * 20 + 1,
                             -(getHeight() / 2) * 20 + 1,
                             (getWidth() / 2) * 20 - 1,
                             (getHeight() / 2) * 20 - 1, al_map_rgb(0, 255, 0));
  }

  // PI/2 is a quarter turn. Editor boxes orientation is a range from 1-4.
  // So we have a quarter turn * 1-4, creating a quarter turn, half turn, ect.
  // - PI/2 is because we start rotated right a quarter turn.
  const float new_angle = (PI / 2) * orientation - PI / 2;
  al_draw_rotated_bitmap(sprite, 16, 16, 0, 0, new_angle, 0);

  // restore the old transform
  al_use_transform(&prevTrans);
}

// Get box type
int Explosive::getType() {
  return EXPLOSIVE;
}
