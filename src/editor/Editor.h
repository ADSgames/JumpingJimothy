/**
 * Editor
 * Allan Legemaate and Danny Van Stemp
 * Level editor to create/edit levels
 * 05/05/2017
 **/

#ifndef EDITOR_H
#define EDITOR_H

#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>

#include <string>
#include <vector>

#include "../State.h"
#include "../ui/UIHandler.h"

// Nice editor boxes
struct editor_box {
  float x;
  float y;
  std::string x_str;
  std::string y_str;
  std::string type_str;
  int type;
  int orientation[4];
  int width;
  int height;
  bool affect_character;
  std::string width_str;
  std::string height_str;
};

// The editor state
class Editor : public State {
 public:
  Editor();
  virtual ~Editor();

  // Override parent
  virtual void update(StateEngine* engine) override;
  virtual void draw() override;

 private:
  UIHandler editorUI;
  // Images
  ALLEGRO_BITMAP* image_box[6];
  ALLEGRO_BITMAP* tiles[5][15];
  ALLEGRO_BITMAP* help_menu;
  ALLEGRO_BITMAP* cursor;

  // Functions
  void calculate_orientation_global();

  bool box_at(const int x, const int y);
  bool box_at_with_type(const int newType, const int x, const int y);

  bool is_player();

  bool save_map(const std::string& mapName);
  bool load_map(const std::string& mapName);

  static const int BUTTON_COUNT = 17;

  void set_explosive_ui_status();

  // Tiles
  std::vector<editor_box> editorBoxes;

  // Map name
  std::string file_name;

  bool gui_mode;
  float box_1_x;
  float box_1_y;

  float box_2_x;
  float box_2_y;

  bool is_dragging_box;
  bool dialog_open;

  // Remember saves
  bool is_saved;
  bool display_help;

  // Vars
  bool grid_on;

  int explosive_orientation;

  int tile_type;
  bool modified;

  // The font
  ALLEGRO_FONT* edit_font;
};

#endif  // EDITOR_H
