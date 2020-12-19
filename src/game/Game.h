/**
 * Game
 * Allan Legemaate and Danny Van Stemp
 * Game state
 * 05/05/2017
 **/

#ifndef GAME_H
#define GAME_H

#include <Box2D/Box2D.h>
#include <vector>

#include "../State.h"

#include "../ui/Button.h"
#include "../util/Sound.h"
#include "Box.h"
#include "Character.h"
#include "Goat.h"

#include "World.h"

class Game : public State {
 public:
  // Construct / destruct
  Game();
  ~Game();

  // Override parent
  virtual void update(StateEngine* engine) override;
  virtual void draw() override;
  bool level_complete();

 private:
  // Functions
  void load_world(const std::string& file);
  void load_sprites();
  void reset();

  // Creation code
  Box* create_dynamic_box(const float x,
                          const float y,
                          const float velX,
                          const float velY,
                          ALLEGRO_BITMAP*);

  Box* create_explosive_box(const float x,
                            const float y,
                            int orientation,
                            bool affectsCharacter);

  Box* create_static_box(const float x, const float y, ALLEGRO_BITMAP*);

  Box* create_collision_box(const float x,
                            const float y,
                            const float,
                            const float);

  Goat* create_goat(const float x, const float y);
  Character* create_character(const float x, const float y);

  // Our character and goat
  Goat* gameGoat;
  Character* gameCharacter;

  // Game variables
  std::vector<Box*> gameBoxes;

  int level;
  bool first_play;
  bool static_mode;

  std::vector<std::string> help_text;

  ALLEGRO_FONT* game_font;
  ALLEGRO_FONT* help_font;
  ALLEGRO_FONT* edit_font;

  // Bitmaps
  ALLEGRO_BITMAP* box;
  ALLEGRO_BITMAP* box_repel;
  ALLEGRO_BITMAP* box_repel_direction;
  ALLEGRO_BITMAP* character;
  ALLEGRO_BITMAP* goat_map;
  ALLEGRO_BITMAP* play;
  ALLEGRO_BITMAP* pause;

  ALLEGRO_BITMAP* new_dynamic_tile[100];

  // TODO (Danny#1#): FIX WHEN NOT DEAD INSIDE

  // Samples
  Sound toggle_off;
  Sound toggle_on;
  Sound death;
  Button* testing_back_button;

  // Box 2d game world
  World* world;
};

#endif  // GAME_H
