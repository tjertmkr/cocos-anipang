#include "GameScene.h"
#include "AppDelegate.h"
#include "SimpleAudioEngine.h"
#include "Scripts.h"

USING_NS_CC;
using namespace std;
using namespace CocosDenshion;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init()) return false;

    setup();

    arrange();
    touch_event_append();
    start_background();
    schedule(CC_SCHEDULE_SELECTOR(GameScene::updater), 1.0f);
    componentBeforeAppend();

    return true;
}

void GameScene::updater(float dt)
{
    Variables->CountDown();
    if (!(Variables->OverSeconds < 1))
    {
        reorganizationTimeLimit(false);
        if (Variables->ShowHintSeconds <= 0)
        {
            if (GameShowHintComponent == NULL)
            {
                vector<tuple<int, int>> canhints = canhint();
                if (canhints.size() <= 0)
                {
                    removeChilds();
                    setup();
                    arrange();
                }
                else
                {
                    tuple<int, int> showhint = canhints[rand() % canhints.size()];

                    GameShowHintComponent = Sprite::create(AssetsShowHint);
                    GameShowHintComponent->setAnchorPoint(Vec2(0.1, 0.1));
                    GameShowHintComponent->setOpacity(0);
                    GameShowHintComponent->setPosition(Vec2(get<1>(showhint) * INTERVAL_NUMBER + INTERVAL_STEP_FORWARD_NUMBER, (get<0>(showhint) + 1) * INTERVAL_NUMBER + INTERVAL_STEP_FORWARD_NUMBER));
                    GameShowHintComponent->runAction(RepeatForever::create(
                        Sequence::create(FadeIn::create(ActionShowHintDeltaTime), DelayTime::create(ActionDelayTime), FadeOut::create(ActionShowHintDeltaTime), NULL)
                    ));

                    this->addChild(GameShowHintComponent, 1);
                }
            }
        }
    }
    else
    {
        fixed_candocontrol_seter();
        removeEffects();
        reorganizationTimeLimit(true);
        unschedule(CC_SCHEDULE_SELECTOR(GameScene::updater));
        scene_retry();
    }
}

void GameScene::scene_retry()
{
    Sprite* disposable_1 = Sprite::create(UIGameOver);
    disposable_1->setPosition(Vec2(DEFAULT_WIDTH / 2, DEFAULT_HEIGHT / 2));
    disposable_1->setOpacity(240);

    GameRetryButton = Sprite::create(UIReStartButton);
    GameRetryButton->setPosition(Vec2(DEFAULT_WIDTH / 2, DEFAULT_HEIGHT / 2));

    Label* disposable_2 = Label::createWithTTF(Variables->Comma(to_string(Variables->Score)), UIScoreTTF, 50, Size::ZERO, TextHAlignment::CENTER);
    disposable_2->setPosition(Vec2(DEFAULT_WIDTH / 2, (DEFAULT_HEIGHT / 2) + 70));
    disposable_2->enableOutline(Color4B::BLACK, 3);

    this->addChild(disposable_1, 2);
    this->addChild(GameRetryButton, 3);
    this->addChild(disposable_2, 3);
}

void GameScene::removeEffects()
{
    if (GameShowHintComponent != NULL)
    {
        this->removeChild(GameShowHintComponent);
        GameShowHintComponent = NULL;
    }
}

void GameScene::removeChilds()
{
    for (int i = 0; i < Components.size(); i++)
    {
        for (int j = 0; j < Components[i].size(); j++)
        {
            this->removeChild(Components[i][j]->Component);
        }
    }
    Components.clear();
}

void GameScene::removeShowHint()
{
    if (GameShowHintComponent != NULL)
    {
        this->removeChild(GameShowHintComponent);
        GameShowHintComponent = NULL;
    }
}

void GameScene::arrange()
{
    for (int i = 0; i < Components.size(); i++)
    {
        for (int j = 0; j < Components[i].size(); j++)
        {
            Components[i][j]->Component->setAnchorPoint(Vec2(0.1, 0.1));
            Components[i][j]->Component->setPosition(Vec2(j * INTERVAL_NUMBER + INTERVAL_STEP_FORWARD_NUMBER, (i + 1) * INTERVAL_NUMBER + INTERVAL_STEP_FORWARD_NUMBER));

            this->addChild(Components[i][j]->Component, 0);
        }
    }
}

