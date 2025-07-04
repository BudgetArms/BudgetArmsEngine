﻿cmake_minimum_required(VERSION 3.26)
project(BudgetEngine)

message("\nBudgetArmsEngine CMake")


if(EMSCRIPTEN)
    message("Using Emscripten!")
    set(ENGINE_NAME BudgetEngine_web PARENT_SCOPE)
    set(ENGINE_NAME BudgetEngine_web)
    set(ENGINE_PARAM )
else()
    set(ENGINE_NAME BudgetEngine PARENT_SCOPE)
    set(ENGINE_NAME BudgetEngine)

    if(ENABLE_CONSOLE)
        set(ENGINE_PARAM )
    else()
        set(ENGINE_PARAM WIN32)
    endif()

endif()


add_library(${ENGINE_NAME} STATIC
    "Core/BudgetEngine.cpp"                     "Core/BudgetEngine.h"
    "Core/AudioClip.h"
    "Core/SdlAudioClip.cpp"                     "Core/SdlAudioClip.h"
    "Core/GameObject.cpp"                       "Core/GameObject.h"
    "Core/EventTypes.h"
    "Core/Event.h"
    "Core/EventQueue.h"                         "Core/EventQueue.cpp"
    "Core/EventListener.h"
    "Core/Subject.h"
    "Core/SoundSystem.h"
    "Core/NullSoundSystem.h"
    "Core/SdlSoundSystem.h"                     "Core/SdlSoundSystem.cpp"
    "Core/LoggingSoundSystem.cpp"               "Core/LoggingSoundSystem.h"                 
    "Core/ServiceLocator.cpp"                   "Core/ServiceLocator.h"
    "Core/Observer.h"
    "Core/Renderer.cpp"                         "Core/Renderer.h"
    "Core/Scene.cpp"                            "Core/Scene.h"
    "Commands/Command.h"
    "Commands/GameActorCommand.h"
    "Commands/MoveCommand.h"
    "Components/TextComponent.cpp"              "Components/TextComponent.h"
    "Components/Component.h"
    "Components/FpsCounterComponent.cpp"        "Components/FpsCounterComponent.h"
    "Components/ImguiComponent.cpp"             "Components/ImguiComponent.h"
    "Components/InputComponent.h"
    "Components/SpriteComponent.cpp"            "Components/SpriteComponent.h"
    "Components/TextureComponent.cpp"           "Components/TextureComponent.h"
    "Components/TrashTheCacheComponent.cpp"     "Components/TrashTheCacheComponent.h"
    "Components/TransformComponent.cpp"         "Components/TransformComponent.h"
    "Managers/InputManager.cpp"                 "Managers/InputManager.h"
    "Managers/ResourceManager.cpp"              "Managers/ResourceManager.h"
    "Managers/SceneManager.cpp"                 "Managers/SceneManager.h"
    "Singletons/GameTime.h"
    "Singletons/Singleton.h"
    "Wrappers/AudioChunk.cpp"                   "Wrappers/AudioChunk.h"
    "Wrappers/Controller.cpp"                   "Wrappers/Controller.h"
    "Wrappers/Keyboard.cpp"                     "Wrappers/Controller.h"
    "Wrappers/Font.cpp"                         "Wrappers/Font.h"
    "Wrappers/Texture2D.cpp"                    "Wrappers/Texture2D.h"
 )
 


if(STEAMWORKS_AVAILABLE)

    target_sources(${ENGINE_NAME} PRIVATE
        "Core/Achievement.cpp"     "Core/Achievement.h"
        "Managers/AchievementManager.h"
        "Managers/SteamManager.cpp"     "Managers/SteamManager.h"
    )

endif()


 # enable c++20 features
target_compile_features(${ENGINE_NAME} PUBLIC cxx_std_20)



include(FetchContent)
# add glm
FetchContent_Declare(
    glm
    URL https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.zip
    DOWNLOAD_NO_PROGRESS ON
    DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
)

