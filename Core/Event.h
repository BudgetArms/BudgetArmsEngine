#pragma once

#include <string>

// can't be struct bc then string should be const
// then copy/move assignment is not possible.
// ---
// or use smart-pointers for this, but that seems like a waste of memory/performance hit
namespace bae
{
    class Event
    {
    public:
        explicit Event(const std::string& id) :
            m_Id{ id }
        {
        }

        [[nodiscard]] constexpr std::string GetId() const { return m_Id; }

    private:
        std::string m_Id;
    };
}


