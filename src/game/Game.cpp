#include "Game.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/bitmap.h>
#include <fstream>

#include "../util/ActionBinder.h"
#include "../util/KeyListener.h"
#include "../util/MouseListener.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wzero-as-null-pointer-constant"
#pragma GCC diagnostic ignored "-Wswitch-default"
#include "../rapidxml/rapidxml.hpp"
#include "../rapidxml/rapidxml_print.hpp"
#pragma GCC diagnostic pop

#include "CollisionBox.h"
#include "DynamicBox.h"
#include "Explosive.h"
#include "StaticBox.h"

#include "../util/Config.h"
#include "../util/Globals.h"
#include "../util/MusicManager.h"
#include "../util/Tools.h"

// Constructor
Game::Game() {
  // Set to null
  world = nullptr;
  gameCharacter = nullptr;
  gameGoat = nullptr;

  // Load fonts
  game_font = al_load_ttf_font("fonts/munro.ttf", 30, 0);
  help_font = al_load_ttf_font("fonts/munro.ttf", 50, 0);
  edit_font = al_load_ttf_font("fonts/fantasque.ttf", 18, 0);

  // Load sounds
  toggle_on.load_wav("sfx/toggle_on.wav");
  toggle_off.load_wav("sfx/toggle_off.wav");
  death.load_wav("sfx/death.wav");

  // Back button
  testing_back_button = nullptr;

  // Create back button if needed
  if (Config::getBooleanValue("EditingLevel")) {
    level = 0;
    testing_back_button = new Button(966, 728, "Back", "btnBack", edit_font);
  } else {
    level = Config::getIntValue("level_to_start");
  }

  // Load images
  load_sprites();

  // Reset fresh
  reset();

  // Load and play music
  MusicManager::menu_music->stop();
  MusicManager::game_music->play();
}

// Destructor
Game::~Game() {
  // Destory bitmaps
  // if (box != nullptr)
  //  al_destroy_bitmap( box);
  if (goat_map != nullptr)
    al_destroy_bitmap(goat_map);
  if (character != nullptr)
    al_destroy_bitmap(character);
  if (play != nullptr)
    al_destroy_bitmap(play);
  if (pause != nullptr)
    al_destroy_bitmap(pause);
}

// Creates box in world
Goat* Game::create_goat(float x, float y) {
  Goat* newGoat = new Goat(x, y, gameCharacter, goat_map, world->getB2World());
  if (gameCharacter == nullptr)
    tools::log_message(
        "WARNING: gameCharacter is nullptr when creating a goat.");
  gameBoxes.push_back(newGoat);
  return newGoat;
}

// Creates box in world
Box* Game::create_dynamic_box(float x,
                              float y,
                              float velX,
                              float velY,
                              ALLEGRO_BITMAP* image) {
  DynamicBox* newDynamicBox =
      new DynamicBox(x, y, velX, velY, world->getB2World());
  newDynamicBox->setImage(image);
  gameBoxes.push_back(newDynamicBox);
  return newDynamicBox;
}

// Creates box in world
Box* Game::create_static_box(float x, float y, ALLEGRO_BITMAP* image) {
  StaticBox* newStaticBox = new StaticBox(x, y);
  newStaticBox->setImage(image);
  gameBoxes.push_back(newStaticBox);
  return newStaticBox;
}

Box* Game::create_collision_box(float x, float y, float width, float height) {
  CollisionBox* newCollisionBox =
      new CollisionBox(x, y, width, height, world->getB2World());
  gameBoxes.push_back(newCollisionBox);
  return newCollisionBox;
}

Box* Game::create_explosive_box(float x,
                                float y,
                                int orientation,
                                bool affectsCharacter) {
  Explosive* newExplosive =
      new Explosive(x, y, affectsCharacter, gameCharacter, world->getB2World());
  ALLEGRO_BITMAP* newBoxImage;

  if (orientation == 0)
    newBoxImage = box_repel;
  else
    newBoxImage = box_repel_direction;

  newExplosive->setImage(newBoxImage);
  newExplosive->setOrientation(orientation);

  if (gameCharacter == nullptr)
    tools::log_message(
        "WARNING: gameCharacter is nullptr when creating an explosive box.");

  gameBoxes.push_back(newExplosive);
  return newExplosive;
}

