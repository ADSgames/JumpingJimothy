/**
 * Box
 * Danny Van Stemp
 * A base physics enabled box type
 * 05/05/2017
 **/

#ifndef BOX_H
#define BOX_H

#include <Box2D/Box2D.h>
#include <allegro5/allegro.h>

class Box {
 public:
  // Constructor
  Box();
  Box(const float x,
      const float y,
      const float width,
      const float height,
      b2World* world);

  // Destructor
  virtual ~Box();

  // Create body
  void createBody(b2World* world);

  // Draw
  virtual void draw() = 0;

  // Update logic
  virtual void update(b2World* world) = 0;

  // Get type
  virtual int getType() = 0;

  // Set images
  void setImage(ALLEGRO_BITMAP* image);

  // Get X
  float getX();

  // Get Y
  float getY();

  // Get width
  float getWidth();

  // Get height
  float getHeight();

  // Get angle
  float getAngle();

  // Get physics body
  b2Body* getBody();

  // Set paused
  void setPaused(const bool pause);

  // Is pausable
  virtual bool isPausable();

  // Set orientation
  void setOrientation(const int orientation);

 protected:
  // Orientation
  int orientation;

  // Colour
  ALLEGRO_COLOR color;

  // Sprite for box
  ALLEGRO_BITMAP* sprite;

  // Pointer to physics body
  b2Body* body;

  // Paused state
  bool isPaused;

  // Snapshot of velocity
  b2Vec2 paused_velocity;

  // Snapshot of angular velocity
  float paused_angular_velocity;

 protected:
  // Position
  b2Vec2 initial_position;

  // Size
  b2Vec2 initial_size;

  // Angle
  float initial_angle;
};

#endif  // BOX_H
