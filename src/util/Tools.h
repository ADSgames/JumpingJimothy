/**
 * TOOLS
 * Allan Legemaate
 * General purpose utilities used throughout
 * 05/05/2017
 **/

#ifndef TOOLS_H
#define TOOLS_H

#include <sstream>
#include <string>
#include <vector>

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

// Tools class
class tools {
 public:
  // Convert char to float
  static void abort_on_error(const std::string& message, std::string title);

  static ALLEGRO_BITMAP* load_bitmap_ex(const std::string& file);
  static ALLEGRO_SAMPLE* load_sample_ex(const std::string& file);

  static int get_text_offset_x(ALLEGRO_FONT* newFont, std::string newText);
  static int get_text_offset_y(ALLEGRO_FONT* newFont, std::string newText);
  static int get_text_width(ALLEGRO_FONT* newFont, std::string newText);
  static int get_text_height(ALLEGRO_FONT* newFont, std::string newText);

  // Clamp values
  template <class T>
  static T clamp(T min_val, T max_val, T value) {
    if (value < min_val)
      return min_val;
    if (value > max_val)
      return max_val;
    return value;
  }

  // Clamp values
  template <class T>
  static T negative_clamp_thing(T min_val, T max_val, T value) {
    if (value < min_val)
      return min_val + -1 * value;
    if (value > max_val)
      return max_val - value;
    return value;
  }

  // Function to check for collision
  template <class T>
  static bool collision(T xMin1,
                        T xMax1,
                        T xMin2,
                        T xMax2,
                        T yMin1,
                        T yMax1,
                        T yMin2,
                        T yMax2) {
    if (xMin1 < xMax2 && yMin1 < yMax2 && xMin2 < xMax1 && yMin2 < yMax1) {
      return true;
    }
    return false;
  }

  // Convert type to string
  template <class T>
  static std::string toString(const T& value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
  }

  // Convert types to string
  static int stringToInt(const std::string& str);
  static float stringToFloat(const std::string& str);

  // Split string
  static std::vector<std::string> split_string(const std::string& p_pcstStr,
                                               const char delim);

  // Random number
  static int random_int(const int min, const int max);

  // Debug message
  static void log_message(const std::string& message, bool debug = false);

  // Mouse over
  static bool mouse_over(const int x,
                         const int y,
                         const int width,
                         const int height);
  static bool mouse_clicked(const int button, bool reset = true);
};

#endif  // TOOLS_H