void GameScene::start_background()
{
    Sprite* SpriteUIUnder = Sprite::create(UIUnderUI);
    Sprite* SpriteUIUp = Sprite::create(UIUpUI);
    Sprite* SpriteUIScoreBar = Sprite::create(UIScoreBar);

    GameTimeLimitComponent = Label::createWithTTF(to_string(Variables->OverSeconds), UITimeLimitBarTTF, 20, Size::ZERO, TextHAlignment::CENTER);
    GameTimeLimitComponent->setPosition(Vec2(DEFAULT_WIDTH / 2, INTERVAL_NUMBER / 3.95));

    GameScoreComponent = Label::createWithTTF(to_string(Variables->Score), UIScoreTTF, 40, Size::ZERO, TextHAlignment::CENTER);
    GameScoreComponent->setPosition(Vec2(DEFAULT_WIDTH / 2, DEFAULT_HEIGHT - (INTERVAL_NUMBER * 2) / 3));
    GameScoreComponent->enableOutline(Color4B::BLACK, 3);

    GameScoreBar = ProgressTimer::create(Sprite::create(UIProgressBar));
    GameScoreBar->setPosition(Vec2(DEFAULT_WIDTH / 2, INTERVAL_NUMBER / 3.95));
    GameScoreBar->setPercentage(100.0f);
    GameScoreBar->setMidpoint(Point(0, 0));
    GameScoreBar->setBarChangeRate(Point(1, 0));
    GameScoreBar->setType(ProgressTimer::Type::BAR);

    SpriteUIUnder->setPosition(Vec2(DEFAULT_WIDTH / 2, 280));
    SpriteUIUp->setPosition(Vec2(DEFAULT_WIDTH / 2, DEFAULT_HEIGHT - INTERVAL_NUMBER));
    SpriteUIScoreBar->setPosition(Vec2(DEFAULT_WIDTH / 2, INTERVAL_NUMBER / 3.95));

    this->addChild(SpriteUIUnder, -1);
    this->addChild(SpriteUIUp, 1);
    this->addChild(SpriteUIScoreBar, 1);
    this->addChild(GameScoreBar, 1);
    this->addChild(GameTimeLimitComponent, 2);
    this->addChild(GameScoreComponent, 1);

    GameScoreBar->runAction(ProgressFromTo::create(60, 100, 0));
}

void GameScene::reorganizationScore()
{
    if (GameScoreComponent != NULL)
    {
        GameScoreComponent->setString(Variables->Comma(to_string(Variables->Score)));
    }
}

void GameScene::reorganizationTimeLimit(bool over)
{
    if (GameTimeLimitComponent != NULL)
    {
        if (over == false) GameTimeLimitComponent->setString(to_string(Variables->OverSeconds));
        else
        {
            GameTimeLimitComponent->setString(TimeOut);
        }
    }
}

void GameScene::setup()
{
    srand(time(NULL));
    for (int i = 0; i < DEFAULT_MAX_HEIGHT; i++)
    {
        vector<GameObject*> subVector;
        for (int j = 0; j < DEFAULT_MAX_WIDTH; j++)
        {
            GameObject* cocositem = new GameObject();
            cocositem->IconType = rand() % CharacterList.size();
            cocositem->Component = Sprite::create(CharacterList[cocositem->IconType]);
            subVector.push_back(cocositem);
        }
        if (!(subVector.size() <= 0)) Components.push_back(subVector);
    }
    unnecessaryRemove();
}

void GameScene::unnecessaryRemove()
{
    auto canpangs = canpang();
    for (; !(canpangs.size() <= 0);) {
        for (int i = 0; i < canpangs.size(); i++) {
            Components[get<0>(canpangs[i])][get<1>(canpangs[i])]->IconType = rand() % CharacterList.size();;
            Components[get<0>(canpangs[i])][get<1>(canpangs[i])]->Component = Sprite::create(CharacterList[Components[get<0>(canpangs[i])][get<1>(canpangs[i])]->IconType]);
        }
        canpangs = canpang();
    }
}

int GameScene::underComponent(int x, int y)
{
    int under_number = 0;
    if (y <= 0) return under_number;

    for (int i = (y - 1); i >= 0; i--)
    {
        if (Components[i][x]->Component == NULL) under_number++;
    }
    return under_number;
}

