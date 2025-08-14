#pragma once

#include <filesystem>
#include <string>
#include <vector>

#include <glm.hpp>
#include <SDL_rect.h>


namespace bae::Utils
{
	struct Window
	{
		explicit Window(const std::string& title, const std::filesystem::path& resourceFolder, int width, int height, bool isVSyncOn);


		std::string title;
		std::filesystem::path resourceFolder;
		int width;
		int height;
		bool bIsVSyncOn;


	};


	struct Color
	{
	public:
		explicit Color(float r, float g, float b, float a = 1.f);
		Color(const Color& other, float a = 1.f);
		Color();


		float r;
		float g;
		float b;
		float a;

		static const Color White;
		static const Color Black;
		static const Color Red;
		static const Color Green;
		static const Color Blue;
		static const Color Yellow;
		static const Color Orange;
		static const Color Magenta;
		static const Color Cyan;
		static const Color Purple;
		static const Color Gray;
		static const Color DarkGray;
		static const Color Transparent;

	};


	bool IsOverlapping(const SDL_Rect& rect1, const SDL_Rect& rect2);

	Uint8 FloatToUint8(float v);

	void SetDrawColor(const Color& color);

	void DrawLine(const glm::vec2& line1, const glm::vec2& line2, int width, const Color& color);

	void DrawRect(const SDL_Rect& rect, const Color& color);
	void FillRect(const SDL_Rect& rect, const Color& color);

	void DrawCircle(const glm::vec2& center, int radius, const Color& color);
	void FillCircle(const glm::vec2& center, int radius, const Color& color);

	void DrawEllipse(const glm::vec2& center, int radiusX, int radiusY, const Color& color);
	void FillEllipse(const glm::vec2& center, int radiusX, int radiusY, const Color& color);

	void DrawPolygon(const std::vector<glm::vec2>& points, const Color& color);
	void FillPolygon(const std::vector<glm::vec2>& points, const Color& color);




}


