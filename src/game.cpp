#include "game.h"

// Constructor
game::game(){
  b2_setup();
  load_sprites();
  load_world();
}

// Destructor
game::~game(){

}

// Creates box in world
void game::create_box( float newX, float newY, float newWidth, float newHeight, bool newBodyType, bool newIsSensor){
  Box *newBox = new Box();
  newBox -> init( newX, newY, newWidth, newHeight, newBodyType, box, gameWorld);
  gameBoxes.push_back( newBox);
}

// Add character to world
void game::create_character( float newX, float newY){
  Character *newCharacter = new Character();
  newCharacter -> init( newX, newY, gameWorld);
  gameBoxes.push_back(newCharacter);
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
	groundBodyDef.position.Set( 0.0f, -40.0f);

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = gameWorld -> CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2PolygonShape groundBox;

	// The extents are the half-widths of the box.
	groundBox.SetAsBox( 50.0f, 10.0f);

	// Add the ground fixture to the ground body.
	groundBody -> CreateFixture( &groundBox, 0.0f);

}

// Load world from xml
void game::load_world(){
  // Doc
  rapidxml::xml_document<> doc;
  rapidxml::xml_node<> * root_node;

  // Make an xml object
  std::ifstream theFile( "data/Level.xml");
  std::vector<char> xml_buffer( (std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
  xml_buffer.push_back('\0');

  // Parse the buffer using the xml file parsing library into doc
  doc.parse<0>(&xml_buffer[0]);

  // Find our root node
  root_node = doc.first_node("data");

  // Iteratboole over the brewerys
  for (rapidxml::xml_node<> * object_node = root_node -> first_node("Object"); object_node; object_node = object_node -> next_sibling()){
    std::string type = "";
    std::string x = "";
    std::string y = "";
    std::string bodytype = "";

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
        default:
          break;
      }
      i++;
    }
    if( type == "Tile")
      create_box( tools::string_to_float(x), tools::string_to_float(y), 1.6, 1.6, bodytype != "Static", false);
    if( type == "Character")
      create_character( tools::string_to_float(x),tools::string_to_float(y));
  }
}

// Load all sprites for in game
void game::load_sprites(){
  box = tools::load_bitmap_ex( "box.png");
}

// Update game logic
void game::update(){
  if( mouseListener::mouse_pressed & 1)
    create_box( mouseListener::mouse_x / 20, -mouseListener::mouse_y / 20, 1.6, 1.6, true, false);

  if( mouseListener::mouse_pressed & 2)
    create_character( mouseListener::mouse_x / 20, -mouseListener::mouse_y / 20);

  // Update the Box2D game world
  gameWorld -> Step( timeStep, velocityIterations, positionIterations);

  // Update character
  for( unsigned int i = 0; i < gameBoxes.size(); i++){
    if( gameBoxes[i] -> getType() == CHARACTER){
      gameBoxes[i] -> update();
    }
  }{

}

  // Pause/Play time
  if(keyListener::lastKeyPressed==ALLEGRO_KEY_SPACE){
    static_mode =! static_mode;
    if(static_mode){
      for( unsigned int i = 0; i < gameBoxes.size(); i++){
        if( gameBoxes[i] -> getType()==BOX){
          // Character *newCharacter = dynamic_cast<Character*>(&gameBoxes[i]);
          gameBoxes[i] -> setStatic();
        }
      }
    }
    else{
      for( unsigned int i = 0; i < gameBoxes.size(); i++){
        if( gameBoxes[i] -> getType()==BOX){
          // Character *newCharacter = dynamic_cast<Character*>(&gameBoxes[i]);
          gameBoxes[i] -> setDynamic();
        }
      }
    }
  }
}

// Draw to screen
void game::draw(){
  // Background
  al_clear_to_color( al_map_rgb(200,200,255));

  // Draw boxes
  for( unsigned int i = 0; i < gameBoxes.size(); i++){
    gameBoxes[i] -> draw();
  }
}