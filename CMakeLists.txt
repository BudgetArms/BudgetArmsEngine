cmake_minimum_required(VERSION 4.0.0)
project(BudgetEngine)

message("\nBudgetArmsEngine CMake:")

set(ENGINE_NAME BudgetEngine PARENT_SCOPE)
set(ENGINE_NAME BudgetEngine)

if(EMSCRIPTEN)
    message("Using Emscripten!")
else()
    message("NOT Using Emscripten!")
endif()

# Emscripten doesn't allow shared libs
if(EMSCRIPTEN)
    set(BUILD_SHARED_LIBS OFF)
endif()


add_library(${ENGINE_NAME} STATIC
    "Core/BudgetEngine.cpp"
    "Core/EventTypes.h"
    "Core/Event.h"
    "Core/EventQueue.cpp"
    "Core/EventListener.h"
    "Core/GameObject.cpp"
    "Core/HelperFunctions.h"
    "Core/Observer.h"
    "Core/Renderer.cpp"
    "Core/RingBuffer.h"
    "Core/ServiceLocator.cpp"
    "Core/Scene.cpp" "Core/Scene.h"
    "Core/State.h"
    "Core/Subject.cpp"
    "Core/Text2D.cpp"
    "Core/Utils.cpp"

    "Commands/Command.h"
    "Commands/GameActorCommand.h"

    "Components/Component.h"
    "Components/FpsCounterComponent.cpp"
    "Components/ImguiComponent.cpp"
    "Components/SpriteComponent.cpp"
    "Components/TextComponent.cpp"
    "Components/TextureComponent.cpp"
    "Components/TransformComponent.cpp"

    "Graphs/ConnectionCostCalculator.h"
    "Graphs/Graph.cpp"
    "Graphs/GraphConnection.h"
    "Graphs/GraphEnums.h"
    "Graphs/GraphNode.h"
    "Graphs/GraphNodeFactory.h"
    "Graphs/GridGraph.cpp"
    "Graphs/AStar.cpp"
    "Graphs/HeuristicFunctions.h"
    "Graphs/Heuristic.h"

    "Managers/InputManager.cpp"
    "Managers/ResourceManager.cpp"
    "Managers/SceneManager.cpp"

    "Singletons/GameTime.h"
    "Singletons/Singleton.h"

    "Sounds/AudioClip.h"
    "Sounds/LoggingSoundSystem.cpp"
    "Sounds/NullAudioClip.h"
    "Sounds/NullSoundSystem.h"
    "Sounds/SdlAudioClip.cpp"
    "Sounds/SdlSoundSystem.cpp"
    "Sounds/SoundSystem.h"

    "Wrappers/Audio.cpp"
    "Wrappers/AudioTrack.cpp"
    "Wrappers/Controller.cpp"
    "Wrappers/Font.cpp"
    "Wrappers/Keyboard.cpp"
    "Wrappers/Mouse.cpp"
    "Wrappers/Texture2D.cpp"
)


if(STEAMWORKS_ENABLED)
    target_sources(${ENGINE_NAME} PRIVATE
        "Core/Achievement.cpp"
        "Managers/AchievementManager.h"
        "Managers/SteamManager.cpp"
    )
endif()


# enable c++20 features
target_compile_features(${ENGINE_NAME} PUBLIC cxx_std_20)

# Compile at max warning level + treat warnings as errors
if(MSVC)
    target_compile_options(${ENGINE_NAME} PRIVATE /W4 /WX)
