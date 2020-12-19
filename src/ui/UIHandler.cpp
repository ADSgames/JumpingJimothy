#include "UIHandler.h"

#include "../util/Config.h"

#include "Button.h"
#include "Label.h"

#include "../util/Globals.h"
#include "../util/Tools.h"

#include "../util/ActionBinder.h"
#include "../util/MouseListener.h"

// Create UI handler
UIHandler::UIHandler() {
  // Load cursor image
  ui_cursor = tools::load_bitmap_ex("images/cursor.png");

  // Focused element
  focusedElement = -1;
}

// Destroy UI handler
UIHandler::~UIHandler() {
  // Destory cursor
  // al_destroy_bitmap(ui_cursor);

  // Destroy elements of UI
  for (unsigned int i = 0; i < ui_elements.size(); i++) {
    delete ui_elements.at(i);
  }
  ui_elements.clear();
}

// Add element to handler
void UIHandler::addElement(UIElement* elem) {
  ui_elements.push_back(elem);
}

// Is hovering over ui element
bool UIHandler::isHovering() {
  // Check global hover status
  for (unsigned int i = 0; i < ui_elements.size(); i++) {
    if (ui_elements.at(i)->hover()) {
      return true;
    }
  }
  return false;
}

// Get all UI elements
std::vector<UIElement*> UIHandler::getUIElements() {
  return ui_elements;
}

// Create button
void UIHandler::createButton(const int x,
                             const int y,
                             std::string text,
                             std::string id,
                             ALLEGRO_FONT* font) {
  ui_elements.push_back(new Button(x, y, text, id, font));
}

// Create anchored button
void UIHandler::createAnchoredButton(const std::string& text,
                                     ALLEGRO_FONT* font,
                                     std::string anchorID,
                                     std::string id,
                                     const bool justification) {
  ui_elements.push_back(new Button(
      getElementById(anchorID)->getX() + getElementById(anchorID)->getWidth(),
      getElementById(anchorID)->getY(), text, id, font));
}

// Search for element by text
UIElement* UIHandler::getElementByText(const std::string& text) {
  // Find element
  for (unsigned int i = 0; i < ui_elements.size(); i++) {
    if (ui_elements.at(i)->getText() == text) {
      return ui_elements.at(i);
    }
  }
  tools::log_message("Warning: not found " + text);

  // Not found
  return nullptr;
}

// Search for elemnt by ID
UIElement* UIHandler::getElementById(const std::string& id) {
  // Find element
  for (unsigned int i = 0; i < ui_elements.size(); i++) {
    if (ui_elements.at(i)->getId() == id) {
      return ui_elements.at(i);
    }
  }
  tools::log_message("Warning: not found " + id);

  // Not found
  return nullptr;
}

// Draw UIElement to screen
void UIHandler::draw() {
  // Draw all elements
  for (unsigned int i = 0; i < ui_elements.size(); i++) {
    ui_elements.at(i)->draw();
  }

  // Draw cursor if required
  if (Config::getBooleanValue("draw_cursor") && ui_cursor) {
    al_draw_bitmap(ui_cursor, MouseListener::mouse_x, MouseListener::mouse_y,
                   0);
  }
}

// Update UIElement logic
void UIHandler::update() {
  // Update all elements
  for (unsigned int i = 0; i < ui_elements.size(); i++) {
    ui_elements.at(i)->update();
  }

  // Move between elements
  if (ui_elements.size() > 0) {
    // Key pressed
    if (ActionBinder::actionBegun(ACTION_UP) ||
        ActionBinder::actionBegun(ACTION_DOWN)) {
      // Unfocus current
      if (focusedElement >= 0 && focusedElement < (signed)ui_elements.size())
        ui_elements.at((focusedElement))->unfocus();

      // Focus direction -1 down, 0 none, 1 up
      int focusDirection = 0;

      // Choose direction
      if (ActionBinder::actionBegun(ACTION_UP)) {
        focusDirection = -1;
      } else if (ActionBinder::actionBegun(ACTION_DOWN)) {
        focusDirection = 1;
      }

      // Find next focusable
      for (unsigned int i = 0; i < ui_elements.size(); i++) {
        // Increment focused element
        focusedElement += focusDirection;

        // Keep in bounds
        if (focusedElement >= (signed)ui_elements.size())
          focusedElement = 0;
        else if (focusedElement < 0)
          focusedElement = (signed)ui_elements.size() - 1;

        // Check if focusable
        if (ui_elements.at((focusedElement))->canFocus() &&
            ui_elements.at((focusedElement))->isEnabled() &&
            ui_elements.at((focusedElement))->isVisible()) {
          ui_elements.at((focusedElement))->focus();
          break;
        }
      }
    }

    // Unfocus if mouse moved
    if (MouseListener::mouse_moved) {
      if (focusedElement >= 0 && focusedElement < (signed)ui_elements.size())
        ui_elements.at((focusedElement))->unfocus();
      focusedElement = -1;
    }
  }
}