void GameScene::createdown()
{
    for (int i = (DEFAULT_MAX_HEIGHT - 1); i >= 0; i--) {
        for (int j = (DEFAULT_MAX_WIDTH - 1); j >= 0; j--) {
            if (Components[i][j]->Component == NULL) {
                int under_component_number = underComponent(j, i);
                createComponent(j, i, Vec2(j * INTERVAL_NUMBER + INTERVAL_STEP_FORWARD_NUMBER, (under_component_number + (DEFAULT_MAX_HEIGHT + 1)) * INTERVAL_NUMBER + INTERVAL_STEP_FORWARD_NUMBER));
                candownAction(j, i);
            }
        }
    }
}

void GameScene::createComponent(int x, int y, Vec2 vec)
{
    Components[y][x]->IconType = rand() % CharacterList.size();
    Components[y][x]->Component = Sprite::create(CharacterList[Components[y][x]->IconType]);
    Components[y][x]->Component->setAnchorPoint(Vec2(0.1, 0.1));
    Components[y][x]->Component->setPosition(vec);

    this->addChild(Components[y][x]->Component, 0);
}

void GameScene::candown()
{
    for (int i = 0; i < Components.size(); i++) {
        for (int j = 0; j < Components[i].size(); j++) {
            if (Components[i][j]->Component == NULL) {
                tuple<int, int> up_component = candownComponent(j, i);
                if (!(get<0>(up_component) == -1 && get<1>(up_component) == -1)) {
                    dataCanswap(j, i, get<0>(up_component), get<1>(up_component));
                    candownAction(j, i);
                }
            }
        }
    }
    createdown();
}

void GameScene::candownAction(int x, int y)
{
    Components[y][x]->Component->runAction(MoveTo::create(ActionDownDeltatime, Vec2(x * INTERVAL_NUMBER + INTERVAL_STEP_FORWARD_NUMBER, (y + 1) * INTERVAL_NUMBER + INTERVAL_STEP_FORWARD_NUMBER)));
}

tuple<int, int> GameScene::candownComponent(int x, int y)
{
    for (int i = y; i < Components.size(); i++) {
        if (Components[i][x]->Component != NULL) return make_tuple(x, i);
    }
    return make_tuple(-1, -1);
}

void GameScene::dataCanswap(int componentX, int componentY, int component2X, int component2Y)
{
    GameObject* replaceItem = Components[componentY][componentX];
    Components[componentY][componentX] = Components[component2Y][component2X];
    Components[component2Y][component2X] = replaceItem;
}

void GameScene::canswap(tuple<int, int> component1, tuple<int, int> component2, float deltatime, bool usefunc)
{
    int swap_number0 = get<0>(component1) + get<0>(component2);
    int swap_number1 = get<1>(component1) + get<1>(component2);

    int original_number0 = get<0>(component1);
    int original_number1 = get<1>(component1);

    Vec2 swap_component_vec = Components[original_number1][original_number0]->Component->getPosition();
    Vec2 original_component_vec = Components[swap_number1][swap_number0]->Component->getPosition();

    dataCanswap(original_number0, original_number1, swap_number0, swap_number1);
    if (!(CantDoControl) && usefunc)
    {
        candocontrol_seter();
    }

    Components[original_number1][original_number0]->Component->runAction(MoveTo::create(deltatime, swap_component_vec));
    if (usefunc == true) {
        Components[swap_number1][swap_number0]->Component->runAction(Sequence::create(MoveTo::create(deltatime, original_component_vec),
            CallFunc::create(CC_CALLBACK_0(GameScene::canswap_calloc, this)), NULL));
    }
    else {
        Components[swap_number1][swap_number0]->Component->runAction(
            Sequence::create(MoveTo::create(deltatime, original_component_vec), CallFunc::create(CC_CALLBACK_0(GameScene::candocontrol_seter, this)), NULL));
    }
}

void GameScene::shader_show(vector<tuple<int, int>> components)
{
    for (int i = 0; i < components.size(); i++)
    {
        this->Components[get<0>(components[i])][get<1>(components[i])]->Component->setTexture(ShaderList[Components[get<0>(components[i])][get<1>(components[i])]->IconType]);
    }
    //SimpleAudioEngine::getInstance()->playEffect(PangEffect, true);
}

