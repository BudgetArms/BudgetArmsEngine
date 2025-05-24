//#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#include <memory>
#include <cassert>

#include <Windows.h>
#include <filesystem>
#include <iostream>

#include <glm.hpp>
#include <SDL.h>
#include <imgui.h>
#include <imgui_plot.h>


// BudgetArmsEngine Includes
#include "Core/BudgetEngine.h"
#include "Core/GameObject.h"
#include "Core/Scene.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Core/Renderer.h"

#include "Commands/Command.h"
#include "Commands/FireCommand.h"
#include "Commands/MoveCommand.h"

#include "Components/Component.h"
#include "Components/ImguiComponent.h"
#include "Components/FpsCounterComponent.h"
#include "Components/TransformComponent.h"
#include "Components/TrashTheCacheComponent.h"

#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"

#include "Wrappers/Controller.h"
#include "Wrappers/Keyboard.h"

// Game Includes
#include "Entities/Qbert.h"
#include "Components/HealthComponent.h"


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

#endif

    BudgetEngine engine(resourcesFolder);
    engine.Run(Start);

    return 0;
}


void Start()
{
    auto& scene = SceneManager::GetInstance().CreateScene("Demo");

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


    auto fireCommand = std::make_unique<FireCommand>(*fpsCounter);
    fireCommand->Execute();


    auto player = std::make_shared<Game::Qbert>("player");
    player->SetWorldLocation({ 100, 100, 0 });
    //player->SetWorldRotation(45.f);
    player->SetWorldScale({ 2.f, 2.f });



    auto player2 = std::make_shared<Game::Qbert>("player2");
    player2->SetWorldLocation({ 100, 200, 0 });
    //player->SetWorldRotation(45.f);
    player2->SetWorldScale({ 2.f, 2.f });



    InputManager::GetInstance().AddController(0);
    InputManager::GetInstance().AddController(1);
    auto* myController = InputManager::GetInstance().GetController(1);
    auto& keyboard = InputManager::GetInstance().GetKeyboard();

    // works :D
    auto coolCommand = std::make_unique<FireCommand>(*fpsCounter);
    myController->AddControllerCommands(std::move(coolCommand), XINPUT_GAMEPAD_A, InputManager::ButtonState::Down);
    //coolCommand = nullptr;

    // works, FireCommands rotates the fps counter 180, and logs something
    auto coolerCommand = std::make_unique<FireCommand>(*fpsCounter);
    keyboard.AddKeyboardCommands(std::move(coolerCommand), SDLK_0, InputManager::ButtonState::Pressed);

    // also works :D
    auto printCommand = std::make_unique<PrintCommand>();
    keyboard.AddKeyboardCommands(std::move(printCommand), SDLK_p, InputManager::ButtonState::Down);


    // move fps counter with wasd
    auto moveCommand = std::make_unique<MoveCommand>(*player2, glm::vec2(0, -1), 100.f);
    keyboard.AddKeyboardCommands(std::move(moveCommand), SDLK_w, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*player2, glm::vec2(0, 1), 100.f);
    keyboard.AddKeyboardCommands(std::move(moveCommand), SDLK_s, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*player2, glm::vec2(-1, 0), 100.f);
    keyboard.AddKeyboardCommands(std::move(moveCommand), SDLK_a, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*player2, glm::vec2(1, 0), 100.f);
    keyboard.AddKeyboardCommands(std::move(moveCommand), SDLK_d, InputManager::ButtonState::Pressed);



    // move logo with dpad (does not seem to work??)
    moveCommand = std::make_unique<MoveCommand>(*player, glm::vec2(0, 1), 200.f);
    myController->AddControllerCommands(std::move(moveCommand), XINPUT_GAMEPAD_DPAD_DOWN, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*player, glm::vec2(0, -1), 200.f);
    myController->AddControllerCommands(std::move(moveCommand), XINPUT_GAMEPAD_DPAD_UP, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*player, glm::vec2(-1, 0), 200.f);
    myController->AddControllerCommands(std::move(moveCommand), XINPUT_GAMEPAD_DPAD_LEFT, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*player, glm::vec2(1, 0), 200.f);
    myController->AddControllerCommands(std::move(moveCommand), XINPUT_GAMEPAD_DPAD_RIGHT, InputManager::ButtonState::Pressed);


    auto spriteTestCommand = std::make_unique<bae::SpriteTesterCommand>(*player);
    keyboard.AddKeyboardCommands(std::move(spriteTestCommand), SDLK_KP_DIVIDE, bae::InputManager::ButtonState::Down);
    spriteTestCommand = std::make_unique<bae::SpriteTesterCommand>(*player);
    keyboard.AddKeyboardCommands(std::move(spriteTestCommand), SDLK_KP_DIVIDE, bae::InputManager::ButtonState::Up);



    scene.Add(go);
    scene.Add(title);
    scene.Add(fpsCounter);
    scene.Add(player);
    scene.Add(player2);

}
