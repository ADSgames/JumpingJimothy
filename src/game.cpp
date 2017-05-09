#include "game.h"
#include <allegro5/bitmap.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_native_dialog.h>

// Constructor
game::game(){
  // Reset fresh
  reset();

  // Load and play music
  music = tools::load_sample_ex( "music/tojam.ogg");
  al_play_sample( music, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, currentMusic);
}

// Destructor
game::~game(){
  // Stop music
  al_stop_sample( currentMusic);
}

// Creates box in world
Box *game::create_goat( float newX, float newY){
  Box *newBox = new Box();
  newBox -> init( newX, newY,goat_map, gameWorld,gameCharacter);
  gameBoxes.push_back( newBox);
  return newBox;
}

// Creates box in world
Box *game::create_box( float newX, float newY, float newWidth, float newHeight, float newVelX,float newVelY, ALLEGRO_BITMAP *newSprite, bool newBodyType, bool newIsSensor){
  Box *newBox = new Box();
  newBox -> init( newX, newY, newWidth, newHeight, newVelX,newVelY,newBodyType, newSprite, gameWorld);
  gameBoxes.push_back( newBox);
  return newBox;
}

// Add character to world
Character *game::create_character( float newX, float newY){
  Character *newCharacter = new Character();
  newCharacter -> init( newX, newY,character, gameWorld);
  gameBoxes.push_back(newCharacter);
  return newCharacter;
}

// Sets up Box2D world
void game::b2_setup(){
  // Box2D world parameters
  gravity = b2Vec2(0.0f, -10.0f);
  timeStep = 1.0f / 60.0f;
  velocityIterations = 6;
  positionIterations = 2;
  doSleep = true;

  // Box2D game world
  gameWorld = new b2World( gravity, doSleep);

	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set( 0.0f, -48.4f);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	groundBody = gameWorld -> CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox( 80.0f, 10.0f);

	// Add the ground fixture to the ground body.
	groundBody -> CreateFixture( &groundBox, 0.0f);
}

