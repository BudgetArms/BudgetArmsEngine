#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <filesystem>
#include <iostream>

#include "GameEngine.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "TextObject.h"

namespace fs = std::filesystem;

void Start();

int main(int, char* [])
{

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
        throw std::runtime_error(std::string("Error: Resource Folder Not Found"));
        exit(-1);
    }

#endif

    dae::GameEngine engine(resourcesFolder);
    engine.Run(Start);

    return 0;
}

void Start()
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

    auto go = std::make_shared<dae::GameObject>();
    go->SetTexture("background.tga");
    scene.Add(go);

    go = std::make_shared<dae::GameObject>();
    go->SetTexture("logo.tga");
    go->SetPosition(216, 180);
    scene.Add(go);

    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    auto to = std::make_shared<dae::TextObject>("Programming 4 Assignment", font);
    to->SetPosition(80, 20);
    scene.Add(to);
    std::cout << "test";

}





