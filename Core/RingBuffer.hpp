#pragma once

#include <mutex>
#include <vector>

#include "Core/Logger.hpp"


namespace bae
{
    template<typename T>
    class RingBuffer
    {
    public:
        explicit RingBuffer(size_t capacity);

        RingBuffer(const RingBuffer&)            = delete;
        RingBuffer(RingBuffer&&)                 = delete;
        RingBuffer& operator=(const RingBuffer&) = delete;
        RingBuffer& operator=(RingBuffer&&)      = delete;

        bool Push(const T& item);
        bool Pop(T& item);
        // bool Resize(int size);

        [[nodiscard]] bool IsEmpty() const;
        [[nodiscard]] bool IsFull() const;

    private:
        [[nodiscard]] size_t Increment(size_t idx) const;


        std::mutex m_Mutex;
        std::vector<T> m_Buffer{};
        size_t m_Head{};
        size_t m_Tail{};
    };
}


template<typename T>
bae::RingBuffer<T>::RingBuffer(size_t capacity)
{
    m_Buffer.resize(capacity);
}

template<typename T>
bool bae::RingBuffer<T>::Push(const T& item)
{
    std::lock_guard lock(m_Mutex);

    if(IsFull())
    {
        Logger::LogWarning("RingBuffer is full");
        return false;
    }

    m_Buffer[m_Head] = item;
    m_Head           = Increment(m_Head);
    return true;
}

template<typename T>
bool bae::RingBuffer<T>::Pop(T& item)
{
    std::lock_guard lock(m_Mutex);

    if(IsEmpty())
    {
        return false;
    }

    item   = m_Buffer[m_Tail];
    m_Tail = Increment(m_Tail);
    return true;
}

template<typename T>
bool bae::RingBuffer<T>::IsEmpty() const
{
    return m_Head == m_Tail;
}

template<typename T>
bool bae::RingBuffer<T>::IsFull() const
{
    return Increment(m_Head) == m_Tail;
}

template<typename T>
size_t bae::RingBuffer<T>::Increment(const size_t idx) const
{
    return (idx + 1) % m_Buffer.capacity();
}
