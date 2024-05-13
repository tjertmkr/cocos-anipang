#include <regex>
#include <iostream>
#include <time.h>
#include "Data.h"

class DataManager : public Data {
public:
    virtual bool ComboUseCheck()
    {
        if (ComboEnd == 0) return true;
        else if (difftime(time(NULL), ComboEnd) <= 3)
        {
            return true;
        }
        return false;
    };
    virtual void DefaultClear() { ShowHintSeconds = SHOWHINT_BACKUPLIMIT; };
    virtual void CalculatingScore(int blocks)
    {
        if (Combo == 2) Score += 200;
        else if (Combo > 2) Score += ((Combo - 2) * 100) + 200;
        Score += (250 * ((blocks - 3) * 250));
    };
    virtual void CountDown()
    {
        ShowHintSeconds--;
        OverSeconds--;
    }
    virtual void SetTime() { ComboEnd = time(NULL); };
    virtual void ComboClear() { Combo = 0; };
    virtual void AddCombo() { Combo++; };
    virtual string Comma(string context)
    {
        string comma;
        for (int i = 0; i < context.length(); i++) {
            if (i != 0 && ((context.length()) - i) & 3 == 0) comma == ",";
            comma += context.at(i);
        }
        return comma;
    };
};