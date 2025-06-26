

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
#include "Commands/SpriteTestCommand.h"
#include "Commands/DamageCommand.h"
#include "Commands/ScoreCommand.h"
#include "Commands/SoundTestCommand.h"


#include "Components/HealthComponent.h"
#include "Components/HealthDisplayComponent.h"
#include "Components/ScoreComponent.h"
#include "Components/ScoreDisplayComponent.h"
#include "Components/PyramidComponent.h"

#include "Entities/QbertComponent.h"
#include "Entities/CoilyComponent.h"


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


    auto font = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 36);
    auto fontSmall = ResourceManager::GetInstance().LoadFont("Fonts/Lingua.otf", 18);

    auto fpsCounter = std::make_shared<GameObject>("Fps Counter");
    fpsCounter->AddComponent<FpsTextComponent>(*fpsCounter, fontSmall);

    SDL_Window* window = Renderer::GetInstance().GetSDLWindow();
    int width, height;
    SDL_GetWindowSize(window, &width, &height);
    fpsCounter->SetWorldLocation({ width, 0, 0 });
    fpsCounter->AddLocation({ 200, 150, 0 });


    scene.Add(fpsCounter);


    auto pyramid = std::make_shared<GameObject>("Pyramid");
    pyramid->AddComponent<Game::PyramidComponent>(*pyramid);
    pyramid->SetWorldLocation({ width / 2, height / 2, 0 });
    //pyramid->SetWorldScale({ 2.f, 2.f });
    auto test = std::make_shared<GameObject>("QbertOfPyramid GameObject");
    //test->AddComponent<Game::QbertComponent>(*test);
    pyramid->AttachChild(test.get(), false, false, false);

    scene.Add(test);
    scene.Add(pyramid);

    auto& keyboard = InputManager::GetInstance().GetKeyboard();

    // pyramid sprite tester (goes through all it's sprites)
    auto spriteTestCommand = std::make_unique<Game::SpriteTesterCommand>(*pyramid);
    keyboard.AddKeyboardCommands(std::move(spriteTestCommand), SDLK_KP_DIVIDE, bae::InputManager::ButtonState::Down);
    spriteTestCommand = std::make_unique<Game::SpriteTesterCommand>(*pyramid);
    keyboard.AddKeyboardCommands(std::move(spriteTestCommand), SDLK_KP_DIVIDE, bae::InputManager::ButtonState::Up);





    auto coily = std::make_shared<GameObject>("Coily1");
    coily->AddComponent<Game::CoilyComponent>(*coily);
    coily->SetWorldLocation({ width / 2, 100, 0 });
    coily->AddLocation({ 100.f, 100.f, 0.f });
    coily->SetWorldScale({ 2.f, 2.f });

    scene.Add(coily);



    /*
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




        // Sound commands
        std::unique_ptr<Game::SoundTestCommand> soundCommand = nullptr;
        soundCommand = std::make_unique<Game::SoundPlayCommand>(Game::Sounds::SoundEvents::Victory);
        keyboard.AddKeyboardCommands(std::move(soundCommand), SDLK_5, bae::InputManager::ButtonState::Down);

        soundCommand = std::make_unique<Game::SoundPauseCommand>(Game::Sounds::SoundEvents::Victory);
        keyboard.AddKeyboardCommands(std::move(soundCommand), SDLK_6, bae::InputManager::ButtonState::Down);

        soundCommand = std::make_unique<Game::SoundResumeCommand>(Game::Sounds::SoundEvents::Victory);
        keyboard.AddKeyboardCommands(std::move(soundCommand), SDLK_7, bae::InputManager::ButtonState::Down);


        // F12
        auto commandSound = std::make_unique<Game::SoundMuteAllSoundsCommand>();
        keyboard.AddKeyboardCommands(std::move(commandSound), SDLK_F12, bae::InputManager::ButtonState::Down);



        // add observers
        if (!player1->GetComponent<Game::HealthComponent>() || !player2->GetComponent<Game::HealthComponent>())
        {
            throw std::runtime_error("Main::Start(), player1 or player2 don't have healthComponent");
        }

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


        scene.Add(player1);
        scene.Add(player2);
    */

    auto soundSystem = &bae::ServiceLocator::GetSoundSystem();
    soundSystem->Play(Game::Sounds::GetSoundId(Game::Sounds::SoundEvents::Tune), 1.f);

}



void LoadSounds()
{
    namespace gs = Game::Sounds;

    bae::ServiceLocator::RegisterSoundSystem(std::make_unique<bae::SdlSoundSystem>());
    auto soundSystem = &bae::ServiceLocator::GetSoundSystem();

    gs::g_sSoundEvents =
    {
        { gs::SoundEvents::BallJump,         soundSystem->LoadSound("Resources/Sounds/Coin.wav") },
        { gs::SoundEvents::CoillyBallJump,   soundSystem->LoadSound("Resources/Sounds/CoilyBallJump.wav") },
        { gs::SoundEvents::CoillyFall,       soundSystem->LoadSound("Resources/Sounds/CoilyFall.wav") },
        { gs::SoundEvents::CoillyJump,       soundSystem->LoadSound("Resources/Sounds/CoilyJump.wav") },
        { gs::SoundEvents::Coin,             soundSystem->LoadSound("Resources/Sounds/Coin.wav") },
        { gs::SoundEvents::Prize,            soundSystem->LoadSound("Resources/Sounds/Prize.wav") },
        { gs::SoundEvents::QbertFall,        soundSystem->LoadSound("Resources/Sounds/QbertFall.wav") },
        { gs::SoundEvents::QbertJump,        soundSystem->LoadSound("Resources/Sounds/QbertJump.wav") },
        { gs::SoundEvents::QbertLift,        soundSystem->LoadSound("Resources/Sounds/QbertLift.wav") },
        { gs::SoundEvents::QbertSpeech1,     soundSystem->LoadSound("Resources/Sounds/QbertSpeech1.wav") },
        { gs::SoundEvents::QbertSpeech2,     soundSystem->LoadSound("Resources/Sounds/QbertSpeech2.wav") },
        { gs::SoundEvents::QbertSpeech3,     soundSystem->LoadSound("Resources/Sounds/QbertSpeech3.wav") },
        { gs::SoundEvents::QbertSpeech4,     soundSystem->LoadSound("Resources/Sounds/QbertSpeech4.wav") },
        { gs::SoundEvents::StartLevel,       soundSystem->LoadSound("Resources/Sounds/StartLevel.wav") },
        { gs::SoundEvents::Tune,             soundSystem->LoadSound("Resources/Sounds/Tune.wav") },
        { gs::SoundEvents::UgglySpeech,      soundSystem->LoadSound("Resources/Sounds/UggySpeech.wav") },
        { gs::SoundEvents::Victory,          soundSystem->LoadSound("Resources/Sounds/Victory.wav") },

    };

}






