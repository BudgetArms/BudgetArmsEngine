#include "ImguiComponent.h"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <imgui_plot.h> 

#include "Core/Renderer.h"


using namespace bae;


ImguiComponent::ImguiComponent(GameObject& owner) :
	Component(owner)
{
}


