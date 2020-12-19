#include "Sensor.h"

#include <Box2D/Box2D.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_primitives.h>

#include "../util/Globals.h"
#include "../util/KeyListener.h"

Sensor::Sensor(const float x,
               const float y,
               const float width,
               const float height)
    : Box(x, y, width, height, nullptr) {}

void Sensor::init(b2World* world, b2Body* parentBody) {
  // Set world
  gameWorld = world;

  // Create body
  createBody(BODY_DYNAMIC, true);

  // Override defaults
  b2Fixture* fixPointer = body->GetFixtureList();
  fixPointer->SetDensity(0.0001f);
  fixPointer->SetFriction(0.0f);
  fixPointer->SetSensor(true);

  // Feet anchor
  b2Vec2 FeetAnchor(0, 0);
  b2WeldJointDef* jointDef = new b2WeldJointDef();
  jointDef->Initialize(getBody(), parentBody, FeetAnchor);
  jointDef->collideConnected = false;
  jointDef->referenceAngle = 0;
  gameWorld->CreateJoint(jointDef);
}

// Create body
void Sensor::createBody(int bodyType, bool fixedRotation) {
  // World must be set
  if (!gameWorld) {
    return;
  }

  // Body
  b2BodyDef bodyDef;

  // Body types
  switch (bodyType) {
    case BODY_DYNAMIC:
      bodyDef.type = b2_dynamicBody;
      break;
    case BODY_KINEMATIC:
      bodyDef.type = b2_kinematicBody;
      break;
    default:
      bodyDef.type = b2_dynamicBody;
      break;
  }

  bodyDef.position.Set(initial_position.x, initial_position.y);
  body = gameWorld->CreateBody(&bodyDef);

  if (fixedRotation) {
    body->SetFixedRotation(true);
  }

  // Define another box shape for our dynamic body.
  b2PolygonShape dynamicBox;
  dynamicBox.SetAsBox(initial_size.x / 2, initial_size.y / 2);

  // Define the dynamic body fixture.
  b2FixtureDef fixtureDef;
  fixtureDef.shape = &dynamicBox;

  // Set the box density to be non-zero, so it will be dynamic.
  fixtureDef.density = 1.0f;

  // Override the default friction.
  fixtureDef.friction = 0.3f;

  // Add the shape to the body.
  body->CreateFixture(&fixtureDef);
}

bool Sensor::isColliding() {
  // Parse contacts
  for (b2ContactEdge* contact = body->GetContactList(); contact;
       contact = contact->next) {
    return true;
  }
  return false;
}

bool Sensor::isCollidingWithDynamicBody() {
  // Parse contacts
  for (b2ContactEdge* contact = body->GetContactList(); contact;
       contact = contact->next) {
    if (contact->other->GetType() == b2_dynamicBody) {
      return true;
    }
  }
  return false;
}

bool Sensor::isCollidingWithBody(b2Body* newBody) {
  // Parse contacts
  for (b2ContactEdge* contact = body->GetContactList(); contact;
       contact = contact->next) {
    if (contact->other == newBody) {
      return true;
    }
  }
  return false;
}

// Draw box to screen
void Sensor::draw() {
  // Draw transform
  ALLEGRO_TRANSFORM trans, prevTrans;

  // back up the current transform
  al_copy_transform(&prevTrans, al_get_current_transform());

  // scale using the new transform
  al_identity_transform(&trans);

  al_rotate_transform(&trans, -getAngle());
  al_translate_transform(&trans, getX() * 20, getY() * -20);

  al_use_transform(&trans);

  al_draw_filled_rectangle(-(getWidth() / 2) * 20, -(getHeight() / 2) * 20,
                           (getWidth() / 2) * 20, (getHeight() / 2) * 20,
                           al_map_rgb(255, 255, 0));

  // restore the old transform
  al_use_transform(&prevTrans);
}

// Get box type
int Sensor::getType() {
  return BOX;
}