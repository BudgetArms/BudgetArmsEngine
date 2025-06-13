

#pragma region Includes

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#include <memory>
#include <cassert>

#include <Windows.h>
#include <filesystem>
#include <iostream>
#include <unordered_map>

#include <glm.hpp>
#include <SDL.h>
#include <imgui.h>
#include <imgui_plot.h>

#ifdef STEAMWORKS_ENABLED
#pragma warning (push)
#pragma warning (disable: 4996)
#include <steam_api.h>
#pragma warning (pop)
#endif


// BudgetArmsEngine Includes
#include "Core/BudgetEngine.h"
#include "Core/GameObject.h"
#include "Core/Renderer.h"
#include "Core/Scene.h"
#include "Core/SoundSystem.h"
#include "Core/SdlSoundSystem.h"
#include "Core/ServiceLocator.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"

#include "Commands/Command.h"
#include "Commands/MoveCommand.h"

#include "Components/Component.h"
#include "Components/FpsCounterComponent.h"
#include "Components/ImguiComponent.h"
#include "Components/TransformComponent.h"
#include "Components/TrashTheCacheComponent.h"

#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"


#ifdef STEAMWORKS_ENABLED

#include "Core/Achievement.h"
#include "Managers/SteamManager.h"
#include "Managers/AchievementManager.h"

#endif

#include "Wrappers/Controller.h"
#include "Wrappers/Keyboard.h"


// Game Includes
#include "Base/SoundEvents.h"
#include "Commands/FireCommand.h"
#include "Commands/DamageCommand.h"
#include "Commands/ScoreCommand.h"

#include "Components/HealthComponent.h"
#include "Components/HealthDisplayComponent.h"
#include "Components/ScoreComponent.h"
#include "Components/ScoreDisplayComponent.h"

#include "Entities/QbertComponent.h"


#pragma endregion



namespace fs = std::filesystem;
using namespace bae;

void Start();

int main(int, char* [])
{
    //thx myself for dying to find this
#ifdef WIN32
    if (AllocConsole())
    {
        FILE* pEmpty;
        freopen_s(&pEmpty, "CONOUT$", "w", stdout);
        freopen_s(&pEmpty, "CONOUT$", "w", stderr);
    }
#endif

#if __EMSCRIPTEN__
    fs::path data_location = "";
#else
    fs::path resourcesFolder = "./Resources/";


    if (!fs::exists(resourcesFolder))
        resourcesFolder = "../Resources/";

    if (!fs::exists(resourcesFolder))
    {
        std::cout << "Resouces Folder Not Found" << '\n';
        assert("Resouces Folder Not Found");
        exit(-1);
    }

#ifdef STEAMWORKS_ENABLED
    SteamManager::GetInstance().Initialize();

#endif

#endif


    BudgetEngine engine(resourcesFolder);
    engine.Run(Start);


#ifdef STEAMWORKS_ENABLED
    SteamManager::GetInstance().Shutdown();
#endif

    return 0;
}


void LoadSounds();


