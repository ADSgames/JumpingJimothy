#include "ActionBinder.h"

#include <allegro5/keycodes.h>

#include "JoystickListener.h"
#include "KeyListener.h"

std::vector<Binding*> ActionBinder::bindings;

bool ActionBinder::actionBegun(const BINDING_ACTION action) {
  std::vector<Binding*> foundBindings = findBindings(action);

  for (unsigned int i = 0; i < foundBindings.size(); i++) {
    if (foundBindings.at(i)->getAction() != ACTION_NONE) {
      switch (foundBindings.at(i)->getType()) {
        case TYPE_KEY:
          if (KeyListener::keyPressed[foundBindings.at(i)->getCode()]) {
            return true;
          }
          break;
        case TYPE_JOY_BUTTON:
          if (JoystickListener::buttonPressed[foundBindings.at(i)->getCode()]) {
            return true;
          }
          break;
        case TYPE_JOY_STICK:
          if (JoystickListener::stickMoved[foundBindings.at(i)->getCode()]) {
            return true;
          }
          break;
      }
    }
  }
  return false;
}

bool ActionBinder::actionHeld(const BINDING_ACTION action) {
  std::vector<Binding*> foundBindings = findBindings(action);

  for (unsigned int i = 0; i < foundBindings.size(); i++) {
    if (foundBindings.at(i)->getAction() != ACTION_NONE) {
      switch (foundBindings.at(i)->getType()) {
        case TYPE_KEY:
          if (KeyListener::key[foundBindings.at(i)->getCode()]) {
            return true;
          }
          break;
        case TYPE_JOY_BUTTON:
          if (JoystickListener::button[foundBindings.at(i)->getCode()]) {
            return true;
          }
          break;
        case TYPE_JOY_STICK:
          if (JoystickListener::stick[foundBindings.at(i)->getCode()]) {
            return true;
          }
          break;
      }
    }
  }
  return false;
}

std::vector<Binding*> ActionBinder::findBindings(const BINDING_ACTION action) {
  // Empty binding vector
  std::vector<Binding*> foundBindings;

  // Find
  for (unsigned int i = 0; i < ActionBinder::bindings.size(); i++) {
    if (ActionBinder::bindings.at(i)->getAction() == action) {
      foundBindings.push_back(ActionBinder::bindings.at(i));
    }
  }

  // Return vector
  return foundBindings;
}

// Add binding
void ActionBinder::addBinding(const BINDING_ACTION action,
                              const BINDING_TYPE type,
                              const int code) {
  ActionBinder::bindings.push_back(new Binding(action, type, code));
}

void ActionBinder::setDefaults() {
  // Left
  addBinding(ACTION_LEFT, TYPE_KEY, ALLEGRO_KEY_A);
  addBinding(ACTION_LEFT, TYPE_KEY, ALLEGRO_KEY_LEFT);
  addBinding(ACTION_LEFT, TYPE_JOY_STICK, LEFT_STICK_LEFT);
  addBinding(ACTION_LEFT, TYPE_JOY_BUTTON, JOY_XBOX_PAD_LEFT);

  // Right
  addBinding(ACTION_RIGHT, TYPE_KEY, ALLEGRO_KEY_D);
  addBinding(ACTION_RIGHT, TYPE_KEY, ALLEGRO_KEY_RIGHT);
  addBinding(ACTION_RIGHT, TYPE_JOY_STICK, LEFT_STICK_RIGHT);
  addBinding(ACTION_RIGHT, TYPE_JOY_BUTTON, JOY_XBOX_PAD_RIGHT);

  // Up
  addBinding(ACTION_UP, TYPE_KEY, ALLEGRO_KEY_UP);
  addBinding(ACTION_UP, TYPE_KEY, ALLEGRO_KEY_W);
  addBinding(ACTION_UP, TYPE_JOY_STICK, LEFT_STICK_UP);
  addBinding(ACTION_UP, TYPE_JOY_BUTTON, JOY_XBOX_PAD_UP);

  // Down
  addBinding(ACTION_DOWN, TYPE_KEY, ALLEGRO_KEY_DOWN);
  addBinding(ACTION_DOWN, TYPE_KEY, ALLEGRO_KEY_S);
  addBinding(ACTION_DOWN, TYPE_JOY_STICK, LEFT_STICK_DOWN);
  addBinding(ACTION_DOWN, TYPE_JOY_BUTTON, JOY_XBOX_PAD_DOWN);

  // Action A
  addBinding(ACTION_A, TYPE_KEY, ALLEGRO_KEY_W);
  addBinding(ACTION_A, TYPE_KEY, ALLEGRO_KEY_UP);
  addBinding(ACTION_A, TYPE_JOY_BUTTON, JOY_XBOX_A);

  // Action B
  addBinding(ACTION_B, TYPE_KEY, ALLEGRO_KEY_SPACE);
  addBinding(ACTION_B, TYPE_JOY_BUTTON, JOY_XBOX_B);

  // Select
  addBinding(ACTION_SELECT, TYPE_KEY, ALLEGRO_KEY_ENTER);
  addBinding(ACTION_SELECT, TYPE_JOY_BUTTON, JOY_XBOX_A);
  addBinding(ACTION_SELECT, TYPE_JOY_BUTTON, JOY_XBOX_START);
}
