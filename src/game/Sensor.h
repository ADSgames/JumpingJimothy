/**
 * Sensor
 * Danny Vanstemp
 * Senses physics bodies
 * 05/05/2017
 **/

#ifndef SENSOR_H
#define SENSOR_H

#include "Box.h"

class Sensor : public Box {
 public:
  // Constructor
  Sensor(const float x, const float y, const float width, const float height);

  void init(b2World*, b2Body*);
  void createBody(int bodyType, bool fixedRotation);

  // Colliding with something
  bool isColliding();

  // Colliding with dynamic body
  bool isCollidingWithDynamicBody();

  // Colliding with any body
  bool isCollidingWithBody(b2Body*);

  // Update sensor
  virtual void update(b2World* world) override{};

  // Draw sensor
  virtual void draw() override;

  // Get type
  virtual int getType() override;

 private:
  b2World* gameWorld;
};

#endif  // SENSOR_H
