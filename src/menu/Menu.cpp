#include "Menu.h"

#include <allegro5/allegro.h>
#include <allegro5/allegro_ttf.h>

#include "../util/Config.h"
#include "../util/MusicManager.h"
#include "../util/Tools.h"

#include "../util/KeyListener.h"

// Init menu
Menu::Menu() {
  credits_menu = false;

  int button_offset_x = 40;

  title = tools::load_bitmap_ex("images/title_static.png");
  title_overlay = tools::load_bitmap_ex("images/title_overlay.png");
  title_shine = tools::load_bitmap_ex("images/title_shine.png");
  logo = tools::load_bitmap_ex("images/logo.png");

  menu_font = al_load_ttf_font("fonts/munro.ttf", 18, 0);
  button_font = al_load_ttf_font("fonts/munro.ttf", 24, 0);
  credits_font = al_load_ttf_font("fonts/munro.ttf", 32, 0);

  menu_ui.addElement(
      new Button(button_offset_x, 500, "Play", "btnPlay", button_font));
  menu_ui.getElementById("btnPlay")->setSize(179, 20);
  // menu_buttons[menu_button_play].setVisibleBackground(false);
  // menu_buttons[menu_button_play].setTextColour(al_map_rgb(255,255,255));

  menu_ui.addElement(new Button(button_offset_x, 550, "Level Editor",
                                "btnEditor", button_font));
  menu_ui.getElementById("btnEditor")->setSize(179, 20);
  // menu_buttons[menu_button_edit].setVisibleBackground(false);
  // menu_buttons[menu_button_edit].setTextColour(al_map_rgb(255,255,255));

  menu_ui.addElement(
      new Button(button_offset_x, 600, "Settings", "btnSettings", button_font));
  menu_ui.getElementById("btnSettings")->setSize(179, 20);
  // menu_buttons[menu_button_options].setVisibleBackground(false);
  // menu_buttons[menu_button_options].setTextColour(al_map_rgb(255,255,255));

  menu_ui.addElement(
      new Button(button_offset_x, 650, "Credits", "btnCredits", button_font));
  menu_ui.getElementById("btnCredits")->setSize(179, 20);
  // menu_buttons[menu_button_help].setVisibleBackground(false);
  // menu_buttons[menu_button_help].setTextColour(al_map_rgb(255,255,255));

  menu_ui.addElement(
      new Button(button_offset_x, 700, "Exit", "btnExit", button_font));
  menu_ui.getElementById("btnExit")->setSize(179, 20);
  // menu_buttons[menu_button_exit].setVisibleBackground(false);
  // menu_buttons[menu_button_exit].setTextColour(al_map_rgb(255,255,255));

  if (!MusicManager::menu_music->getIsPlaying()) {
    MusicManager::menu_music->play();
  }

  counter_title = 0;
}

// Destory menu
Menu::~Menu() {
  // Destroy images
  al_destroy_bitmap(title);
  al_destroy_bitmap(title_overlay);
  al_destroy_bitmap(title_shine);
  al_destroy_bitmap(logo);

  // Destroy fonts
  al_destroy_font(menu_font);
  al_destroy_font(button_font);
  al_destroy_font(credits_font);
}

// Update animation and wait for input
void Menu::update(StateEngine* engine) {
  // Update UI
  menu_ui.update();

  // Main menu
  if (!credits_menu) {
    // Click play
    if (menu_ui.getElementById("btnPlay")->clicked()) {
      setNextState(engine, StateEngine::STATE_LEVELSELECT);
    }

    // Click editor
    if (menu_ui.getElementById("btnEditor")->clicked()) {
      setNextState(engine, StateEngine::STATE_EDIT);
    }

    // Click editor
    if (menu_ui.getElementById("btnExit")->clicked()) {
      setNextState(engine, StateEngine::STATE_EXIT);
    }

    // Click settings
    if (menu_ui.getElementById("btnSettings")->clicked()) {
      setNextState(engine, StateEngine::STATE_OPTIONS);
    }

    // Click credits
    if (menu_ui.getElementById("btnCredits")->clicked()) {
      credits_menu = true;
    }
  }

  // Exit credits
  // TODO replace with less hacked version
  if (KeyListener::anyKeyPressed) {
    credits_menu = false;
  }

  // Counter for title animation
  counter_title = (counter_title + 1) % 300;
}

