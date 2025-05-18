cmake_minimum_required(VERSION 3.26)
project(BudgetEngine)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EMSCRIPTEN OFF)
set(ENABLE_CONSOLE ON)

message("\n\n\n Starting CMAKE\n\n")

if(EMSCRIPTEN)
    message("Using Emscripten!")
endif()

if(EMSCRIPTEN)
    set(TARGET_NAME BudgetEngine_web)
    set(TARGET_PARAM )
else()
    set(TARGET_NAME BudgetEngine)

    if(ENABLE_CONSOLE)
        set(TARGET_PARAM )
    else()
        set(TARGET_PARAM WIN32)
    endif()

endif()

# list source files here
add_executable(${TARGET_NAME} ${TARGET_PARAM}
    "Game/Main.cpp"
    "BudgetEngine/Components/Component.h"
    "BudgetEngine/Core/BudgetEngine.cpp"                    "BudgetEngine/Core/BudgetEngine.h"
    "BudgetEngine/Core/GameObject.cpp"                      "BudgetEngine/Core/GameObject.h"
    "BudgetEngine/Core/Renderer.cpp"                        "BudgetEngine/Core/Renderer.h"
    "BudgetEngine/Core/Scene.cpp"                           "BudgetEngine/Core/Scene.h"
    "BudgetEngine/Commands/Command.h"
    "BudgetEngine/Commands/GameActorCommand.h"
    "BudgetEngine/Commands/MoveCommand.h"
    "BudgetEngine/Commands/FireCommand.h"
    "BudgetEngine/Components/TextComponent.cpp"             "BudgetEngine/Components/TextComponent.h"
    "BudgetEngine/Components/Component.h"
    "BudgetEngine/Components/FpsCounterComponent.cpp"       "BudgetEngine/Components/FpsCounterComponent.h"
    "BudgetEngine/Components/ImguiComponent.cpp"            "BudgetEngine/Components/ImguiComponent.h"
    "BudgetEngine/Components/InputComponent.h"
    "BudgetEngine/Components/TextureComponent.cpp"          "BudgetEngine/Components/TextureComponent.h"
    "BudgetEngine/Components/TrashTheCacheComponent.cpp"    "BudgetEngine/Components/TrashTheCacheComponent.h"
    "BudgetEngine/Components/TransformComponent.cpp"        "BudgetEngine/Components/TransformComponent.h"
    "BudgetEngine/Managers/InputManager.cpp"                "BudgetEngine/Managers/InputManager.h"
    "BudgetEngine/Managers/ResourceManager.cpp"             "BudgetEngine/Managers/ResourceManager.h"
    "BudgetEngine/Managers/SceneManager.cpp"                "BudgetEngine/Managers/SceneManager.h"
    "BudgetEngine/Singletons/GameTime.h"
    "BudgetEngine/Singletons/Singleton.h"
    "BudgetEngine/Wrappers/Controller.cpp"                   "BudgetEngine/Wrappers/Controller.h"
    "BudgetEngine/Wrappers/Keyboard.cpp"                   "BudgetEngine/Wrappers/Controller.h"
    "BudgetEngine/Wrappers/Font.cpp"                        "BudgetEngine/Wrappers/Font.h"
    "BudgetEngine/Wrappers/Texture2D.cpp"                   "BudgetEngine/Wrappers/Texture2D.h"
 )


# enable c++20 features
target_compile_features(${TARGET_NAME} PUBLIC cxx_std_20)

# set BudgetEngine as the default startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${TARGET_NAME})




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

#add_library(imgui_plot STATIC
#    ${imgui_plot_SOURCE_DIR}/src/imgui_plot.cpp    
#)

target_include_directories(imgui_plot PUBLIC
  ${imgui_plot_SOURCE_DIR}/include/imgui_plot
)

# Link Imgui Plot to ImGui
target_link_libraries(imgui_plot PUBLIC imgui)

