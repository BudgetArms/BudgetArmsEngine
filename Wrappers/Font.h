#pragma once
#include <string>


struct TTF_Font;

namespace bae
{
    /**
     * Simple RAII wrapper for a _TTF_Font
     */
    class Font final
    {
    public:
        explicit Font(const std::string& fullPath, float size);
        ~Font();

        Font(const Font&)             = delete;
        Font(Font&&)                  = delete;
        Font& operator=(const Font&)  = delete;
        Font& operator=(const Font&&) = delete;


        [[nodiscard]] TTF_Font* GetFont() const;

    private:
        TTF_Font* m_Font;
    };
}