// Draw images to screen
void Menu::draw() {
  // Background
  al_clear_to_color(al_map_rgb(50, 50, 50));

  // Menu updates
  if (!credits_menu) {
    // Offsets for title
    const int title_offset_x = 150;
    const int title_offset_y = 50;

    // Title
    al_draw_scaled_bitmap(title, 0, 0, 175, 160, 150 + title_offset_x,
                          50 + title_offset_y, 612, 560, 0);

    // Title shine
    if (counter_title < 50)
      al_draw_scaled_bitmap(title_shine, 0, 0, 60, 150,
                            150 + title_offset_x + counter_title * 10,
                            50 + title_offset_y, 210, 525, 0);

    // Title top layer
    al_draw_scaled_bitmap(title_overlay, 0, 0, 200, 160, 150 + title_offset_x,
                          50 + title_offset_y, 700, 560, 0);

    // Top right info text
    al_draw_textf(menu_font, al_map_rgb(255, 255, 255), 1010, 15, 2,
                  "TOJam 12, 2017");
    al_draw_textf(menu_font, al_map_rgb(255, 255, 255), 1010, 35, 2,
                  "Danny Van Stemp");
    al_draw_textf(menu_font, al_map_rgb(255, 255, 255), 1010, 55, 2,
                  "Allan Legemaate");
    al_draw_textf(menu_font, al_map_rgb(255, 255, 255), 1010, 75, 2,
                  "Sullivan Stobo");
    al_draw_textf(menu_font, al_map_rgb(255, 255, 255), 1010, 95, 2,
                  "Max Keleher");

    // Draw buttons
    menu_ui.draw();
  }

  // Credits menu
  else {
    const int padding = 50;
    const int x_location = 395;
    al_draw_bitmap(logo, 730, 50, 0);
    al_draw_textf(credits_font, al_map_rgb(255, 255, 255), x_location,
                  40 + padding, 1, "Written in C++ using Code::Blocks");
    al_draw_textf(credits_font, al_map_rgb(255, 255, 255), x_location,
                  80 + padding, 1, "Allegro 5 for graphics");
    al_draw_textf(credits_font, al_map_rgb(255, 255, 255), x_location,
                  120 + padding, 1, "Box2D for physics");
    al_draw_textf(credits_font, al_map_rgb(255, 255, 255), x_location,
                  160 + padding, 1, "RapidXml for level loading/saving");
    al_draw_textf(credits_font, al_map_rgb(255, 255, 255), x_location,
                  240 + padding, 1, "Music/code by Allan Legemaate");
    al_draw_textf(credits_font, al_map_rgb(255, 255, 255), x_location,
                  280 + padding, 1, "Art/game design by Sullivan Stobo");
    al_draw_textf(credits_font, al_map_rgb(255, 255, 255), x_location,
                  320 + padding, 1, "Level/game design by Max Keleher");
    al_draw_textf(credits_font, al_map_rgb(255, 255, 255), x_location,
                  360 + padding, 1, "Lead code by Danny Van Stemp");
    al_draw_textf(credits_font, al_map_rgb(255, 255, 255), x_location,
                  400 + padding, 1, "Art made in Paint.net and Asperite");
    al_draw_textf(credits_font, al_map_rgb(255, 255, 255), x_location,
                  440 + padding, 1, "Music made in FL Studio");
    al_draw_textf(credits_font, al_map_rgb(255, 255, 255), 835, 320, 1,
                  "Made for TOJam 12");
    al_draw_textf(credits_font, al_map_rgb(255, 255, 255), 835, 280, 1,
                  "ADS Games, 2017");
    al_draw_textf(credits_font, al_map_rgb(255, 100, 100), 40, 720, 0,
                  "Press any key to return.");
  }
}