FetchContent_GetProperties(glm)

if(NOT glm_POPULATED)
    FetchContent_MakeAvailable(glm)
endif()


# when I have time, I want to remove the fetching of the precompiled binary files
# since then it's code-wise less hard on the eyes and more intuative, 
# and then it's also cross-platform. But then I will need another type of input manager
# since I get XInput straight from Window's api.
if (WIN32)

    FetchContent_Declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG "docking" 
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
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
        ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    )


    FetchContent_Declare(
        imgui_plot
        GIT_REPOSITORY https://github.com/soulthreads/imgui-plot
        GIT_TAG "master"
    )

    FetchContent_MakeAvailable(imgui_plot)

    target_include_directories(imgui_plot PUBLIC
      ${imgui_plot_SOURCE_DIR}/include/imgui_plot
    )

    # Link Imgui Plot to ImGui
    target_link_libraries(imgui_plot PUBLIC imgui)

    # Link xinput to project (windows only bc can't fetch xinput)
    #target_link_libraries(${ENGINE_NAME} PUBLIC XInput)


    if(STEAMWORKS_AVAILABLE AND EXISTS "${STEAMWORKS_SDK_PATH}")

        set(STEAMWORKS_INCLUDE_DIR "${STEAMWORKS_SDK_PATH}/public/steam")

        if(CMAKE_SIZEOF_VOID_P EQUAL 8)
            set(STEAMWORKS_LIBRARY "${STEAMWORKS_SDK_PATH}/redistributable_bin/win64/steam_api64.lib")
            set(STEAMWORKS_LIBRARY_DLL "${STEAMWORKS_SDK_PATH}/redistributable_bin/win64/steam_api64.dll")
        else()
            set(STEAMWORKS_LIBRARY "${STEAMWORKS_SDK_PATH}/redistributable_bin/steam_api.lib")
            set(STEAMWORKS_LIBRARY_DLL "${STEAMWORKS_SDK_PATH}/redistributable_bin/steam_api.dll")
        endif()

        if(EXISTS "${STEAMWORKS_LIBRARY}")
            message(STATUS "Steamworks library found: ${STEAMWORKS_LIBRARY}")
        else()
            message(WARNING "Steamworks library not found: ${STEAMWORKS_LIBRARY}")
            set(STEAMWORKS_AVAILABLE FALSE)
        endif()

    endif()



    # add SDL2
    FetchContent_Declare(
        SDL2
        URL https://www.libsdl.org/release/SDL2-devel-2.32.8-VC.zip 
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads 
    )

    FetchContent_GetProperties(SDL2)

    if(NOT SDL2_POPULATED)
        FetchContent_MakeAvailable(SDL2)
        set(SDL2_INCLUDE_DIR ${sdl2_SOURCE_DIR}/include)

    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x64/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x64/SDL2main.lib")
        set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x64/SDL2.dll")
    else()
        set(SDL2_LIBRARIES "${sdl2_SOURCE_DIR}/lib/x86/SDL2.lib;${sdl2_SOURCE_DIR}/lib/x86/SDL2main.lib")
        set(SDL2_LIBRARY_DLL "${sdl2_SOURCE_DIR}/lib/x86/SDL2.dll")
    endif()

    endif()

    # add SDL2_image
    FetchContent_Declare(
        SDL2_image
        URL https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.8.8-VC.zip 
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )

    FetchContent_GetProperties(SDL2_image)

    if(NOT SDL2_image_POPULATED)
        FetchContent_MakeAvailable(SDL2_image)
        set(SDL2_IMAGE_INCLUDE_DIR ${sdl2_image_SOURCE_DIR}/include)

    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.lib")
        set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x64/SDL2_image.dll")
    else()
        set(SDL2_IMAGE_LIBRARIES "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.lib")
        set(SDL2_IMAGE_LIBRARY_DLL "${sdl2_image_SOURCE_DIR}/lib/x86/SDL2_image.dll")
    endif()

    endif()

    # add SDL2_ttf
    FetchContent_Declare(
        SDL2_ttf
        #URL https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.24.0-VC.zip
        URL https://www.libsdl.org/projects/SDL_ttf/release/SDL2_ttf-devel-2.22.0-VC.zip
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )

    FetchContent_GetProperties(SDL2_ttf)

    if(NOT sdl2_ttf_POPULATED)
        FetchContent_MakeAvailable(SDL2_ttf)
        set(SDL2_TTF_INCLUDE_DIR ${sdl2_ttf_SOURCE_DIR}/include)

    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.lib)
        set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x64/SDL2_ttf.dll")
    else()
        set(SDL2_TTF_LIBRARIES ${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.lib)
        set(SDL2_TTF_LIBRARY_DLL "${sdl2_ttf_SOURCE_DIR}/lib/x86/SDL2_ttf.dll")
    endif()

    endif() 

    # add SDL2_mixer
    FetchContent_Declare(
        SDL2_mixer
        URL https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.8.1-VC.zip
        DOWNLOAD_NO_PROGRESS ON
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/downloads
    )

    FetchContent_GetProperties(SDL2_mixer)

    if(NOT sdl2_mixer_POPULATED)
        FetchContent_MakeAvailable(SDL2_mixer)
        set(SDL2_MIXER_INCLUDE_DIR ${sdl2_mixer_SOURCE_DIR}/include)

    if (${CMAKE_SIZEOF_VOID_P} MATCHES 8)
        set(SDL2_MIXER_LIBRARIES ${sdl2_mixer_SOURCE_DIR}/lib/x64/SDL2_mixer.lib)
        set(SDL2_MIXER_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x64/SDL2_mixer.dll")
    else()
        set(SDL2_MIXER_LIBRARIES ${sdl2_mixer_SOURCE_DIR}/lib/x86/SDL2_mixer.lib)
        set(SDL2_MIXER_LIBRARY_DLL "${sdl2_mixer_SOURCE_DIR}/lib/x86/SDL2_mixer.dll")
    endif()

    endif()




    # include & link sdl to imgui
    target_include_directories(imgui PUBLIC 
        ${imgui_SOURCE_DIR} 
        ${imgui_SOURCE_DIR}/backends 
        ${SDL2_INCLUDE_DIR}
    )

    target_link_libraries(imgui PUBLIC
        ${SDL2_LIBRARIES}
    )


    find_package(VLD CONFIG)

    set(XINPUT_LIB XInput)




    # set all include directories
    #target_include_directories(${ENGINE_NAME} PRIVATE
    target_include_directories(${ENGINE_NAME} PUBLIC
        ${VLD_INCLUDE_DIR}
        ${SDL2_INCLUDE_DIR} 
        ${SDL2_IMAGE_INCLUDE_DIR} 
        ${SDL2_TTF_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR}

        ${imgui_SOURCE_DIR}
        ${imgui_SOURCE_DIR}/backends
        ${imgui_plot_SOURCE_DIR}
    )

    # include steamworks 
    if(STEAMWORKS_AVAILABLE)

        target_include_directories(${ENGINE_NAME} PUBLIC
            ${STEAMWORKS_INCLUDE_DIR}
        )

        target_compile_definitions(${ENGINE_NAME} PUBLIC STEAMWORKS_ENABLED)

    endif()


    
    # Link libraries to Engine
    target_link_libraries(${ENGINE_NAME} PUBLIC
        ${SDL2_LIBRARIES} 
        ${SDL2_IMAGE_LIBRARIES} 
        ${SDL2_TTF_LIBRARIES} 
        ${SDL2_MIXER_LIBRARIES} 

        ${VLD_LIBRARY}
        ${XINPUT_LIB}
        imgui
        imgui_plot
    )

    # Link Steamworks
    if(STEAMWORKS_AVAILABLE)

        target_link_libraries(${ENGINE_NAME} PUBLIC 
            ${STEAMWORKS_LIBRARY}
        )

    endif()


elseif(NOT EMSCRIPTEN)

    FetchContent_Declare(
        SDL2
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG release-2.32.8
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    FetchContent_MakeAvailable(SDL2)
    
    FetchContent_Declare(
        SDL2_image
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
        GIT_TAG release-2.8.8
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    
    set(SDL2IMAGE_INSTALL OFF)
    FetchContent_MakeAvailable(SDL2_image)
    
    FetchContent_Declare(
        SDL2_ttf
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
        #GIT_TAG release-2.24.0
        GIT_TAG release-2.22.0
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    
    set(SDL2TTF_INSTALL OFF)
    FetchContent_MakeAvailable(SDL2_ttf) 
    

    FetchContent_Declare(
        SDL2_mixer
        GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
        GIT_TAG release-2.8.1
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
    )
    
    set(SDL2MIXER_INSTALL OFF)
    FetchContent_MakeAvailable(SDL2_mixer)
    
    target_include_directories(${ENGINE_NAME} PUBLIC 
        ${sdl2_SOURCE_DIR}/include
        ${sdl2_image_SOURCE_DIR}/include
        ${sdl2_ttf_SOURCE_DIR}/include
        ${sdl2_mixer_SOURCE_DIR}/include
    )
    
    target_link_libraries(${ENGINE_NAME} PUBLIC 
        SDL2::SDL2main 
        SDL2::SDL2 
        SDL2_image::SDL2_image 
        SDL2_ttf::SDL2_ttf
        SDL2_mixer::SDL2_mixer
    )

endif()



# errors:
target_include_directories(${ENGINE_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${glm_SOURCE_DIR}
)



# compile at max warning level + treat warnings as errors
target_compile_options(${ENGINE_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)




if(WIN32) 

    # add post build commands
    add_custom_command(TARGET ${ENGINE_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_LIBRARY_DLL}" "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}"
    )
    add_custom_command(TARGET ${ENGINE_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_IMAGE_LIBRARY_DLL}" "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}"
    )

    add_custom_command(TARGET ${ENGINE_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_TTF_LIBRARY_DLL}" "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}"
    )
 
    add_custom_command(TARGET ${ENGINE_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_MIXER_LIBRARY_DLL}" "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}"
    )


   if(STEAMWORKS_AVAILABLE)

        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy "${CMAKE_SOURCE_DIR}/BudgetEngine/Core/steam_appid.txt" "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}/steam_appid.txt"
        )

        add_custom_command(TARGET ${ENGINE_NAME} POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy "${STEAMWORKS_LIBRARY_DLL}" "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}"
        )


    endif()


    add_custom_command(TARGET ${ENGINE_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Resources" "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}/Resources"
    )


elseif(EMSCRIPTEN)

    set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} \ 
        -sUSE_SDL=2 \
        -sUSE_SDL_IMAGE=2 \ 
        -sUSE_SDL_TTF=2 \
        -sUSE_SDL_MIXER=2 \ 
        -g"
    )
    set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} \
        -sUSE_SDL=2 \
        -sUSE_SDL_IMAGE=2 \
        -sUSE_SDL_TTF=2 \
        -sUSE_SDL_MIXER=2 \
        -g \
        -sALLOW_MEMORY_GROWTH \ 
        --preload-file \"${CMAKE_SOURCE_DIR}/Resources@/\" \
        -sSDL2_IMAGE_FORMATS=['tga','png']"
    )

    # work around https://github.com/emscripten-core/emscripten/issues/11513
    set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -fno-stack-protector")

    # Use the Emscripten toolchain
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/EmscriptenToolchain.cmake)

    # Additional settings for Emscripten build
    set_target_properties(${ENGINE_NAME} PROPERTIES
        COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
        LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
    )

    # Have emscripten generate a html page too.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")

endif()


