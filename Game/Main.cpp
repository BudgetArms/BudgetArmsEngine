//#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif


#include <glm.hpp>
#include <SDL.h>


#include <memory>
#include <cassert>

#include <filesystem>
#include <iostream>

#include "Core/BudgetEngine.h"
#include "Core/GameObject.h"
#include "Core/Scene.h"
#include "Components/TextComponent.h"
#include "Components/TextureComponent.h"
#include "Core/Renderer.h"

#include "Components/Component.h"
#include "Components/FpsCounterComponent.h"
#include "Components/TransformComponent.h"

#include "Managers/ResourceManager.h"
#include "Managers/SceneManager.h"


#include <Windows.h>

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
    go->AddLocation({ 100, 0, 0 });
    go->AddRotation(180.f);

    auto goMirror = std::make_shared<GameObject>("Logo Mirrored");
    goMirror->AddComponent<TextureComponent>(*goMirror, "logo.tga");

    goMirror->SetWorldRotation(0.f);
    goMirror->SetWorldScale({ -1, 1 });

    go->AttachChild(goMirror.get(), false, true, true);


    auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    auto title = std::make_shared<GameObject>("Title");
    title->AddComponent<TextComponent>(*title, "Programming 4 Assignment", font);
    title->SetLocalLocation({ 20, 10, 0 });

    auto specialText = std::make_shared<GameObject>("special Text");
    specialText->AddComponent<TextComponent>(*specialText, "FPS Component =", font);
    specialText->SetLocalLocation({ 0, 50, 0 });

    auto fpsCounter = std::make_shared<GameObject>("Fps Counter");
    fpsCounter->AddComponent<FpsTextComponent>(*fpsCounter, font);
    fpsCounter->SetLocalLocation({ 300, 0, 0 });

    title->AttachChild(dynamic_cast<GameObject*>(specialText.get()), false);
    specialText->AttachChild(dynamic_cast<GameObject*>(fpsCounter.get()), false);


    specialText->AddLocation({ 0.f, 30.f, 0.f });

    specialText->SetLocalRotation(-45.f);
    fpsCounter->AddRotation(45.f);

    specialText->SetLocalScale({ 1.f, 1.4f });
    fpsCounter->SetLocalScale({ 0.5f, 1.f });


    scene.Add(go);
    scene.Add(goMirror);
    scene.Add(title);
    scene.Add(specialText);
    scene.Add(fpsCounter);


}


