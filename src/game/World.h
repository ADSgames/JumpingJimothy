#ifndef WORLD_H
#define WORLD_H

#include <Box2D/Box2D.h>

class World {
 public:
  // Constructor
  World(b2Vec2 gravity,
        float timeStep,
        int32 velocityIterations,
        int32 positionIterations,
        bool doSleep);

  // Destructor
  virtual ~World();

  // Get b2 world
  b2World* getB2World();

  // Step
  void step();

 protected:
 private:
  // Box 2d game world
  b2World* bWorld;

  // Vars
  b2Vec2 gravity;
  float timeStep;
  int32 velocityIterations;
  int32 positionIterations;
  bool doSleep;
};

#endif  // WORLD_H
