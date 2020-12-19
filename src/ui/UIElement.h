/**
 * UI Element
 * Danny Van Stemp and Allan Legemaate
 * Top level UI element
 * 24/09/2017
 **/

#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <allegro5/allegro_font.h>

#include <string>

class UIElement {
 public:
  // Constructor
  UIElement();
  UIElement(const int x,
            const int y,
            std::string text,
            std::string id,
            ALLEGRO_FONT* font);

  // Dtor
  virtual ~UIElement();

  // Get X
  int getX();

  // Get y
  int getY();

  // Get text of element
  std::string getText();

  // Get element ID
  std::string getId();

  // Hide element
  void hide();

  // Show element
  void show();

  // Is visible or not
  bool isVisible();

  // Set visibility
  void setVisibility(bool visible);

  // Disable element
  void disable();

  // Enable element
  void enable();

  // Is enabled or not
  bool isEnabled();

  // Set transparency level
  void setTransparency(const float alpha);

  // Set image rotation
  void setBitmapRotationAngle(const float rotation);

  // Set x
  void setX(const int x);

  // Set y
  void setY(const int y);

  // Set text colour
  void setTextColour(ALLEGRO_COLOR colour);

  // Set background colour
  void setBackgroundColour(ALLEGRO_COLOR colour);

  // Set cell fill
  void setCellFillTransparent(const bool n);

  // Set text justification
  void setTextJustification(const int justification);

  // Element width
  int getWidth();

  // Element height
  int getHeight();

  // Set padding
  void setPadding(const int x, const int y);

  // Set position
  void setPosition(const int x, const int y);

  // Set element size
  void setSize(const int width, const int height);

  // Set text
  void setText(const std::string& text);

  // Set id
  void setId(const std::string& id);

  // Set image
  void setImage(ALLEGRO_BITMAP* image);

  // Set font
  void setFont(ALLEGRO_FONT* font);

  // Set background visibility
  void setVisibleBackground(const bool b);

  // Set width
  void setWidth(const int width);

  // Set height
  void setHeight(const int height);

  // Can focus
  virtual bool canFocus();

  // Focus
  void focus();

  // Unfocus
  void unfocus();

  // Set border thickness
  void setBorderThickness(const int thickness);

  // Disable hover effect
  void disableHoverEffect();

  // Disable hover effect
  void enableHoverEffect();

  // Mouse over
  bool hover();

  // Clicked
  bool clicked();

  // Update element
  virtual void update() = 0;

  // Draw element
  virtual void draw() = 0;

  // Set callbacks
  void setOnClick(void* func);
  void setOnHover(void* func);
  void setOnFocus(void* func);

 protected:
  // Position
  int x, y;

  // Dimensions of element
  int width, height;

  // Padding
  int padding_x, padding_y;

  // Text colour
  ALLEGRO_COLOR text_colour;

  // Background colour
  ALLEGRO_COLOR background_colour;

  // Fill with transparency
  bool transparent_cell_fill;

  // Primitive alpha
  float alpha;

  // Background visibility
  bool visible_background;

  // Thickness of border
  int border_thickness;

  // Visibility status
  bool visible;

  // Disabled status
  bool disabled;

  // Hover effect enabled
  bool hover_effect;

  // Image rotation
  float bitmap_rotation_angle;

  // Optional image
  ALLEGRO_BITMAP* image;

  // Font
  ALLEGRO_FONT* UIElement_font;

  // Text of element
  std::string text;

  // Justification of text
  int justification;

  // Unique id of element
  std::string id;

  // Focused
  bool focused;

  // Function pointers
  // On hover
  void(*onHover);

  // On focus
  void(*onFocus);

  // On click
  void(*onClick);
};

#endif  // UIELEMENT_H