// Add character to world
Character* Game::create_character(float x, float y) {
  Character* newCharacter = new Character(x, y, character, world->getB2World());
  gameBoxes.push_back(newCharacter);
  return newCharacter;
}

// Load world from xml
void Game::load_world(std::string file) {
  // Load level message
  tools::log_message("Attempting to load " + file + " into game");

  int goat_count = 0;
  int character_count = 0;
  int dynamic_count = 0;
  int static_count = 0;

  // Doc
  rapidxml::xml_document<> doc;
  rapidxml::xml_node<>* root_node;

  // Make an xml object
  std::ifstream theFile(file);
  std::vector<char> xml_buffer((std::istreambuf_iterator<char>(theFile)),
                               std::istreambuf_iterator<char>());
  xml_buffer.push_back('\0');

  // Parse the buffer using the xml file parsing library into doc
  doc.parse<0>(&xml_buffer[0]);

  // Find our root node
  root_node = doc.first_node("data");

  // Make sure node exists
  if (root_node != 0) {
    // Iteratboole over objects
    for (rapidxml::xml_node<>* object_node = root_node->first_node("Object");
         object_node; object_node = object_node->next_sibling()) {
      // All the variables we will load
      std::string type = "Static";
      std::string x = "0";
      std::string y = "0";
      std::string orientation = "0";
      // std::string bodytype = "Static";
      std::string width = "0";
      std::string height = "0";
      std::string affect_character = "false";
      int orientation_array[4];
      Box* newBox = nullptr;

      // Load data
      if (object_node->first_attribute("type") != 0)
        type = object_node->first_attribute("type")->value();
      if (object_node->first_node("x") != 0)
        x = object_node->first_node("x")->value();
      if (object_node->first_node("y") != 0)
        y = object_node->first_node("y")->value();
      if (object_node->first_node("width") != 0)
        width = object_node->first_node("width")->value();
      if (object_node->first_node("height") != 0)
        height = object_node->first_node("height")->value();
      // if( object_node -> first_node("bodytype") != 0)
      //   bodytype = object_node -> first_node("bodytype") -> value();
      if (object_node->first_node("orientation") != 0)
        orientation = object_node->first_node("orientation")->value();
      if (object_node->first_node("affect_character") != 0)
        affect_character = object_node->first_node("affect_character")->value();

      if (type == "Static") {
        std::vector<std::string> splits = tools::split_string(orientation, ' ');
        if (splits.size() == 4) {
          for (int k = 0; k < 4; k++) {
            orientation_array[k] = (tools::stringToInt(splits.at(k)));
          }
        }

        // Combine images
        const int tmp_width = al_get_bitmap_width(new_dynamic_tile[0]) * 2;
        const int tmp_height = al_get_bitmap_height(new_dynamic_tile[0]) * 2;

        ALLEGRO_BITMAP* combined_image =
            al_create_bitmap(tmp_width, tmp_height);
        al_set_target_bitmap(combined_image);

        // Offsets from subtile
        for (int t = 0; t < 4; t++) {
          const int off_x = (t == 1 || t == 3) ? 16 : 0;
          const int off_y = (t >= 2) ? 16 : 0;

          al_draw_bitmap(new_dynamic_tile[orientation_array[t]], off_x, off_y,
                         0);
        }

        newBox = create_static_box(tools::stringToFloat(x),
                                   tools::stringToFloat(y), combined_image);

        static_count++;
      } else if (type == "Dynamic") {
        newBox = create_dynamic_box(
            tools::stringToFloat(x), tools::stringToFloat(y),
            tools::stringToFloat(width), tools::stringToFloat(height), box);
        dynamic_count++;
      } else if (type == "Collision") {
        newBox = create_collision_box(
            tools::stringToFloat(x), tools::stringToFloat(y),
            tools::stringToFloat(width), tools::stringToFloat(height));
      } else if (type == "Character") {
        gameCharacter = create_character(tools::stringToFloat(x),
                                         tools::stringToFloat(y) - 1.6);
        character_count++;
      } else if (type == "Finish") {
        gameGoat =
            create_goat(tools::stringToFloat(x), tools::stringToFloat(y) - 1);
        if (gameCharacter == nullptr)
          tools::log_message(
              "WARNING: Game: goat is passed nullptr gameCharacter");
        goat_count++;
      } else if (type == "Explosive") {
        newBox = create_explosive_box(
            tools::stringToFloat(x), tools::stringToFloat(y),
            tools::stringToFloat(orientation), (affect_character == "true"));
      }
    }

    // Get game help text
    rapidxml::xml_node<>* help_node = root_node->first_node("Help");
    help_text.clear();

    if (help_node) {
      std::vector<std::string> temp_help =
          tools::split_string(help_node->value(), '%');
      for (unsigned int i = 0; i < temp_help.size(); i++) {
        help_text.push_back(temp_help.at(i));
      }
    }
  }

  // Nice debug code
  // Allan's big ol' debug lines, they're pretty tho
  tools::log_message("===============================");
  // std::cout << "Level loaded: " << static_count << " static, " <<
  // dynamic_count << " dynamic, " << character_count << " character(s), " <<
  // goat_count << " goat(s)\n";
  if (character_count > 1)
    tools::log_message(
        "  WARNING: Multiple characters loaded, will have undesired "
        "results...");
  if (goat_count > 1)
    tools::log_message(
        "  WARNING: Multiple goats loaded, will have undesired results...");
  if (character_count == 0)
    tools::log_message(
        "  WARNING: No character loaded, will have undesired results...");
  if (goat_count == 0)
    tools::log_message(
        "  WARNING: No goat loaded, will have undesired results...");
  if (static_count == 0 && dynamic_count == 0 && goat_count == 0 &&
      character_count == 0)
    tools::log_message("  WARNING: No data loaded!");
  tools::log_message("===============================");
}

