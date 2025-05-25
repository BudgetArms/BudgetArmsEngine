cmake_minimum_required(VERSION 3.26)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME}
	"Main.cpp"

    "Base/Achievement.h"

    "Commands/DamageCommand.h" 
    "Commands/FireCommand.h"
    "Commands/ScoreCommand.h"
    
    "Components/ScoreComponent.cpp"             "Components/ScoreComponent.h"
    "Components/ScoreDisplayComponent.cpp"      "Components/ScoreDisplayComponent.h"
    "Components/HealthComponent.cpp"            "Components/HealthComponent.h"
    "Components/HealthDisplayComponent.cpp"     "Components/HealthDisplayComponent.h"

    "Entities/Qbert.cpp"                        "Entities/Qbert.h"
)
 

find_package(VLD CONFIG)

target_compile_options(${PROJECT_NAME} PRIVATE
  $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic -Werror>
)

target_link_libraries(${PROJECT_NAME} PUBLIC ${ENGINE_NAME})
target_include_directories(${ENGINE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})


if(WIN32)
     # add post build commands

  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E copy_directory "${CMAKE_SOURCE_DIR}/Resources" "${CMAKE_BINARY_DIR}/${PROJECT_FOLDER}/Resources"
  )
endif()