void Start()
{
    auto& scene = SceneManager::GetInstance().CreateScene("Demo");

    LoadSounds();

    // use of go for two variables
    auto go = std::make_shared<GameObject>("Background");
    go->AddComponent<TextureComponent>(*go, "Textures/background.tga");
    scene.Add(go);

    go = std::make_shared<GameObject>("Logo");
    go->AddComponent<TextureComponent>(*go, "Textures/logo.tga");
    go->SetLocalLocation({ 216, 180, 0 });

    auto font = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 36);
    auto fontSmall = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 18);

    auto title = std::make_shared<GameObject>("Title");
    title->AddComponent<TextComponent>(*title, "Programming 4 Assignment", font);
    title->SetLocalLocation({ 20, 10, 0 });

    auto fpsCounter = std::make_shared<GameObject>("Fps Counter");
    fpsCounter->AddComponent<FpsTextComponent>(*fpsCounter, fontSmall);

    SDL_Window* window = Renderer::GetInstance().GetSDLWindow();
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    fpsCounter->SetWorldLocation({ width, 0, 0 });
    fpsCounter->AddLocation({ -75, 0, 0 });


    auto fireCommand = std::make_unique<Game::RotateCommand>(*fpsCounter);
    fireCommand->Execute();


    auto player1 = std::make_shared<GameObject>("Player 1");
    player1->AddComponent<Game::QbertComponent>(*player1);
    player1->SetWorldLocation({ 100, 100, 0 });
    //player1->SetWorldRotation(45.f);
    player1->SetWorldScale({ 2.f, 2.f });


    auto player2 = std::make_shared<GameObject>("Player 2");
    player2->AddComponent<Game::QbertComponent>(*player2);
    player2->SetWorldLocation({ 100, 200, 0 });
    //player2->SetWorldRotation(45.f);
    player2->SetWorldScale({ 2.f, 2.f });


    player1->AddComponent<Game::ScoreComponent>(*player1);
    player2->AddComponent<Game::ScoreComponent>(*player2);


    InputManager::GetInstance().AddController(0);
    InputManager::GetInstance().AddController(1);
    auto* myController = InputManager::GetInstance().GetController(1);
    auto& keyboard = InputManager::GetInstance().GetKeyboard();

    if (!myController)
        throw std::runtime_error("Main: Controller Not Valid");


    // RotateCommand rotates the fps counter 180
    auto coolCommand = std::make_unique<Game::RotateCommand>(*fpsCounter);
    myController->AddControllerCommands(std::move(coolCommand), XINPUT_GAMEPAD_A, InputManager::ButtonState::Down);

    auto coolerCommand = std::make_unique<Game::RotateCommand>(*fpsCounter);
    keyboard.AddKeyboardCommands(std::move(coolerCommand), SDLK_0, InputManager::ButtonState::Down);


    // move player 1 (controller)
    auto moveCommand = std::make_unique<MoveCommand>(*player1, glm::vec2(0, 1), 200.f);
    myController->AddControllerCommands(std::move(moveCommand), XINPUT_GAMEPAD_DPAD_DOWN, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*player1, glm::vec2(0, -1), 200.f);
    myController->AddControllerCommands(std::move(moveCommand), XINPUT_GAMEPAD_DPAD_UP, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*player1, glm::vec2(-1, 0), 200.f);
    myController->AddControllerCommands(std::move(moveCommand), XINPUT_GAMEPAD_DPAD_LEFT, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*player1, glm::vec2(1, 0), 200.f);
    myController->AddControllerCommands(std::move(moveCommand), XINPUT_GAMEPAD_DPAD_RIGHT, InputManager::ButtonState::Pressed);


    // move player 2 (keyboard)
    moveCommand = std::make_unique<MoveCommand>(*player2, glm::vec2(0, -1), 100.f);
    keyboard.AddKeyboardCommands(std::move(moveCommand), SDLK_w, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*player2, glm::vec2(0, 1), 100.f);
    keyboard.AddKeyboardCommands(std::move(moveCommand), SDLK_s, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*player2, glm::vec2(-1, 0), 100.f);
    keyboard.AddKeyboardCommands(std::move(moveCommand), SDLK_a, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*player2, glm::vec2(1, 0), 100.f);
    keyboard.AddKeyboardCommands(std::move(moveCommand), SDLK_d, InputManager::ButtonState::Pressed);



    // player sprite tester (goes through all it's sprites)
    auto spriteTestCommand = std::make_unique<Game::SpriteTesterCommand>(*player1);
    myController->AddControllerCommands(std::move(spriteTestCommand), XINPUT_GAMEPAD_BACK, bae::InputManager::ButtonState::Down);
    spriteTestCommand = std::make_unique<Game::SpriteTesterCommand>(*player1);
    myController->AddControllerCommands(std::move(spriteTestCommand), XINPUT_GAMEPAD_BACK, bae::InputManager::ButtonState::Up);

    spriteTestCommand = std::make_unique<Game::SpriteTesterCommand>(*player2);
    keyboard.AddKeyboardCommands(std::move(spriteTestCommand), SDLK_KP_DIVIDE, bae::InputManager::ButtonState::Down);
    spriteTestCommand = std::make_unique<Game::SpriteTesterCommand>(*player2);
    keyboard.AddKeyboardCommands(std::move(spriteTestCommand), SDLK_KP_DIVIDE, bae::InputManager::ButtonState::Up);


    // damage commands
    auto damageCommand = std::make_unique<Game::DamageCommand>(*player1, 10.f);
    myController->AddControllerCommands(std::move(damageCommand), XINPUT_GAMEPAD_X, bae::InputManager::ButtonState::Down);

    damageCommand = std::make_unique<Game::DamageCommand>(*player1, -10.f);
    myController->AddControllerCommands(std::move(damageCommand), XINPUT_GAMEPAD_Y, bae::InputManager::ButtonState::Down);

    damageCommand = std::make_unique<Game::DamageCommand>(*player2, 3.f);
    keyboard.AddKeyboardCommands(std::move(damageCommand), SDLK_KP_MINUS, bae::InputManager::ButtonState::Down);

    damageCommand = std::make_unique<Game::DamageCommand>(*player2, -3.f);
    keyboard.AddKeyboardCommands(std::move(damageCommand), SDLK_KP_PLUS, bae::InputManager::ButtonState::Down);



    // score commands
    auto scoreCommand = std::make_unique<Game::ScoreCommand>(*player1, -1);
    myController->AddControllerCommands(std::move(scoreCommand), XINPUT_GAMEPAD_LEFT_SHOULDER, bae::InputManager::ButtonState::Down);

    scoreCommand = std::make_unique<Game::ScoreCommand>(*player1, 2);
    myController->AddControllerCommands(std::move(scoreCommand), XINPUT_GAMEPAD_RIGHT_SHOULDER, bae::InputManager::ButtonState::Down);

    scoreCommand = std::make_unique<Game::ScoreCommand>(*player2, -1);
    keyboard.AddKeyboardCommands(std::move(scoreCommand), SDLK_LEFTBRACKET, bae::InputManager::ButtonState::Down);

    scoreCommand = std::make_unique<Game::ScoreCommand>(*player2, 2);
    keyboard.AddKeyboardCommands(std::move(scoreCommand), SDLK_RIGHTBRACKET, bae::InputManager::ButtonState::Down);


    // Health Display
    auto healthDisplayPlayer1 = std::make_shared<GameObject>("HealthDisplay Player 1");
    healthDisplayPlayer1->AddComponent<Game::HealthDisplayComponent>(*healthDisplayPlayer1, font);
    auto* healthComponentPlayer1 = healthDisplayPlayer1->GetComponent<Game::HealthDisplayComponent>();

    auto healthDisplayPlayer2 = std::make_shared<GameObject>("HealthDisplay Player 2");
    healthDisplayPlayer2->AddComponent<Game::HealthDisplayComponent>(*healthDisplayPlayer2, font);
    auto* healthComponentPlayer2 = healthDisplayPlayer2->GetComponent<Game::HealthDisplayComponent>();


    // Score Display
    auto scoreDisplayPlayer1 = std::make_shared<GameObject>("ScoreDisplay Player 1");
    scoreDisplayPlayer1->AddComponent<Game::ScoreDisplayComponent>(*scoreDisplayPlayer1, font);
    auto* scoreDisplayComponentPlayer1 = scoreDisplayPlayer1->GetComponent<Game::ScoreDisplayComponent>();

    auto scoreDisplayPlayer2 = std::make_shared<GameObject>("ScoreDisplay Player 2");
    scoreDisplayPlayer2->AddComponent<Game::ScoreDisplayComponent>(*scoreDisplayPlayer2, font);
    auto* scoreDisplayComponentPlayer2 = scoreDisplayPlayer2->GetComponent<Game::ScoreDisplayComponent>();


    // add observers
    if (!player1->GetComponent<Game::HealthComponent>() || !player2->GetComponent<Game::HealthComponent>())
    {
        throw std::runtime_error("Main::Start(), player1 or player2 don't have healthComponent");
    }

    player1->GetComponent<Game::HealthComponent>()->AddObserver(healthComponentPlayer1);
    player2->GetComponent<Game::HealthComponent>()->AddObserver(healthComponentPlayer2);


    player1->GetComponent<Game::ScoreComponent>()->AddObserver(scoreDisplayComponentPlayer1);
    player2->GetComponent<Game::ScoreComponent>()->AddObserver(scoreDisplayComponentPlayer2);