# Link xinput to project (windows only bc can't fetch xinput)
target_link_libraries(${TARGET_NAME} PUBLIC XInput)


# add SDL2
FetchContent_Declare(
    SDL2
    URL https://www.libsdl.org/release/SDL2-devel-2.30.10-VC.zip 
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
    URL https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.8.3-VC.zip 
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



# include & link sdl to imgui
target_include_directories(
    imgui 
    PUBLIC 
    ${imgui_SOURCE_DIR} 
    ${imgui_SOURCE_DIR}/backends 
    ${SDL2_INCLUDE_DIR}
)

target_link_libraries(
    imgui 
    PUBLIC
    ${SDL2_LIBRARIES}
)


find_package(VLD CONFIG)


# set all include directories
target_include_directories(${TARGET_NAME} PRIVATE
    ${VLD_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR} 
    ${SDL2_IMAGE_INCLUDE_DIR} 
    ${SDL2_TTF_INCLUDE_DIR}
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
    ${imgui_plot_SOURCE_DIR}
)
  
# set libraries to link with
target_link_libraries(${TARGET_NAME} PUBLIC
    imgui_plot
    imgui
    ${SDL2_LIBRARIES} 
    ${SDL2_IMAGE_LIBRARIES} 
    ${SDL2_TTF_LIBRARIES} 
    ${VLD_LIBRARY}
)


elseif(NOT EMSCRIPTEN)

FetchContent_Declare(
    SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.30.10
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(SDL2)
  
FetchContent_Declare(
    SDL2_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG release-2.8.3
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

set(SDL2IMAGE_INSTALL OFF)
FetchContent_MakeAvailable(SDL2_image)

FetchContent_Declare(
    SDL2_ttf
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
    GIT_TAG release-2.22.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

set(SDL2TTF_INSTALL OFF)
FetchContent_MakeAvailable(SDL2_ttf)

target_include_directories("${TARGET_NAME}" PUBLIC 
    ${sdl2_SOURCE_DIR}/include
    ${sdl2_image_SOURCE_DIR}/include
    ${sdl2_ttf_SOURCE_DIR}
)

target_link_libraries("${TARGET_NAME}" PUBLIC 
    SDL2::SDL2main SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf
)

endif()

target_include_directories(${TARGET_NAME} PRIVATE
  ${CMAKE_SOURCE_DIR}/BudgetEngine/
  ${glm_SOURCE_DIR}
)

# compile at max warning level + treat warnings as errors
target_compile_options(${TARGET_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)


if(EMSCRIPTEN) 

    set(EMSCRIPTEN_COMPILE_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -g")
    set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -sUSE_SDL=2 -sUSE_SDL_IMAGE=2 -sUSE_SDL_TTF=2 -g -sALLOW_MEMORY_GROWTH --preload-file \"${CMAKE_SOURCE_DIR}/Resources@/\" -sSDL2_IMAGE_FORMATS=['tga','png']")

    # work around https://github.com/emscripten-core/emscripten/issues/11513
    set(EMSCRIPTEN_LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS} -fno-stack-protector")

    # Use the Emscripten toolchain
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/EmscriptenToolchain.cmake)

    # Additional settings for Emscripten build
    set_target_properties(${TARGET_NAME} PROPERTIES
        COMPILE_FLAGS "${EMSCRIPTEN_COMPILE_FLAGS}"
        LINK_FLAGS "${EMSCRIPTEN_LINK_FLAGS}"
    )

    # Have emscripten generate a html page too.
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
  
elseif(WIN32)

    set_target_properties(${TARGET_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:${TARGET_NAME}>")
   
    # add post build commands
    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    )

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_IMAGE_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    )

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy "${SDL2_TTF_LIBRARY_DLL}" "$<TARGET_FILE_DIR:${TARGET_NAME}>"
    )

    add_custom_command(TARGET ${TARGET_NAME} POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/Resources" "$<TARGET_FILE_DIR:${TARGET_NAME}>/Resources"
    )

endif()



