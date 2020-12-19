#include "Character.h"

#include <Box2D/Box2D.h>
#include <allegro5/allegro_primitives.h>

#include "Box.h"
#include "Sensor.h"

#include "../util/ActionBinder.h"
#include "../util/Globals.h"
#include "../util/Tools.h"

Character::Character(const float x,
                     const float y,
                     ALLEGRO_BITMAP* image,
                     b2World* world)
    : Box(x, y, 0.8, 2.5, world) {
  timer_sound_delay = 0;
  timer_jump_delay = 0;
  counter_sensor_contact = 0;
  velocity_old = 0;
  tick = 0;
  frame = 0;
  landed = true;

  direction = false;
  color = al_map_rgb(0, 0, 255);

  // Modify body
  body->SetType(b2_dynamicBody);
  body->SetTransform(b2Vec2(x, y + 0.40f), getAngle());
  body->SetFixedRotation(true);

  // Create sensor
  sensor_box = new Sensor(x, y - 0.55f, getWidth() * 0.4f, 0.6f);
  sensor_box->init(world, getBody());

  // Image
  setImage(image);

  jump.load_wav("sfx/jump.wav");
  land.load_wav("sfx/land.wav");

  // Slice character images up
  for (int i = 0; i < 15; i++) {
    sprites[i] = al_create_sub_bitmap(sprite, i * 32, 0, 32, 64);
  }
}

// Destructor
Character::~Character() {
  // Remove body

  delete sensor_box;

  // Remove sprite as it was created here
  // if (sprite) {
  // al_destroy_bitmap(sprite);
  //}
}

// Get sensor body
b2Body* Character::getSensorBody() {
  return sensor_box->getBody();
}

void Character::update(b2World* world) {
  // Store collision status
  bool sensor_colliding = sensor_box->isColliding();

  if (sensor_colliding) {
    counter_sensor_contact++;
  } else {
    counter_sensor_contact = 0;
  }

  if (counter_sensor_contact > 25) {
    landed = true;
  }

  if ((body->GetLinearVelocity().y >= -0.01f &&
       body->GetLinearVelocity().y <= 0.01f) &&
      sensor_colliding && velocity_old < -0.01f) {
    // Volume of land sound
    float land_volume = velocity_old / 20;
    if (land_volume > 1) {
      land_volume = 1;
    }
    land.play(land_volume);
    landed = true;
  }

  velocity_old = body->GetLinearVelocity().y;

  // special case of sensor_box colliding, will keep check in
  if (sensor_box->isCollidingWithDynamicBody()) {
    landed = true;
  }

  // Animation speed
  int ticks_per_frame = 5;
  if (!sensor_colliding) {
    ticks_per_frame = 2;
  } else {
    body->GetFixtureList()->SetFriction(0.0f);
  }

  tick++;
  if (tick >= ticks_per_frame) {
    frame++;
    tick = 0;
  }
  if (frame == 14) {
    frame = 0;
  }

  // Position of body
  b2Vec2 position = body->GetPosition();

  // Y velocity of body
  const double yVel = getBody()->GetLinearVelocity().y;

  // TODO (Danny#1#): Movement too slow
  // Velocity on ground
  const float x_velocity_ground = 6.0f;

  // Velocity impulse while in air
  const float x_velocity_air = 3.1f;

  // Max air velocity
  const float x_velocity_air_max = 4.0f;

  if (ActionBinder::actionHeld(ACTION_LEFT)) {
    direction = false;
    if (sensor_colliding) {
      body->SetLinearVelocity(b2Vec2(-x_velocity_ground, yVel));
    } else if (getBody()->GetLinearVelocity().x > -x_velocity_air_max) {
      body->ApplyLinearImpulse(b2Vec2(-x_velocity_air, 0), position, true);
    }
  } else if (ActionBinder::actionHeld(ACTION_RIGHT)) {
    direction = true;
    if (sensor_colliding) {
      body->SetLinearVelocity(b2Vec2(x_velocity_ground, yVel));
    } else if (getBody()->GetLinearVelocity().x < x_velocity_air_max) {
      body->ApplyLinearImpulse(b2Vec2(x_velocity_air, 0), position, true);
    }
  } else if (sensor_colliding) {
    body->SetLinearVelocity(b2Vec2(0, body->GetLinearVelocity().y));
  }

  // Jumping Jimothy
  timer_jump_delay++;
  if (ActionBinder::actionBegun(ACTION_A) && sensor_colliding &&
      body->GetLinearVelocity().y < 0.1f && landed) {
    if (timer_jump_delay > 20) {
      timer_jump_delay = 0;
      body->ApplyLinearImpulse(b2Vec2(0, 17), position, true);
      landed = false;
      if (timer_sound_delay > 10) {
        jump.play_random_frequency(90, 110);
        timer_sound_delay = 0;
      }
    }
  }
  timer_sound_delay++;

  // Fixed Danny's poop code by getting rid if it =)
  if (sensor_colliding) {
    color = al_map_rgb(50, 100, 255);
  } else {
    color = al_map_rgb(0, 0, 255);
  }
}

void Character::draw() {
  // Transform for draw
  ALLEGRO_TRANSFORM trans, prevTrans;

  // back up the current transform
  al_copy_transform(&prevTrans, al_get_current_transform());

  // scale using the new transform
  al_identity_transform(&trans);

  al_rotate_transform(&trans, -getAngle());
  al_translate_transform(&trans, getX() * 20, -getY() * 20);

  al_use_transform(&trans);

  // Nice debug draw for player hitbox
  // al_draw_filled_rectangle( -(width/2) * 20 + 1, -(height/2)*20 + 1,
  // (width/2) * 20 - 1, (height/2) * 20 - 1,al_map_rgb(255,25,64));

  // Draw offset x
  const int x_offset = -8;

  // Draw offset y
  const int y_offset = -13;

  if (direction) {
    if (body->GetLinearVelocity().Length() > 0.1f) {
      al_draw_bitmap(sprites[frame], -(getWidth() / 2) * 20 + x_offset,
                     (-(getHeight() / 2) * 20) + y_offset, 0);
    } else {
      al_draw_bitmap(sprites[14], -(getWidth() / 2) * 20 + x_offset,
                     (-(getHeight() / 2) * 20) + y_offset, 0);
    }
  } else {
    if (body->GetLinearVelocity().Length() > 0.1f) {
      al_draw_bitmap(sprites[frame], -(getWidth() / 2) * 20 + x_offset,
                     (-(getHeight() / 2) * 20) + y_offset,
                     ALLEGRO_FLIP_HORIZONTAL);
    } else {
      al_draw_bitmap(sprites[14], -(getWidth() / 2) * 20 + x_offset,
                     (-(getHeight() / 2) * 20) + y_offset,
                     ALLEGRO_FLIP_HORIZONTAL);
    }
  }

  // restore the old transform
  al_use_transform(&prevTrans);

  // Nice debug draw for sensor box
  // sensor_box -> draw();
}

// Get type
int Character::getType() {
  return CHARACTER;
}