#ifdef STEAMWORKS_ENABLED

    auto& achievementManager = AchievementManager::GetInstance();

    auto dieAchievement = std::make_unique<Achievement>
        (
            "ACH_TRAVEL_FAR_SINGLE",
            bae::EventType::PLAYER_DIED,
            [](Subject* subject)
            {
                auto* healthComponent = subject->GetGameObject()->GetComponent<Game::HealthComponent>();
                if (!healthComponent)
                    return false;

                return (healthComponent->GetHealth() <= 0);
            }
        );

    auto overPoweredAchievement = std::make_unique<Achievement>
        (
            "ACH_TRAVEL_FAR_ACCUM",
            bae::EventType::PLAYER_HEALTH_CHANGE,
            [](Subject* subject)
            {
                auto* healthComponent = subject->GetGameObject()->GetComponent<Game::HealthComponent>();
                if (!healthComponent)
                    return false;

                return (healthComponent->GetHealth() > 100);
            }
        );

    auto loserAchievement = std::make_unique<Achievement>
        (
            "ACH_WIN_ONE_GAME",
            bae::EventType::PLAYER_SCORE_CHANGE,
            [](Subject* subject)
            {
                auto* scoreComponent = subject->GetGameObject()->GetComponent<Game::ScoreComponent>();
                if (!scoreComponent)
                    return false;

                return (scoreComponent->GetScore() <= 0);
            }
        );

    auto winnerAchievement = std::make_unique<Achievement>
        (
            "ACH_WIN_100_GAMES",
            bae::EventType::PLAYER_SCORE_CHANGE,
            [](Subject* subject)
            {
                auto* scoreComponent = subject->GetGameObject()->GetComponent<Game::ScoreComponent>();
                if (!scoreComponent)
                    return false;

                return (scoreComponent->GetScore() >= 100);
            }
        );


    achievementManager.AddAchievement(std::move(dieAchievement));
    achievementManager.AddAchievement(std::move(overPoweredAchievement));
    achievementManager.AddAchievement(std::move(loserAchievement));
    achievementManager.AddAchievement(std::move(winnerAchievement));


    player1->GetComponent<Game::HealthComponent>()->AddObserver(&achievementManager);
    player2->GetComponent<Game::HealthComponent>()->AddObserver(&achievementManager);

    player1->GetComponent<Game::ScoreComponent>()->AddObserver(&achievementManager);
    player2->GetComponent<Game::ScoreComponent>()->AddObserver(&achievementManager);


