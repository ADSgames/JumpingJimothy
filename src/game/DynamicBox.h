/**
 * Dyanmic Box
 * Danny Van Stemp
 * Physics based box that is influenced by
 *   gravity.
 * 30/07/2017
 **/

#ifndef DYNAMICBOX_H
#define DYNAMICBOX_H

#include "Box.h"

class DynamicBox : public Box {
 public:
  // Constructor
  DynamicBox(const float x,
             const float y,
             const float velX,
             const float velY,
             b2World* world);

  // Draw
  virtual void draw() override;

  // Update
  virtual void update(b2World* world) override;

  // Get type
  virtual int getType() override;

  // Is pausable
  virtual bool isPausable() override;
};

#endif  // DYNAMICBOX_H
