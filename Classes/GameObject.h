#include "cocos2d.h"

class GameObject
{
public:
    bool Bombed;

    int EffectLevel;
    int IconType;

    cocos2d::Sprite* Component;
};

class SimpleObject
{
public:
    bool Bombed;
    int IconType;

    SimpleObject(bool b, int i)
    {
        Bombed = b;
        IconType = i;
    }

    static SimpleObject* create(bool b, int i)
    {
        return new SimpleObject(b, i);
    }
};