#endif


    healthDisplayPlayer1->AddLocation({ 0, 50, 0 });
    scoreDisplayPlayer1->AddLocation({ 0, 80, 0 });

    healthDisplayPlayer2->AddLocation({ 0, 150, 0 });
    scoreDisplayPlayer2->AddLocation({ 0, 180, 0 });


    scene.Add(go);
    scene.Add(title);
    scene.Add(fpsCounter);
    scene.Add(player1);
    scene.Add(player2);
    scene.Add(healthDisplayPlayer1);
    scene.Add(healthDisplayPlayer2);
    scene.Add(scoreDisplayPlayer1);
    scene.Add(scoreDisplayPlayer2);

}



void LoadSounds()
{
    bae::ServiceLocator::RegisterSoundSystem(std::make_unique<bae::SdlSoundSystem>());
    bae::ServiceLocator::RegisterSoundSystem(std::make_unique<bae::SdlSoundSystem>());
    auto soundSystem = &bae::ServiceLocator::GetSoundSystem();
    int audioId = soundSystem->LoadSound("Resources/Sounds/Coin_SFX.wav");

    soundSystem->Play(audioId, 1.f);



    //=
   //{
       //{ Game::SoundEvents::QbertHurt, slocator.LoadSound("Resources/Sounds/Coin_SFX.wav") }

   //};

   /*
   std::vector<std::pair<SoundEvents, int>> SoundEventsPair =
   {
      { SoundEvents::QbertHurt, slocator.LoadSound("Resources/Sounds/Coin_SFX.wav")},
      //{ SoundEvents::QbertJump, slocator.LoadSound("") },
      //{ SoundEvents::Tune, slocator.LoadSound("") },

   };
   */



}


