/**
 * Action Binder
 * Danny Van Stemp and Allan Legemaate
 * Abstraction layer on top of key/joy
 *   codes for keybindings
 * 05/05/2017
 **/

#ifndef ACTIONBINDER_H
#define ACTIONBINDER_H

#include <vector>

#define BINDING_NONE -1

enum actions {
  ACTION_NONE,
  ACTION_LEFT,
  ACTION_RIGHT,
  ACTION_UP,
  ACTION_DOWN,
  ACTION_A,
  ACTION_B,
  ACTION_SELECT
};

enum types { TYPE_KEY, TYPE_JOY_STICK, TYPE_JOY_BUTTON };

class Binding {
 public:
  Binding(int action, int type, int code) {
    this->action = action;
    this->type = type;
    this->code = code;
  }

  Binding() : Binding(BINDING_NONE, BINDING_NONE, BINDING_NONE) {}

  int getType() { return type; }
  int getCode() { return code; }
  int getAction() { return action; }

 private:
  int type;
  int code;
  int action;
};

class ActionBinder {
 public:
  static bool actionBegun(const int action);
  static bool actionHeld(const int action);

  static void addBinding(int action, int type, int code);

  static void setDefaults();

 private:
  static std::vector<Binding*> bindings;
  static std::vector<Binding*> findBindings(const int action);
};

#endif  // ACTIONBINDER_H
