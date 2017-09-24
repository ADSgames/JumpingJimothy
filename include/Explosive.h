#ifndef EXPLOSIVE_H
#define EXPLOSIVE_H

#include <Box.h>
#include <Character.h>
#include <globals.h>


class Explosive : public Box
{
  public:
    Explosive();
    virtual ~Explosive();
    void draw();
    void update();

    void init(float, float, float, float,float,float,bool,BITMAP*, b2World *, Character *);
    void applyBlastImpulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower);

  protected:

    Character *gameCharacter;
    int numRays;
    int blastRadius;
    int blastPower;
    bool affect_character;

  private:
};

#endif // EXPLOSIVE_H
