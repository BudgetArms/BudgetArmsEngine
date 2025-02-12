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


void load();

int main(int, char*[])
{

#if __EMSCRIPTEN__
    fs::path data_location = "";
#else
    fs::path data_location = "./Resources/";
    if (!fs::exists(data_location))
        data_location = "../Resources/";
#endif
    dae::GameEngine engine(data_location);
    engine.Run(load);
    return 0;
}

enum class test {
    nice,
    cool,
    fire
};

test g_Testing;

void load()
{
    auto& scene = dae::SceneManager::GetInstance().CreateScene("Demo");

    if (true)
        std::cout << "test" << std::endl;
    // fjksf

    switch (g_Testing) {
    case test::nice: {
        std::cout << "nice" << std::endl;
    } break;
    case test::cool: {
        std::cout << "cool" << std::endl;
    } break;
    case test::fire: {
        std::cout << "fire" << std::endl;
    } break;
    default: {
        std::cout << "default" << std::endl;
    } break;
    }

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
}