void GameScene::remove_component_all(vector<tuple<int, int>> components)
{
    for (int i = 0; i < components.size(); i++)
    {
        this->effect_pang(get<1>(components[i]), get<0>(components[i]));
        this->removeChild(Components[get<0>(components[i])][get<1>(components[i])]->Component);
        this->Components[get<0>(components[i])][get<1>(components[i])]->Component = NULL;
        this->Components[get<0>(components[i])][get<1>(components[i])]->Bombed = false;
    }
    Variables->CalculatingScore(components.size());
    reorganizationScore();
}

void GameScene::effect_pang(int x, int y)
{
    ParticleSystemQuad* ParticleBoom1 = ParticleSystemQuad::create(CharacterPangParticle);
    ParticleBoom1->setScale(0.4);
    ParticleBoom1->setAnchorPoint(Vec2(0.1, 0.1));
    ParticleBoom1->setPosition(
        Vec2(
            x * INTERVAL_NUMBER + (INTERVAL_STEP_FORWARD_NUMBER + INTERVAL_PARTICLE_STEP_FORWARD_NUMBER),
            (y + 1) * INTERVAL_NUMBER + (INTERVAL_STEP_FORWARD_NUMBER + INTERVAL_PARTICLE_STEP_FORWARD_NUMBER)
        )
    );
    this->addChild(ParticleBoom1, 1);
}

void GameScene::function(vector<tuple<int, int>> canpangs)
{
    this->runAction(Sequence::create(
        CallFunc::create(CC_CALLBACK_0(GameScene::shader_show, this, canpangs)), DelayTime::create(ActionDelayTime)->clone(),
        CallFunc::create(CC_CALLBACK_0(GameScene::remove_component_all, this, canpangs)),
        CallFunc::create(CC_CALLBACK_0(GameScene::controlSystem, this, canpangs)), DelayTime::create(ActionParticleAfterDeltatime)->clone(),
        CallFunc::create(CC_CALLBACK_0(GameScene::candown, this)), DelayTime::create(ActionDelayTime)->clone(),
        CallFunc::create(CC_CALLBACK_0(GameScene::retain, this)), NULL));
}

void GameScene::comboFunction()
{
    if (Variables->ComboUseCheck() == true)
    {
        Variables->AddCombo();
    }
    else
    {
        Variables->ComboClear();
        Variables->AddCombo();
    }
    Variables->SetTime();
    showCombo();
}

void GameScene::showCombo()
{
    if (GameComboComponent == NULL)
    {
        GameComboComponent = Label::createWithTTF("Combo " + to_string(Variables->Combo), UIComboTTF, 30, Size::ZERO, TextHAlignment::CENTER);
        GameComboComponent->setAnchorPoint(Vec2(0.1, 0.1));
        this->addChild(GameComboComponent, 2);
    }
    else
    {
        GameComboComponent->setString("Combo " + to_string(Variables->Combo));
    }
    GameComboComponent->setOpacity(255);
    GameComboComponent->setPosition(Vec2(DEFAULT_WIDTH / 2.6, DEFAULT_HEIGHT - (INTERVAL_NUMBER * 4.5) / 2));
    GameComboComponent->runAction(Sequence::create(
        ScaleBy::create(0.1, 1.5), ScaleTo::create(0.15, 1.2), Spawn::createWithTwoActions(FadeOut::create(0.7f), MoveBy::create(0.6f, Vec2(0, 60))), ScaleTo::create(0.1, 1), NULL));
}

void GameScene::canswap_calloc()
{
    vector<tuple<int, int>> canpangs = canpang();
    if (canpangs.size() <= 0)
    {
        canswap(ClickTarget, ClickDirection, ActionSwapDeltatime, false);
    }
    else
    {
        function(canpangs);
        comboFunction();
    }
}

void GameScene::retain()
{
    vector<tuple<int, int>> retain_canpangs = canpang();
    if (!(retain_canpangs.size() <= 0))
    {
        function(retain_canpangs);
    }
    else
    {
        if (!(Variables->Rounds <= 0)) Variables->Rounds = 0;
        candocontrol_seter();
    }
}

void GameScene::candocontrol_seter()
{
    CantDoControl = !CantDoControl;
}

void GameScene::fixed_candocontrol_seter()
{
    RetryCantDoControl = true;
}

