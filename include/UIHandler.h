#ifndef UIHANDLER_H
#define UIHANDLER_H

#include <vector>
#include <string>

#include "UIElement.h"
#include "Globals.h"

//class UIElement;

class UIHandler
{
  public:
    UIHandler() {};
    virtual ~UIHandler() {};
    void addElement(UIElement *newUIElement);
    void draw();
    void update();
    bool isHovering();

    void createButton(int x, int y,std::string newText,ALLEGRO_FONT *newFont);
    void createAnchoredButton(std::string,ALLEGRO_FONT *,std::string,bool);
    void createAnchoredButton(std::string,ALLEGRO_FONT *,std::string,std::string,bool);

    UIElement* getElementByText(std::string);
    UIElement* getElementById(std::string);

    std::vector<UIElement*> getUIElements();

  private:
    std::vector<UIElement*> ui_elements;
};

#endif // UIHANDLER_H
