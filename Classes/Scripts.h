#include <tuple>
#include <vector>

using namespace std;

#define DEFAULT_MAX_WIDTH 7
#define DEFAULT_MAX_HEIGHT 7

#define INTERVAL_STEP_FORWARD_NUMBER 7
#define INTERVAL_NUMBER 70
#define INTERVAL_PARTICLE_STEP_FORWARD_NUMBER 30

vector<const char*> CharacterList = {
        "Cat.png",
        "Chick.png",
        "Dog.png",
        "Monkey.png",
        "Mouse.png",
        "Pig.png",
        "Rabbit.png"
};
vector<const char*> ShaderList = {
    "Shaders/CatTouch.png",
    "Shaders/ChickTouch.png",
    "Shaders/DogTouch.png",
    "Shaders/MonkeyTouch.png",
    "Shaders/MouseTouch.png",
    "Shaders/PigTouch.png",
    "Shaders/RabbitTouch.png"
};

vector<const char*> Levels = {
    "Level1.png",
    "Level2.png"
};

const char* BackgroundMusic = "Sounds/BackgroundMusic.mp3";
const char* PangEffect = "Sounds/PangEffect.wav";

const char* UIUnderUI = "UI/UnderUI.png";
const char* UIUpUI = "UI/UpUI.png";
const char* UIScoreBar = "UI/UnderBarUI.png";
const char* UIProgressBar = "UI/UnderBarUI2.png";
const char* UITimeLimitBarTTF = "Fonts/SafirSoft-Vagron.ttf";
const char* UIScoreTTF = "Fonts/Jalnan.ttf";
const char* UIComboTTF = "Fonts/CookieRun Black.ttf";
const char* GameReplaceTTF = "Fonts/MaruBuri-Bold.ttf";
const char* AudioFileBackGround = "Sounds/anipang.mp3";
const char* AudioFilePangEffect = "Sounds/anipang_pang.wav";
const char* CharacterPangParticle = "Particles/Boom.plist";
const char* TimeOut = "GameOver";
const char* AssetsLevel1Bomb = "Level1.png";
const char* AssetsLevel2Bomb = "Level2.png";
const char* AssetsShowHint = "Hint.png";

const char* UIGameOver = "UI/GameOverUI.png";
const char* UIReStartButton = "UI/ReStartButton.png";

float ActionSwapDeltatime = 0.12f;
float ActionDownDeltatime = 0.2f;
float ActionDelayTime = 0.1f;
float ActionParticleAfterDeltatime = 0.15f;
float ActionShowHintDeltaTime = 0.35f;