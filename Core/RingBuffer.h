#pragma once

#include <atomic>
#include <string>
#include <vector>
#include <mutex>
#include <iostream>


namespace bae
{

    template<typename T>
    class RingBuffer
    {
    public:
        RingBuffer(int capacity);

        bool Push(const T& item);
        bool Pop(T& item);
        bool Resize(int size);

        bool IsEmpty() const;
        bool IsFull() const;


    private:
        size_t Increment(size_t idx) const;


        std::mutex m_Mutex;
        std::vector<T> m_Buffer;
        size_t m_Head;
        size_t m_Tail;


    };
}



template<typename T>
bae::RingBuffer<T>::RingBuffer(int capacity) :
    m_Buffer{},
    m_Head{ 0 },
    m_Tail{ 0 }
{
    m_Buffer.resize(capacity);
}

template<typename T>
bool bae::RingBuffer<T>::Push(const T& item)
{
    std::lock_guard<std::mutex> lock(m_Mutex);

    if (IsFull())
    {
        std::cout << "is full\n";
        return false;
    }

    m_Buffer[m_Head] = item;
    m_Head = Increment(m_Head);
    return true;
}

template<typename T>
bool bae::RingBuffer<T>::Pop(T& item)
{
    std::lock_guard<std::mutex> lock(m_Mutex);

    if (IsEmpty())
    {
        return false;
    }

    item = m_Buffer[m_Tail];
    m_Tail = Increment(m_Tail);
    return true;
}

template<typename T>
bool bae::RingBuffer<T>::IsEmpty() const
{
    return (m_Head == m_Tail);
}

template<typename T>
bool bae::RingBuffer<T>::IsFull() const
{
    return (Increment(m_Head) == m_Tail);
}

template<typename T>
size_t bae::RingBuffer<T>::Increment(size_t idx) const
{
    return (idx + 1) % m_Buffer.capacity();
}


