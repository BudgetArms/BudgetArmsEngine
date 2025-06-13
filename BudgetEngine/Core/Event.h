#pragma once
#include <string>

// can't be struct bc then string should be const 
// then copy/move assignment is not possible.
// ---
// or use smartpointers for this, but that seems like a waste of memory/performance hit
namespace bae
{
    class Event
    {
    public:
        explicit Event(const std::string& id) :
            ID{ id }
        {
        }

        constexpr std::string GetID() const { return ID; }

    private:
        std::string ID;


    };
}


