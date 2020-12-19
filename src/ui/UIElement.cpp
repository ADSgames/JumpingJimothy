#include "UIElement.h"

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>

#include "../util/KeyListener.h"
#include "../util/MouseListener.h"
#include "../util/Tools.h"

#include "../util/ActionBinder.h"

// Defaults
UIElement::UIElement() {
  this->alpha = 255;
  this->background_colour = al_map_rgba(200, 200, 200, alpha);
  this->text_colour = al_map_rgba(0, 0, 0, alpha);
  this->x = 0;
  this->y = 0;
  this->width = 10;
  this->height = 10;
  this->text = "";
  this->UIElement_font = nullptr;
  this->visible = true;
  this->disabled = false;
  this->image = nullptr;
  this->padding_x = 10;
  this->padding_y = 10;
  this->id = "";
  this->visible_background = true;
  this->justification = 0;
  this->border_thickness = 2;
  this->transparent_cell_fill = false;
  this->hover_effect = true;
  this->bitmap_rotation_angle = 0;
  this->focused = false;

  this->onClick = nullptr;
  this->onHover = nullptr;
  this->onFocus = nullptr;
}

UIElement::UIElement(const int x,
                     const int y,
                     std::string text,
                     std::string id,
                     ALLEGRO_FONT* font)
    : UIElement() {
  setId(id);
  setX(x);
  setY(y);
  setText(text);
  setFont(font);
}

// Destruct
UIElement::~UIElement() {
  al_destroy_bitmap(image);
}

// Get X
int UIElement::getX() {
  return x;
}

// Get y
int UIElement::getY() {
  return y;
}

// Get text of element
std::string UIElement::getText() {
  return text;
}

// Get element ID
std::string UIElement::getId() {
  return id;
}

// Hide
void UIElement::hide() {
  this->visible = false;
  this->disabled = true;
}

// Show
void UIElement::show() {
  this->visible = true;
  this->disabled = false;
}

// Is visible or not
bool UIElement::isVisible() {
  return visible;
}

// Set visiblity
void UIElement::setVisibility(bool visible) {
  this->visible = visible;
  this->disabled = !visible;
}

// Disable element
void UIElement::disable() {
  disabled = true;
}

// Enable element
void UIElement::enable() {
  disabled = false;
}

// Is enabled or not
bool UIElement::isEnabled() {
  return !disabled;
}

// Set transparency level
void UIElement::setTransparency(const float alpha) {
  this->alpha = alpha;
  text_colour = al_map_rgba(text_colour.r, text_colour.g, text_colour.b, alpha);
  background_colour.a = alpha;
}

// Set image rotation
void UIElement::setBitmapRotationAngle(const float rotation) {
  bitmap_rotation_angle = rotation;
}

// Set x
void UIElement::setX(const int x) {
  this->x = x;
}

// Set y
void UIElement::setY(const int y) {
  this->y = y;
}

// Set text colour
void UIElement::setTextColour(ALLEGRO_COLOR colour) {
  text_colour = colour;
}

// Set background colour
void UIElement::setBackgroundColour(ALLEGRO_COLOR colour) {
  background_colour = colour;
}

// Set cell fill
void UIElement::setCellFillTransparent(const bool n) {
  transparent_cell_fill = n;
}

// Set text justification
void UIElement::setTextJustification(const int justification) {
  this->justification = justification;
}

// Element width
int UIElement::getWidth() {
  return width + padding_x * 2;
}

// Element height
int UIElement::getHeight() {
  return height + padding_y * 2;
}

// Set padding
void UIElement::setPadding(const int x, const int y) {
  padding_x = x;
  padding_y = y;
}

// Set position
void UIElement::setPosition(const int x, const int y) {
  setX(x);
  setY(y);
}

// Set element size
void UIElement::setSize(const int width, const int height) {
  setWidth(width);
  setHeight(height);
}

// Set text
void UIElement::setText(const std::string& text) {
  this->text = text;
}

// Set id
void UIElement::setId(const std::string& id) {
  this->id = id;
}

// Sets an image
void UIElement::setImage(ALLEGRO_BITMAP* image) {
  if (image == nullptr)
    return;

  // Clone image
  this->image = al_clone_bitmap(image);
  this->width = al_get_bitmap_width(this->image);
  this->height = al_get_bitmap_height(this->image);
}

// Set new font
void UIElement::setFont(ALLEGRO_FONT* font) {
  if (font == nullptr)
    return;
  this->UIElement_font = font;
  this->width = al_get_text_width(UIElement_font, text.c_str());
  this->height = al_get_font_line_height(UIElement_font);
}

// Set background visibility
void UIElement::setVisibleBackground(const bool b) {
  visible_background = b;
}

// Set width
void UIElement::setWidth(const int width) {
  this->width = width;
}

// Set height
void UIElement::setHeight(const int height) {
  this->height = height;
}

// This element can focus
bool UIElement::canFocus() {
  return false;
}

// Focus
void UIElement::focus() {
  this->focused = true;
}

// Unfocus
void UIElement::unfocus() {
  this->focused = false;
}

// Set border thickness
void UIElement::setBorderThickness(const int thickness) {
  border_thickness = thickness;
}

// Disable hover effect
void UIElement::disableHoverEffect() {
  hover_effect = false;
}

// Disable hover effect
void UIElement::enableHoverEffect() {
  hover_effect = true;
}

// True if hovering
bool UIElement::hover() {
  return tools::mouse_over(getX(), getY(), getWidth(), getHeight());
}

// True if clicked
bool UIElement::clicked() {
  return !disabled &&
         ((hover() && tools::mouse_clicked(MouseListener::MOUSE_LEFT)) ||
          (focused && (ActionBinder::actionBegun(ACTION_SELECT))));
}

// Set callbacks
void UIElement::setOnClick(void* func) {
  onClick = func;
}

void UIElement::setOnHover(void* func) {
  onHover = func;
}

void UIElement::setOnFocus(void* func) {
  onFocus = func;
}
