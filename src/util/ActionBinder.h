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

enum BINDING_ACTION {
  ACTION_NONE,
  ACTION_LEFT,
  ACTION_RIGHT,
  ACTION_UP,
  ACTION_DOWN,
  ACTION_A,
  ACTION_B,
  ACTION_SELECT
};

enum BINDING_TYPE { TYPE_KEY, TYPE_JOY_STICK, TYPE_JOY_BUTTON };

class Binding {
 public:
  Binding(const BINDING_ACTION action, const BINDING_TYPE type, const int code)
      : action(action), type(type), code(code) {}

  Binding() : Binding(ACTION_NONE, TYPE_KEY, -1) {}

  BINDING_ACTION getAction() { return action; }
  BINDING_TYPE getType() { return type; }
  int getCode() { return code; }

 private:
  BINDING_ACTION action;
  BINDING_TYPE type;
  int code;
};

class ActionBinder {
 public:
  static bool actionBegun(const BINDING_ACTION action);
  static bool actionHeld(const BINDING_ACTION action);

  static void addBinding(const BINDING_ACTION action,
                         const BINDING_TYPE type,
                         const int code);

  static void setDefaults();

 private:
  static std::vector<Binding*> bindings;
  static std::vector<Binding*> findBindings(const BINDING_ACTION action);
};

#endif  // ACTIONBINDER_H