// Reset game
void Game::reset() {
  // Remove boxes, including character and goatr
  for (unsigned int i = 0; i < gameBoxes.size(); i++) {
    delete gameBoxes.at(i);
  }
  gameBoxes.clear();

  gameGoat = nullptr;
  gameCharacter = nullptr;

  // Box2D game world
  if (world != nullptr) {
    delete world;
  }

  // Create world
  world = new World(b2Vec2(0.0f, -10.0f), 1.0f / 60.0f, 6, 2, true);

  // Load file
  if (Config::getBooleanValue("EditingLevel")) {
    load_world(Config::getValue("EditingLevelFile"));
  } else {
    load_world("data/level_" + tools::toString(level) + ".xml");
  }

  static_mode = true;
  first_play = true;

  // Pause boxes
  for (unsigned int i = 0; i < gameBoxes.size(); i++) {
    if (gameBoxes[i]->getType() == BOX) {
      gameBoxes[i]->setPaused(true);
    }
  }
  if (gameGoat == nullptr)
    tools::log_message("WARNING: Goat is undeclared in game");

  if (gameCharacter == nullptr)
    tools::log_message("WARNING: gameCharacter is undeclared in game");
}

// Load all sprites for in game
void Game::load_sprites() {
  box = tools::load_bitmap_ex("images/box.png");
  box_repel = tools::load_bitmap_ex("images/box_repel.png");
  box_repel_direction = tools::load_bitmap_ex("images/box_repel_direction.png");
  goat_map = tools::load_bitmap_ex("images/goat.png");
  character = tools::load_bitmap_ex("images/character.png");
  play = tools::load_bitmap_ex("images/play.png");
  pause = tools::load_bitmap_ex("images/pause.png");

  ALLEGRO_BITMAP* image_box = tools::load_bitmap_ex("images/StaticBlock.png");

  for (int i = 0; i < 3; i++)
    for (int t = 0; t < 5; t++)
      new_dynamic_tile[i + t * 3] =
          al_create_sub_bitmap(image_box, i * 16, t * 16, 16, 16);
}