void GameScene::use_item(int x, int y)
{
    vector<tuple<int, int>> remove_components;
    if (Components[y][x]->EffectLevel == 1)
    {
        for (int i = 0; i < DEFAULT_MAX_WIDTH; i++)
        {
            remove_components.push_back(make_tuple(y, i));
        }
    }
    else if (Components[y][x]->EffectLevel == 2)
    {
        for (int i = 0; i < DEFAULT_MAX_WIDTH; i++)
        {
            if (overlapping(remove_components, y, i) == false)
            {
                remove_components.push_back(make_tuple(y, i));
            }
        }
        for (int j = 0; j < DEFAULT_MAX_HEIGHT; j++)
        {
            if (overlapping(remove_components, j, x) == false)
            {
                remove_components.push_back(make_tuple(j, x));
            }
        }
    }
    function(remove_components);
    Components[y][x]->EffectLevel = NULL;
    Components[y][x]->Bombed = false;
}

void GameScene::componentBeforeAppend()
{
    if (BeforeComponents.empty() != true)
    {
        BeforeComponents.clear();
    }
    for (int i = 0; i < Components.size(); i++)
    {
        vector<SimpleObject*> subvector;
        for (int j = 0; j < Components[i].size(); j++)
        {
            subvector.push_back(SimpleObject::create(Components[i][j]->Bombed, Components[i][j]->IconType));
        }
        BeforeComponents.push_back(subvector);
    }
}

void GameScene::controlSystem(vector<tuple<int, int>> components)
{
    if (!(UseBombed))
    {
        vector<vector<tuple<int, int>>> rc = differentiate(components);
        int x, y;
        for (int i = 0; i < rc.size(); i++)
        {
            if (rc[i].size() == 4 || rc[i].size() == 5)
            {
                if (Variables->Rounds == 0)
                {
                    int w = get<0>(ClickTarget) + get<0>(ClickDirection), h = get<1>(ClickTarget) + get<1>(ClickDirection);
                    for (int j = 0; j < rc[i].size(); j++)
                    {
                        if (get<0>(ClickTarget) == get<1>(rc[i][j]) && get<1>(ClickTarget) == get<0>(rc[i][j]))
                        {
                            x = get<0>(ClickTarget), y = get<1>(ClickTarget);
                        }
                        else if (w == get<1>(rc[i][j]) && h == get<0>(rc[i][j]))
                        {
                            x = w, y = h;
                        }
                    }
                }
                else
                {
                    for (int j = 0; j < rc[i].size(); j++)
                    {
                        if (BeforeComponents[get<0>(rc[i][j])][get<1>(rc[i][j])]->IconType != Components[get<0>(rc[i][j])][get<1>(rc[i][j])]->IconType)
                        {
                            x = get<1>(rc[i][j]), y = get<0>(rc[i][j]);
                        }
                    }
                }
                Components[y][x]->Component = Sprite::create(Levels[rc[i].size() - 4]);
                Components[y][x]->Component->setAnchorPoint(Vec2(0.1, 0.1));
                Components[y][x]->Component->setPosition(Vec2(x * INTERVAL_NUMBER + INTERVAL_STEP_FORWARD_NUMBER, (y + 1) * INTERVAL_NUMBER + INTERVAL_STEP_FORWARD_NUMBER));
                Components[y][x]->Bombed = true;
                Components[y][x]->EffectLevel = (rc[i].size() - 3);
                this->addChild(Components[y][x]->Component);
            }
        }
    }
    else
    {
        UseBombed = false;
    }
    Variables->Rounds++;
    componentBeforeAppend();
}

