#include "cocos2d.h"
#include "GameObject.h"
#include "DataManager.h""

USING_NS_CC;
using namespace std;

class GameScene : public cocos2d::Scene
{
public:

    static cocos2d::Scene* createScene();

    DataManager* Variables = new DataManager();

    Sprite* GameShowHintComponent = NULL;
    Sprite* GameRetryButton = NULL;

    Label* GameTimeLimitComponent = NULL;
    Label* GameScoreComponent = NULL;
    Label* GameComboComponent = NULL;

    ProgressTimer* GameScoreBar;
    ProgressTimer* GameComboModeBar;

    tuple<int, int> ClickTarget;
    tuple<int, int> ClickDirection;

    EventListenerTouchOneByOne* ClickListener;

    vector<vector<GameObject*>> Components;
    vector<vector<SimpleObject*>> BeforeComponents;

    bool Clicked = false;
    bool CantDoControl = false;
    bool RetryCantDoControl = false; // 버그 방지
    bool UseBombed = false; // 폭탄을 사용했을때 함수 하나를 건너띔
    bool UIHinted = false;

    int underComponent(int x, int y);

    virtual bool init();

    bool isSamed(tuple<int, int> com1, tuple<int, int> com2, int number);
    bool tuple_overlapping(vector<tuple<int, int>> com1, vector<tuple<int, int>> com2);
    bool overlapping(vector<tuple<int, int>> components, int x, int y);
    bool touch_begin(cocos2d::Touch* touch, cocos2d::Event* event);
    bool dohint(int x, int y);

    vector<tuple<int, int>> canpang();
    vector<tuple<int, int>> canhint();
    vector<tuple<int, int>> doingFrontComponent(int x, int y);
    vector<vector<tuple<int, int>>> differentiate(vector<tuple<int, int>> components);

    void candocontrol_seter();
    void updater(float dt);
    void scene_retry();
    void removeEffects();
    void removeShowHint();
    void reorganizationScore();
    void reorganizationTimeLimit(bool over);

    void createdown();
    void createComponent(int x, int y, Vec2 vec);

    void candown();
    void candownAction(int x, int y);

    void canswap_calloc();
    void canswap(tuple<int, int> component1, tuple<int, int> component2, float deltatime, bool usefunc);
    void showCombo();

    void function(vector<tuple<int, int>> canpangs);
    void comboFunction();

    void controlSystem(vector<tuple<int, int>> components);
    void use_item(int x, int y);

    void dataCanswap(int componentX, int componentY, int component2X, int component2Y);
    void shader_show(vector<tuple<int, int>> components);
    void remove_component_all(vector<tuple<int, int>> components);
    void effect_pang(int x, int y);

    void retain();

    void fixed_candocontrol_seter();

    void setup();
    void unnecessaryRemove();
    void arrange();
    void start_background();
    void removeChilds();
    void componentBeforeAppend();

    void touch_moved(Touch* touch, Event* event);
    void touch_ended(cocos2d::Touch* touch, cocos2d::Event* event);
    void touch_event_append();

    tuple<int, int> decide(int original_x, int original_y, int moved_x, int moved_y);
    tuple<int, int> candownComponent(int x, int y);

    CREATE_FUNC(GameScene);

};