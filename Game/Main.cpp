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
    auto moveCommand = std::make_unique<MoveCommand>(*fpsCounter, glm::vec2(0, -1));
    keyboard.AddKeyboardCommands(std::move(moveCommand), SDLK_w, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*fpsCounter, glm::vec2(0, 1));
    keyboard.AddKeyboardCommands(std::move(moveCommand), SDLK_s, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*fpsCounter, glm::vec2(-1, 0));
    keyboard.AddKeyboardCommands(std::move(moveCommand), SDLK_a, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*fpsCounter, glm::vec2(1, 0));
    keyboard.AddKeyboardCommands(std::move(moveCommand), SDLK_d, InputManager::ButtonState::Pressed);



    // move logo with dpad (does not seem to work??)
    moveCommand = std::make_unique<MoveCommand>(*go, glm::vec2(0, 1), 200.f);
    myController->AddControllerCommands(std::move(moveCommand), XINPUT_GAMEPAD_DPAD_DOWN, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*go, glm::vec2(0, -1), 200.f);
    myController->AddControllerCommands(std::move(moveCommand), XINPUT_GAMEPAD_DPAD_UP, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*go, glm::vec2(-1, 0), 200.f);
    myController->AddControllerCommands(std::move(moveCommand), XINPUT_GAMEPAD_DPAD_LEFT, InputManager::ButtonState::Pressed);

    moveCommand = std::make_unique<MoveCommand>(*go, glm::vec2(1, 0), 200.f);
    myController->AddControllerCommands(std::move(moveCommand), XINPUT_GAMEPAD_DPAD_RIGHT, InputManager::ButtonState::Pressed);


    printCommand = std::make_unique<PrintCommand>("LSHOULDER");
    myController->AddControllerCommands(std::move(printCommand), XINPUT_GAMEPAD_LEFT_SHOULDER, InputManager::ButtonState::Up);

    printCommand = std::make_unique<PrintCommand>("RSHOULDER");
    myController->AddControllerCommands(std::move(printCommand), XINPUT_GAMEPAD_RIGHT_SHOULDER, InputManager::ButtonState::Up);

    printCommand = std::make_unique<PrintCommand>("Button A");
    myController->AddControllerCommands(std::move(printCommand), XINPUT_GAMEPAD_A, InputManager::ButtonState::Up);

    printCommand = std::make_unique<PrintCommand>("Button B");
    myController->AddControllerCommands(std::move(printCommand), XINPUT_GAMEPAD_B, InputManager::ButtonState::Up);

    printCommand = std::make_unique<PrintCommand>("Button X");
    myController->AddControllerCommands(std::move(printCommand), XINPUT_GAMEPAD_X, InputManager::ButtonState::Up);

    printCommand = std::make_unique<PrintCommand>("Button Y");
    myController->AddControllerCommands(std::move(printCommand), XINPUT_GAMEPAD_Y, InputManager::ButtonState::Up);

    printCommand = std::make_unique<PrintCommand>("Right Thumb");
    myController->AddControllerCommands(std::move(printCommand), XINPUT_GAMEPAD_RIGHT_THUMB, InputManager::ButtonState::Up);

    printCommand = std::make_unique<PrintCommand>("Left Thumb");
    myController->AddControllerCommands(std::move(printCommand), XINPUT_GAMEPAD_LEFT_THUMB, InputManager::ButtonState::Up);

    printCommand = std::make_unique<PrintCommand>("Share Button");
    myController->AddControllerCommands(std::move(printCommand), XINPUT_GAMEPAD_BACK, InputManager::ButtonState::Up);

    printCommand = std::make_unique<PrintCommand>("Options Button");
    myController->AddControllerCommands(std::move(printCommand), XINPUT_GAMEPAD_TRIGGER_THRESHOLD, InputManager::ButtonState::Up);

    printCommand = std::make_unique<PrintCommand>("Start Button");
    myController->AddControllerCommands(std::move(printCommand), XINPUT_GAMEPAD_START, InputManager::ButtonState::Up);



    Game::Qbert test{ "yes" };
    test.AddComponent<Game::HealthComponent>(test, 100.f);
    test.Destroy();


    scene.Add(go);
    scene.Add(title);
    scene.Add(fpsCounter);

}