vector<vector<tuple<int, int>>> GameScene::differentiate(vector<tuple<int, int>> components)
{
    vector<vector<tuple<int, int>>> return_components;
    vector<tuple<int, int>> directions = {
        make_tuple(1, 0), make_tuple(-1, 0), make_tuple(0, 1), make_tuple(0, -1)
    };
    for (int i = 0; i < components.size(); i++)
    {
        bool isSamed = false; 
        vector<tuple<int, int>> target_components = { components[i] };
        for (int j = 0; j < target_components.size(); j++)
        {
            for (int k = 0; k < directions.size(); k++)
            {
                int x = get<1>(target_components[j]) + get<1>(directions[k]), y = get<0>(target_components[j]) + get<0>(directions[k]);
                for (int m = 0; m < components.size(); m++)
                {
                    if (get<1>(components[m]) == x && get<0>(components[m]) == y)
                    {
                        if (Components[y][x]->IconType == Components[get<0>(components[i])][get<1>(components[i])]->IconType)
                        {
                            if (overlapping(target_components, y, x) == false)
                            {
                                target_components.push_back(components[m]);
                            }
                        }
                    }
                }
            }
        }
        for (int k = 0; k < return_components.size(); k++)
        {
            if (tuple_overlapping(return_components[k], target_components) == true)
            {
                isSamed = true;
            }
        }
        if (isSamed == false)
        {
            return_components.push_back(target_components);
        }
    }
    return return_components;
}