// Load world from xml
void game::load_world(int newLevel){
  // Destroy ground
  gameWorld -> DestroyBody(groundBody);

  // Doc
  rapidxml::xml_document<> doc;
  rapidxml::xml_node<> * root_node;

  // Make an xml object
  std::ifstream theFile( "data/level_"+tools::convertIntToString(newLevel) +".xml");
  std::vector<char> xml_buffer( (std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
  xml_buffer.push_back('\0');

  // Parse the buffer using the xml file parsing library into doc
  doc.parse<0>(&xml_buffer[0]);

  // Find our root node
  root_node = doc.first_node("data");

  // Iteratboole over the nodes
  for (rapidxml::xml_node<> * object_node = root_node -> first_node("Object"); object_node; object_node = object_node -> next_sibling()){
    std::string type = "";
    std::string x = "";
    std::string y = "";
    std::string orientation = "0";
    std::string bodytype = "totally not static";
    std::string vel_x = "0";
    std::string vel_y = "0";


    // Interate over
    // int generatedNumberResult = atoi( generated_node -> first_attribute("number") -> value());
    // if( generatedNumberResult == random_number){
      // for(rapidxml::xml_node<> * tile_node = object_node -> first_node("x"); tile_node; tile_node = tile_node -> next_sibling()){
    type = object_node -> first_attribute("type") -> value();

    int i = 0;
    for( rapidxml::xml_node<> * map_item = object_node->first_node("x"); map_item; map_item = map_item->next_sibling()){
      switch( i ){
        case 0:
          x = map_item -> value();
          break;
        case 1:
          y = map_item -> value();
          break;
        case 2:
          bodytype = map_item -> value();
          break;
        case 3:
          orientation = map_item -> value();
          break;
        case 4:
          vel_x = map_item -> value();
          break;
        case 5:
          vel_y = map_item -> value();
          break;

        default:
          break;
      }
      i++;
    }
    if( type == "Tile"){
      if(bodytype=="Static")
        newBox = create_box( tools::string_to_float(x), tools::string_to_float(y), 1.5, 1.5,tools::string_to_float(vel_x),tools::string_to_float(vel_y), tiles[1][tools::convertStringToInt(orientation)], bodytype != "Static", false);
      else
        newBox = create_box( tools::string_to_float(x), tools::string_to_float(y), 1.6, 1.6,tools::string_to_float(vel_x),tools::string_to_float(vel_y),box, bodytype != "Static", false);
    }
    if( type == "Character")
      gameCharacter = create_character( tools::string_to_float(x),tools::string_to_float(y));
    if( type == "Finish")
      goat = create_goat( tools::string_to_float(x),tools::string_to_float(y));
//    if(group=="1"){
//      if(rootBox==nullptr)
//        rootBox=newBox;
//      else{
//        b2Vec2 FeetAnchor(0,0);
//
//
//        b2WeldJointDef *jointDef = new b2WeldJointDef();
//        jointDef -> Initialize(newBox -> getBody(), rootBox -> getBody(), FeetAnchor);
//        jointDef -> collideConnected = false;
//        jointDef  -> referenceAngle = 0;
//        gameWorld -> CreateJoint(jointDef);
//      }
//
//
//    }

  }
}

// Reset game
void game::reset(){
  gameBoxes.clear();
  goat = nullptr;
  gameCharacter = nullptr;
  b2_setup();
  load_sprites();
  load_world(level);
  static_mode=true;
  first_play=true;

   for( unsigned int i = 0; i < gameBoxes.size(); i++){
    if( gameBoxes[i] -> getType()==BOX){
      gameBoxes[i] -> setStatic();
    }
  }
}

// Load all sprites for in game
void game::load_sprites(){
  box = tools::load_bitmap_ex( "images/box.png");
  goat_sprite = tools::load_bitmap_ex( "images/goat.png");
  goat_map = tools::load_bitmap_ex( "images/goat_map.png");

  help = tools::load_bitmap_ex( "images/help.png");

  character = tools::load_bitmap_ex( "images/character.png");
  static_tileset = tools::load_bitmap_ex( "images/StaticBlock.png");
  play = tools::load_bitmap_ex( "images/play.png");
  pause = tools::load_bitmap_ex( "images/pause.png");

  for( int i = 0; i < 4; i++)
    for( int t = 0; t < 12; t++)
      tiles[1][i + t*4] = al_create_sub_bitmap( static_tileset, i * 32, t * 32, 32, 32);
}

// Update game logic
void game::update(){
  // Game mode
  if( keyListener::keyPressed[ALLEGRO_KEY_P]){
    set_next_state( STATE_EDIT);
  }

  // Touching goat
  if( goat -> getGoatWin()){
    level ++;
    if( level >= 10){
      al_show_native_message_box( nullptr, "Hurrah!", "Victory", "Will you accept your prize?", nullptr, ALLEGRO_MESSAGEBOX_YES_NO);
      gameCharacter -> getBody() -> SetTransform( b2Vec2( 100, 100), 0);
    }
    else{
      reset();
    }
  }

  // Off screen
  if( gameCharacter -> getX() < -1 || gameCharacter -> getX() > 51.5f || gameCharacter -> getY() > 2 || gameCharacter -> getY() < -40)
    reset();

  // Update the Box2D game world
  gameWorld -> Step( timeStep, velocityIterations, positionIterations);

  // Update character
  for( unsigned int i = 0; i < gameBoxes.size(); i++){
    if( gameBoxes[i] -> getType() == CHARACTER){
      gameBoxes[i] -> update();
    }
  }

  // Die
  if( keyListener::keyPressed[ALLEGRO_KEY_Z] || joystickListener::buttonPressed[JOY_XBOX_B])
    reset();

  // Next level
  if( keyListener::keyPressed[ALLEGRO_KEY_C]){
    level++;
    reset();
  }

  // Previous level
  if( keyListener::keyPressed[ALLEGRO_KEY_X]){
    if( level > 1)
      level--;
    reset();
  }

  // Pause/Play time
  if( keyListener::keyPressed[ALLEGRO_KEY_SPACE]  || joystickListener::buttonPressed[JOY_XBOX_X] ||  joystickListener::buttonPressed[JOY_XBOX_Y] || joystickListener::buttonPressed[JOY_XBOX_BUMPER_LEFT] || joystickListener::buttonPressed[JOY_XBOX_BUMPER_RIGHT]  ){
    static_mode =! static_mode;
    if(static_mode){
      for( unsigned int i = 0; i < gameBoxes.size(); i++){
        if( gameBoxes[i] -> getType() == BOX){
          // Character *newCharacter = dynamic_cast<Character*>(&gameBoxes[i]);
          gameBoxes[i] -> setStatic();
        }
      }
    }
    else{
      for( unsigned int i = 0; i < gameBoxes.size(); i++){
        if( gameBoxes[i] -> getType()==BOX){
          // Character *newCharacter = dynamic_cast<Character*>(&gameBoxes[i]);
          gameBoxes[i] -> setDynamic(!first_play);
        }
      }
      first_play = false;
    }
  }
}

// Draw to screen
void game::draw(){
  // Background
  al_clear_to_color( al_map_rgb(40,40,60));

  // Help
  if( level == 1)
    al_draw_bitmap(help,0,0,0);

  // Draw boxes
  for( unsigned int i = 0; i < gameBoxes.size(); i++)
    gameBoxes[i] -> draw();

  // Pause/play buttons
  if(static_mode)
    al_draw_bitmap(pause,10,10,0);
  if(!static_mode)
    al_draw_bitmap(play,10,10,0);
}
