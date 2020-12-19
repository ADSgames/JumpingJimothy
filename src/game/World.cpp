#include "World.h"

World::World(b2Vec2 gravity,
             float timeStep,
             int32 velocityIterations,
             int32 positionIterations,
             bool doSleep) {
  // Box2D world parameters
  this->gravity = gravity;
  this->timeStep = timeStep;
  this->velocityIterations = velocityIterations;
  this->positionIterations = positionIterations;
  this->doSleep = doSleep;

  // Create world
  bWorld = new b2World(this->gravity);
}

World::~World() {}

// Get b2 world
b2World* World::getB2World() {
  return this->bWorld;
}

// Step
void World::step() {
  this->bWorld->Step(timeStep, velocityIterations, positionIterations);
}