// Update game logic
void Game::update(StateEngine* engine) {
  // Back button
  if (testing_back_button) {
    testing_back_button->update();

    if (testing_back_button->clicked())
      setNextState(engine, StateEngine::STATE_EDIT);
  }

  // Game mode
  if (KeyListener::keyPressed[ALLEGRO_KEY_P])
    setNextState(engine, StateEngine::STATE_EDIT);

  // Change state?
  if (KeyListener::key[ALLEGRO_KEY_I])
    setNextState(engine, StateEngine::STATE_MENU);

  // Touching goat
  if (gameGoat != nullptr && gameGoat->getWinCondition()) {
    level++;
    if (level > 14) {
      setNextState(engine, StateEngine::STATE_MENU);
    } else {
      tools::log_message("Level " + tools::toString(level - 1) +
                         " completed, loading next level.");
      Config::setValue("level_" + tools::toString(level - 1) + "_completed",
                       true);

      if (!Config::getBooleanValue("EditingLevel")) {
        reset();
      } else {
        al_show_native_message_box(nullptr, "Level complete!", "Opening editor",
                                   "", "Okily dokily.", 0);
        setNextState(engine, StateEngine::STATE_EDIT);
      }
    }
  }

  // Update the Box2D game world
  world->step();

  // Update character
  for (unsigned int i = 0; i < gameBoxes.size(); i++)
    gameBoxes[i]->update(world->getB2World());

  if (KeyListener::keyPressed[ALLEGRO_KEY_ESCAPE]) {
    setNextState(engine, StateEngine::STATE_MENU);
    MusicManager::game_music->stop();
  }

  // Next level
  if (KeyListener::keyPressed[ALLEGRO_KEY_C]) {
    if (!Config::getBooleanValue("EditingLevel")) {
      tools::log_message("Level " + tools::toString(level) + " skipped.");
      level++;
      reset();
    } else {
      al_show_native_message_box(nullptr, "Level complete!", "Opening editor",
                                 nullptr, nullptr, 0);
      setNextState(engine, StateEngine::STATE_EDIT);
    }
  }

  // Previous level
  if (KeyListener::keyPressed[ALLEGRO_KEY_X]) {
    tools::log_message("Level " + tools::toString(level) + " skipped back.");
    if (level > 1)
      level--;
    reset();
  }

  // Pause/Play time
  if (ActionBinder::actionBegun(ACTION_B)) {
    static_mode = !static_mode;

    for (unsigned int i = 0; i < gameBoxes.size(); i++) {
      if (gameBoxes[i]->isPausable()) {
        gameBoxes[i]->setPaused(static_mode);
      }
    }

    if (static_mode) {
      toggle_off.play();
    } else {
      toggle_on.play();
      first_play = false;
    }
  }

  // Off screen
  // Brutal hack don't look. Prevens character from dying when the character's
  // location is undefined or something. It's really big anyways
  if (gameCharacter != nullptr && gameCharacter->getX() > -5000 &&
      gameCharacter->getX() < 5000 && gameCharacter->getY() > -5000 &&
      gameCharacter->getY() < 5000) {
    if (gameCharacter->getX() < -1 || gameCharacter->getX() > 51.5f ||
        gameCharacter->getY() > 2 || gameCharacter->getY() < -40) {
      death.play();
      reset();
      tools::log_message("===========" +
                         tools::toString(gameCharacter->getX()));
    }
  }

  // Die
  if (KeyListener::keyPressed[ALLEGRO_KEY_R]) {
    death.play();
    reset();
  }
}

// Draw to screen
void Game::draw() {
  // Background
  al_clear_to_color(al_map_rgb(40, 40, 60));

  // Help text
  for (unsigned int i = 0; i < help_text.size(); i++) {
    al_draw_text(help_font, al_map_rgb(255, 255, 255), 500, 75 + i * 50, 1,
                 help_text.at(i).c_str());
  }

  // Draw boxes
  for (unsigned int i = 0; i < gameBoxes.size(); i++) {
    gameBoxes[i]->draw();
  }

  // Pause/play buttons
  if (testing_back_button) {
    testing_back_button->draw();
  }

  if (static_mode) {
    al_draw_bitmap(pause, 10, 10, 0);
  } else {
    al_draw_bitmap(play, 10, 10, 0);
  }

  al_draw_textf(game_font, al_map_rgb(255, 255, 255), 1010, 15, 2, "Level %i",
                level);
}
