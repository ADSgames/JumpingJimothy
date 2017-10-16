#ifndef UIELEMENT_H
#define UIELEMENT_H

#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>

//#include "Button.h"
#include <string>
#include <mouseListener.h>
#include <tools.h>

class UIElement
{
  public:
    // Constructor
    UIElement();

    ~UIElement();

    // Getters
    int getX(){ return x; }
    int getY(){ return y; }


    std::string getText(){ return text; }

    void setVisibility( bool newVisible){ visible = newVisible; }
    void toggleVisibility();
    void toggleStatus();
    void setTransparency(float newAlpha){alpha = newAlpha;}
    void setX(int newX){x=newX;}

    int getWidth(){ return width + padding_x * 2; }
    int getHeight(){ return height + padding_y * 2; }
    int getPaddingX(){ return padding_x; }
    int getPaddingY(){ return padding_y; }
    int getRightX(){ return x+getWidth();}
    void setPadding( int padding_x, int padding_y){ this -> padding_x = padding_x; this -> padding_y = padding_y; }
    // Setters
    void setPosition( int x, int y){ this -> x = x; this -> y = y; }
    void setSize( int width, int height){ this -> width = width; this -> height = height;}
    void setText( std::string text){ this -> text = text; }
    void setImage( ALLEGRO_BITMAP *image);
    void setFont( ALLEGRO_FONT *font);

    bool mouseReleased();
    bool hover();
    bool clicked();
    void update();

    //

    virtual void draw(){};


  protected:
     // Variables
    int x;
    int y;
    int width;
    int height;

    int padding_x;
    int padding_y;

    bool hovering;
    bool old_mouse_down;
    bool mouse_released;

    float alpha;

    // Inactive cannot be clicked/hovered
    // Invisible cannot be seen
    // Disabled is both
    bool visible;
    bool active;

    ALLEGRO_BITMAP *image;

    // Font
    ALLEGRO_FONT *UIElement_font;

    std::string text;
    std::string id;


  private:

};

#endif // UIELEMENT_H