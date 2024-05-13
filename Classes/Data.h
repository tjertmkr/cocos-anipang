#include "cocos2d.h"

using namespace std;

class Data {
public:
    Data()
    {
        SHOWHINT_BACKUPLIMIT = 10;
        ShowHintSeconds = SHOWHINT_BACKUPLIMIT;
        OverSeconds = 60;
        Score = 0;
        Combo = 0;
        Rounds = 0;
        ComboEnd = 0;
    }

    int OverSeconds;
    int ShowHintSeconds;
    int Score;
    int Combo;
    int Rounds;
    int SHOWHINT_BACKUPLIMIT;

    long ComboEnd;
};