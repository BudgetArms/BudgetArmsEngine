#pragma once
#include <glm.hpp>

namespace bae
{
    class Transform final
    {
    public:
        const glm::vec3& GetPosition() const { return m_Position; }
        void SetPosition(float x, float y, float z);

    private:
        glm::vec3 m_Position;


    };
}
