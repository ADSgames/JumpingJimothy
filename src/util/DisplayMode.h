/**
 * Display Mode
 * Danny Van Stemp and Allan Legemaate
 * Sets up and changes passed display
 * 20/11/2018
 **/

#ifndef DISPLAYMODE_H
#define DISPLAYMODE_H

#include <allegro5/display.h>
#include <string>

class DisplayMode {
 public:
  // Possible screen modes
  enum {
    FULLSCREEN_WINDOW_STRETCH,
    FULLSCREEN_WINDOW_LETTERBOX,
    FULLSCREEN_WINDOW_CENTER,
    WINDOWED,
    NUM_GRAPHICS_MODES
  };

  // Setters
  static void setMode(int);
  static void setActiveDisplay(ALLEGRO_DISPLAY** newdisplay);

  // Getters
  static int getDisplayMode();
  static std::string getDisplayModeString();
  static std::string getDisplayModeString(const int mode);

  static int getDrawWidth();
  static int getDrawHeight();

  static int getTranslationX();
  static int getTranslationY();

  static float getScaleX();
  static float getScaleY();

  static int getScaleWidth();
  static int getScaleHeight();

 private:
  // Size of drawing buffer
  static int draw_w;
  static int draw_h;

  // Size of window
  static int window_w;
  static int window_h;

  // Translation amount
  static int translation_x;
  static int translation_y;

  // Draw buffer scale percent (as decimal)
  static float scale_x;
  static float scale_y;

  // Active display
  static ALLEGRO_DISPLAY** display;

  // Private setters to reduce copy code
  static void setWindowSize(const int width, const int height);
  static void setScale(const float x, const float y);
  static void setTranslation(const int x, const int y);
};

#endif  // DISPLAYMODE_H
