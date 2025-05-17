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

#include "Core/BudgetEngine.h"
#include "Core/GameObject.h"
#include "Core/Scene.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Core/Renderer.h"

#include "Commands/Command.h"
#include "Commands/FireCommand.h"

#include "Components/Component.h"
#include "Components/ImguiComponent.h"
#include "Components/FpsCounterComponent.h"
#include "Components/TransformComponent.h"
#include "Components/TrashTheCacheComponent.h"

#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"

#include "Wrappers/Controller.h"
#include "Wrappers/Keyboard.h"


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
    go->AddComponent<TextureComponent>(*go, "background.tga");
    scene.Add(go);

    go = std::make_shared<GameObject>("Logo");
    go->AddComponent<TextureComponent>(*go, "logo.tga");
    go->SetLocalLocation({ 216, 180, 0 });

    auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    auto fontSmall = ResourceManager::GetInstance().LoadFont("Lingua.otf", 18);

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


    Controller myController{ 0 };
    bool isPressed = myController.IsButtonPressed(XINPUT_GAMEPAD_A);
    if (isPressed)
        std::cout << "W\n";
    else
        std::cout << "L\n";


    InputManager::GetInstance().AddController(0);
    auto* myOwnController = InputManager::GetInstance().GetController(0);
    auto& keyboard = InputManager::GetInstance().GetKeyboard();

    // maybe it works, can't test atm, don't have controller rn
    auto coolCommand = std::make_unique<FireCommand>(*fpsCounter);
    myOwnController->AddControllerCommands(std::move(coolCommand), XINPUT_GAMEPAD_A, InputManager::ButtonState::Down);

    // works, FireCommands rotates the fps counter 180, and logs something
    auto coolerCommand = std::make_unique<FireCommand>(*fpsCounter);
    keyboard.AddKeyboardCommands(std::move(coolerCommand), SDLK_0, InputManager::ButtonState::Pressed);

    // also works :D
    auto printCommand = std::make_unique<PrintCommand>();
    keyboard.AddKeyboardCommands(std::move(printCommand), SDLK_p, InputManager::ButtonState::Down);



    scene.Add(go);
    scene.Add(title);
    scene.Add(fpsCounter);

}