else()
    target_compile_options(${ENGINE_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
endif()


if(STEAMWORKS_ENABLED)
    target_compile_definitions(${ENGINE_NAME} PRIVATE
        "STEAMWORKS_ENABLED"
    )
endif()


include(FetchContent)


# Add glm
find_package(glm CONFIG QUIET)
if(glm_FOUND)
    message("\n")
    message("glm package found")
else()
    message("\n")
    message("glm package NOT found, using FetchContent to get it")

    FetchContent_Declare(
        glm
        URL https://github.com/g-truc/glm/releases/download/1.0.3/glm-1.0.3.zip
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )
    FetchContent_MakeAvailable(glm)

endif()


# Add SDL3
find_package(SDL3 3.4 CONFIG QUIET)
if(SDL3_FOUND)
    message("\n")
    message("SDL3 package found")
else()
    message("\n")
    message("SDL3 package NOT found, using FetchContent to get it")

    if(WIN32)
        FetchContent_Declare(
            SDL3
            URL https://www.libsdl.org/release/SDL3-devel-3.4.0-VC.zip
            DOWNLOAD_NO_PROGRESS ON
            DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
        )
        FetchContent_MakeAvailable(SDL3)

        list(PREPEND CMAKE_PREFIX_PATH "${sdl3_SOURCE_DIR}")
        find_package(SDL3 CONFIG REQUIRED)

    else()
        FetchContent_Declare(
            SDL3
            GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
            GIT_TAG release-3.4.0
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
        )
        FetchContent_MakeAvailable(SDL3)

    endif()
endif()


# Add SDL3_ttf
find_package(SDL3_ttf 3.2.2 CONFIG QUIET)
if(SDL3_ttf_FOUND)
    message("\n")
    message("SDL3_ttf package found")
else()
    message("\n")
    message("SDL3_ttf package NOT found, using FetchContent to get it")

    if(WIN32)
        FetchContent_Declare(
            SDL3_ttf
            URL https://www.libsdl.org/projects/SDL_ttf/release/SDL3_ttf-devel-3.2.2-VC.zip
            DOWNLOAD_NO_PROGRESS ON
            DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
        )
        FetchContent_MakeAvailable(SDL3_ttf)

        list(PREPEND CMAKE_PREFIX_PATH "${sdl3_ttf_SOURCE_DIR}")
        find_package(SDL3_ttf CONFIG REQUIRED)

    else()

        FetchContent_Declare(
            SDL3_ttf
            GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
            GIT_TAG release-3.2.2
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
        )

        set(SDL3TTF_INSTALL OFF)
        if(EMSCRIPTEN)
            list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/emscripten")
            set(SDL3TTF_USE_SYSTEM_FREETYPE ON)
            set(SDL3TTF_VENDORED OFF)
            set(SDLTTF_PLUTOSVG OFF)

            add_compile_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
            add_compile_options(-pthread -sPTHREAD_POOL_SIZE=4 -sPROXY_TO_PTHREAD)
            add_compile_options(-fexceptions -sDISABLE_EXCEPTION_CATCHING=0)

            add_link_options(-sUSE_FREETYPE=1 -sUSE_HARFBUZZ=1)
            add_link_options(-pthread -sPTHREAD_POOL_SIZE=4 -sPROXY_TO_PTHREAD)
            add_link_options(-fexceptions -sDISABLE_EXCEPTION_CATCHING=0)
        endif()

        FetchContent_MakeAvailable(SDL3_ttf)

    endif()
endif()


# Add SDL3_mixer
find_package(SDL3_mixer CONFIG QUIET)
if(SDL3_mixer_FOUND)
    message("\n")
    message("SDL3_mixer package found")
else()
    message("\n")
    message("SDL3_mixer package NOT found, using FetchContent to get it")

    if(WIN32)
        FetchContent_Declare(
            SDL3_mixer
            URL https://www.libsdl.org/projects/SDL_mixer/release/SDL3_mixer-devel-3.1.2-VC.zip
            DOWNLOAD_NO_PROGRESS ON
            DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
        )
        FetchContent_MakeAvailable(SDL3_mixer)

        list(PREPEND CMAKE_PREFIX_PATH "${sdl3_mixer_SOURCE_DIR}")
        find_package(SDL3_mixer CONFIG REQUIRED)

    else()
        FetchContent_Declare(
            SDL3_mixer
            GIT_REPOSITORY "https://github.com/libsdl-org/SDL_mixer.git"
            GIT_TAG "prerelease-3.1.2"
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
        )
        FetchContent_MakeAvailable(SDL3_mixer)

    endif()
endif()


# Add SDL3_gfx
find_package(SDL3_gfx CONFIG QUIET)
if(SDL3_gfx_FOUND)
    message("\n")
    message("SDL3_gfx package found")
else()
    message("\n")
    message("SDL3_gfx package NOT found, using FetchContent to get it")

    # The CMake of the SDL3_gfx library uses find_package to find an SDL3, but if it's not found, it searches,
    # for the SDL which it fails at, so I set the SDL3_LIB (dummy) here, because otherwise the SDL3_gfx CMake fails
    set(SDL3_LIB "${CMAKE_BINARY_DIR}/dummy_sdl3.lib" CACHE FILEPATH "Dummy SDL3 lib for SDL3_gfx" FORCE)

    FetchContent_Declare(
        SDL3_gfx
        GIT_REPOSITORY "https://github.com/sabdul-khabir/SDL3_gfx.git"
        GIT_TAG "v1.0.1"
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(SDL3_gfx)

    if(TARGET SDL3_gfx_Shared)
        message(STATUS "Disabling SDL3_gfx_Shared target")
        set_target_properties(SDL3_gfx_Shared PROPERTIES
            EXCLUDE_FROM_ALL ON
            EXCLUDE_FROM_DEFAULT_BUILD ON
        )
    endif()
endif()


# Add imgui
find_package(imgui CONFIG QUIET)
if(imgui_FOUND)
    message("\n")
    message("imgui package found")
else()
    message("\n")
    message("imgui package NOT found, using FetchContent to get it")

    FetchContent_Declare(
        imgui
        GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
        GIT_TAG "v1.92.6"
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(imgui)

    add_library(imgui STATIC
        ${imgui_SOURCE_DIR}/imconfig.h
        ${imgui_SOURCE_DIR}/imgui.cpp
        ${imgui_SOURCE_DIR}/imgui.h
        ${imgui_SOURCE_DIR}/imgui_demo.cpp
        ${imgui_SOURCE_DIR}/imgui_draw.cpp
        ${imgui_SOURCE_DIR}/imgui_internal.h
        ${imgui_SOURCE_DIR}/imgui_tables.cpp
        ${imgui_SOURCE_DIR}/imgui_widgets.cpp
        ${imgui_SOURCE_DIR}/imstb_rectpack.h
        ${imgui_SOURCE_DIR}/imstb_truetype.h
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdlrenderer3.cpp
    )

endif()


# Add imgui-plot
find_package(imgui_plot CONFIG QUIET)
if(imgui_plot_FOUND)
    message("\n")
    message("imgui_plot package found")
else()
    message("\n")
    message("imgui_plot package NOT found, using FetchContent to get it")

    FetchContent_Declare(
        imgui_plot
        GIT_REPOSITORY "https://github.com/soulthreads/imgui-plot"
        GIT_TAG "master"
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(imgui_plot)

endif()


# Add nlohmann_json
find_package(nlohmann_json 3.11.3 CONFIG QUIET)
if(nlohmann_json_FOUND)
    message("\n")
    message("nlohmann_json package found")
else()
    message("\n")
    message("nlohmann_json package NOT found, using FetchContent to get it")

    FetchContent_Declare(
        nlohmann_json
        GIT_REPOSITORY "https://github.com/nlohmann/json.git"
        GIT_TAG "v3.11.3"
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(nlohmann_json)

endif()

# Add VLD
if(WIN32)
    find_package(VLD CONFIG)
    if(VLD_FOUND)
        message("\n")
        message("VLD package found")
    else()
        message("\n")
        message("VLD package NOT found, using FetchContent to get it")

        # Get the package from github
        FetchContent_Declare(
            vld
            GIT_REPOSITORY "https://github.com/avadae/vld.git"
            GIT_TAG "v2.5.12"
            GIT_SHALLOW TRUE
            GIT_PROGRESS TRUE
        )
        FetchContent_MakeAvailable(vld)

    endif()
endif()


# Add XInput
set(XINPUT_LIB XInput)


# Include directories
target_include_directories(SDL3_gfx_Static PUBLIC
    SDL3::SDL3
)

target_include_directories(imgui PUBLIC
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    SDL3::SDL3
)

target_include_directories(${ENGINE_NAME} PUBLIC
    glm::glm

    ${sdl3_gfx_SOURCE_DIR}
    ${VLD_INCLUDE_DIR}
)


# Link Libraries
target_link_libraries(imgui PUBLIC
    SDL3::SDL3
)

target_link_libraries(imgui_plot PUBLIC
    imgui
)

target_link_libraries(SDL3_gfx_Static PUBLIC
    SDL3::SDL3
)

# Link libraries to Engine
target_link_libraries(${ENGINE_NAME} PUBLIC
    glm::glm
    SDL3::SDL3
    SDL3_ttf::SDL3_ttf
    SDL3_mixer::SDL3_mixer

    SDL3_gfx_Static

    imgui
    imgui_plot
    nlohmann_json::nlohmann_json
)

if(WIN32)
    target_link_libraries(${ENGINE_NAME} PUBLIC
        ${XINPUT_LIB}
        ${VLD_LIBRARIES}
    )
endif()

if(STEAMWORKS_ENABLED)
    target_link_libraries(${ENGINE_NAME} PUBLIC
        Steamworks::Steamworks
    )
endif()


target_include_directories(${ENGINE_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${ENGINE_NAME} PRIVATE
    glm::glm
)

# Post-Build Commands
if(WIN32)

    set(SDL_RUNTIME_DLLS
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_mixer::SDL3_mixer
        SDL3_gfx_Static
    )

    foreach(dllTarget IN LISTS SDL_RUNTIME_DLLS)
        message("${ddlTarget}")
        add_custom_command(
            TARGET ${ENGINE_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:${dllTarget}>"
            "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}"
            VERBATIM
        )
    endforeach()


    if(STEAMWORKS_ENABLED)

        # Copy Steamworks dll
        add_custom_command(
            TARGET ${ENGINE_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy
            "${Steamworks_RUNTIME_DLL}"
            "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}/"
        )

        # Create steam_appid file
        add_custom_command(
            TARGET ${ENGINE_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E
            echo "480" > "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}/steam_appid.txt"
            COMMENT "Generating steam_appid.txt"
        )

    endif()


    # Copy Resource folder
    add_custom_command(
        TARGET ${ENGINE_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory
        "${CMAKE_SOURCE_DIR}/${RESOURCE_FOLDER_NAME}"
        "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}/${RESOURCE_FOLDER_NAME}"
    )

endif()


