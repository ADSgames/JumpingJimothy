/**
 * Label
 * Danny Van Stemp and Allan Legemaate
 * UI Button
 * 25/11/2018
 **/

#ifndef LABEL_H
#define LABEL_H

#include "UIElement.h"

class Label : public UIElement {
 public:
  // Constructor
  Label();

  // Detailed constructor
  Label(const int x,
        const int y,
        std::string text,
        std::string id,
        ALLEGRO_FONT* font);

  // Draw
  virtual void draw() override;

  // Update
  virtual void update() override;
};

#endif  // LABEL_H
