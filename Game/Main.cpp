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
#include "Managers/ResourceManager.h"
#include "Core/Scene.h"
#include "Managers/SceneManager.h"
#include "Core/TextObject.h"

#include "Core/Renderer.h"
#include <Windows.h>

namespace fs = std::filesystem;

void Start();

int main(int, char* [])
{
    //bool bae::GameObject::m_bProjectClosing = true;
    atexit(bae::GameObject::SetProjectClosing);

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

    bae::BudgetEngine engine(resourcesFolder);
    engine.Run(Start);

    return 0;
}


void Start()
{
    auto& scene = bae::SceneManager::GetInstance().CreateScene("Demo");

    auto go = std::make_shared<bae::GameObject>();
    go->SetTexture("background.tga");
    scene.Add(go);

    go = std::make_shared<bae::GameObject>();
    go->SetTexture("logo.tga");
    go->SetPosition(216, 180);
    scene.Add(go);

    auto font = bae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    auto to = std::make_shared<bae::TextObject>("Programming 4 Assignment", font);
    to->SetPosition(80, 20);
    scene.Add(to);

    //auto textObject = std::make_shared<bae::TextObject>("Fixing ECS/Scenegraph", font);
    //to->SetPosition(80, 50);
    //scene.Add(to);

}