vector<tuple<int, int>> GameScene::canpang()
{
    vector<tuple<int, int>> components;

    for (int i = 0; i < Components.size(); i++) {
        for (int j = 0; j < Components[i].size() - 2; j++) {
            if (Components[i][j]->IconType == Components[i][j + 1]->IconType && Components[i][j + 1]->IconType == Components[i][j + 2]->IconType) {
                if (Components[i][j]->Bombed == Components[i][j + 1]->Bombed && Components[i][j + 1]->Bombed == Components[i][j + 2]->Bombed)
                {
                    for (int k = 0; k < 3; k++) {
                        if (overlapping(components, i, j + k) == false) {
                            components.push_back(make_tuple(i, j + k)    );
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < DEFAULT_MAX_WIDTH; i++) {
        for (int j = 0; j < DEFAULT_MAX_HEIGHT - 2; j++) {
            if (Components[j][i]->IconType == Components[j + 1][i]->IconType && Components[j + 1][i]->IconType == Components[j + 2][i]->IconType) {
                if (Components[j][i]->Bombed == Components[j + 1][i]->Bombed && Components[j + 1][i]->Bombed == Components[j + 2][i]->Bombed) 
                {
                    for (int k = 0; k < 3; k++) {
                        if (overlapping(components, j + k, i) == false) {
                            components.push_back(make_tuple(j + k, i));
                        }
                    }
                }
            }
        }
    }
    return components;
}

bool GameScene::dohint(int x, int y)
{
    vector<tuple<int, int>> canpangs = canpang();
    for (int i = 0; i < canpangs.size(); i++) {
        if (get<0>(canpangs[i]) == y && get<1>(canpangs[i]) == x) return true;
    }
    return false;
}

vector<tuple<int, int>> GameScene::canhint()
{
    vector<tuple<int, int>> components;

    for (int i = 0; i < DEFAULT_MAX_HEIGHT; i++) {
        for (int j = 0; j < DEFAULT_MAX_WIDTH; j++) {

            vector<tuple<int, int>> swap_components = doingFrontComponent(j, i);
            for (int k = 0; k < swap_components.size(); k++) {
                dataCanswap(j, i, j + get<1>(swap_components[k]), i + get<0>(swap_components[k]));
                if (dohint(j, i) == true && overlapping(components, i + get<0>(swap_components[k]), j + get<1>(swap_components[k])) == false)
                {
                    components.push_back(make_tuple(i + get<0>(swap_components[k]), j + get<1>(swap_components[k])));
                }
                dataCanswap(j, i, j + get<1>(swap_components[k]), i + get<0>(swap_components[k]));
            }
        }
    }
    return components;
}

vector<tuple<int, int>> GameScene::doingFrontComponent(int x, int y)
{
    vector<tuple<int, int>> front_components;
    if (!(x <= 0))
    {
        front_components.push_back(make_tuple(0, -1));
    }
    if (!(x >= (DEFAULT_MAX_WIDTH - 1)))
    {
        front_components.push_back(make_tuple(0, 1));
    }
    if (!(y <= 0))
    {
        front_components.push_back(make_tuple(-1, 0));
    }
    if (!(y >= (DEFAULT_MAX_HEIGHT - 1)))
    {
        front_components.push_back(make_tuple(1, 0));
    }
    return front_components;
}

bool GameScene::touch_begin(Touch* touch, Event* event)
{
    if (CantDoControl || RetryCantDoControl)
    {
        if (GameRetryButton != NULL) {
            if (GameRetryButton->getBoundingBox().containsPoint(touch->getLocation()))
            {
                Director::getInstance()->replaceScene(GameScene::createScene());
            }
        }
        else
        {
            return false;
        }
    }
    else {
        for (int i = 0; i < Components.size(); i++) {
            for (int j = 0; j < Components[i].size(); j++) {
                if (Components[i][j]->Component->getBoundingBox().containsPoint(touch->getLocation())) {
                    if (Components[i][j]->Bombed == true)
                    {
                        UseBombed = true;
                        use_item(j, i);
                        candocontrol_seter();
                        return false;
                    }
                    else
                    {
                        Clicked = true;
                        ClickTarget = make_tuple(j, i);
                        Components[i][j]->Component->setTexture(ShaderList[Components[i][j]->IconType]);
                    }
                }
            }
        }
    }
    return true;
}

void GameScene::touch_moved(Touch* touch, Event* event)
{
    if (Clicked && Variables->OverSeconds > 0) {

        Variables->DefaultClear();
        removeShowHint();

        int component_x = get<0>(ClickTarget);
        int component_y = get<1>(ClickTarget);

        for (int i = 0; i < Components.size(); i++) {
            for (int j = 0; j < Components[i].size(); j++) {
                if (Components[i][j]->Component->getBoundingBox().containsPoint(touch->getLocation())) {
                    if (!(component_x == j && component_y == i)) {
                        auto direction = decide(j, i, component_x, component_y);
                        ClickDirection = direction;
                        Clicked = false;
                        Components[component_y][component_x]->Component->setTexture(CharacterList[Components[component_y][component_x]->IconType]);
                        canswap(ClickTarget, direction, ActionSwapDeltatime, true);
                    }
                }
            }
        }
    }
}

void GameScene::touch_ended(Touch* touch, Event* event)
{
    if (Clicked)
    {
        int component_x = get<0>(ClickTarget);
        int component_y = get<1>(ClickTarget);

        Components[component_y][component_x]->Component->setTexture(CharacterList[Components[component_y][component_x]->IconType]);
    }
}

void GameScene::touch_event_append()
{
    ClickListener = EventListenerTouchOneByOne::create();
    ClickListener->setSwallowTouches(true);
    ClickListener->onTouchBegan = CC_CALLBACK_2(GameScene::touch_begin, this);
    ClickListener->onTouchMoved = CC_CALLBACK_2(GameScene::touch_moved, this);
    ClickListener->onTouchEnded = CC_CALLBACK_2(GameScene::touch_ended, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(ClickListener, this);
}

tuple<int, int> GameScene::decide(int original_x, int original_y, int moved_x, int moved_y)
{
    int calculated_x = moved_x - original_x;
    int calculated_y = moved_y - original_y;

    if (calculated_x == 0 && calculated_y > 0) {
        return make_tuple(0, -1);
    }
    if (calculated_x == 0 && calculated_y < 0) {
        return make_tuple(0, 1);
    }
    if (calculated_x < 0 && calculated_y >= calculated_x) {
        return make_tuple(1, 0);
    }
    if (calculated_x > 0 && calculated_y <= calculated_x) {
        return make_tuple(-1, 0);
    }
}

bool GameScene::isSamed(tuple<int, int> com1, tuple<int, int> com2, int number)
{
    if (number == 0)
    {
        return get<0>(com1) == get<0>(com2);
    }
    else if (number == 1)
    {
        return get<1>(com1) == get<1>(com2);
    }
}

bool GameScene::tuple_overlapping(vector<tuple<int, int>> com1, vector<tuple<int, int>> com2)
{
    if (com1.size() != com2.size())
    {
        return false;
    }
    else
    {
        for (int i = 0; i < com1.size(); i++)
        {
            bool isSamed = false;
            for (int j = 0; j < com2.size(); j++)
            {
                if (get<0>(com1[i]) == get<0>(com2[j]) && get<1>(com1[i]) == get<1>(com2[j]))
                {
                    isSamed = true;
                }
            }
            if (isSamed == false)
            {
                return false;
            }
        }
    }
    return true;
}

bool GameScene::overlapping(vector<tuple<int, int>> components, int x, int y)
{
    for (int i = 0; i < components.size(); i++) {
        if (get<0>(components[i]) == x && get<1>(components[i]) == y) {
            return true;
        }
    }
    return false;
}