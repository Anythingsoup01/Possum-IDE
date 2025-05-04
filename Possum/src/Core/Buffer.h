#pragma once

#include <stddef.h>
#include <stdexcept>

namespace Ferret::Possum
{
    struct CBuffer
    {
    private:
        char* m_Data;
        size_t m_Size;
        size_t m_Capacity;
    public:
        // Defaults to one as initial capacity
        CBuffer(size_t initialCapacity = 10)
        {
            m_Data = (char*)malloc(initialCapacity);
            if (m_Data == nullptr)
            {
                free(m_Data);
                throw std::runtime_error("Failed to malloc data");
            }
            m_Size = 0;
            m_Capacity = initialCapacity;
        }

        void resize(size_t newCapacity)
        {
            char* newData = (char*)realloc(m_Data, newCapacity);
            if (newData == nullptr)
            {
                throw std::runtime_error("Failed to resize buffer");
            }
            m_Data = (char*)malloc(newCapacity);
            m_Data = newData;
            m_Capacity = newCapacity;
        }

        void copy(const char* inData)
        {
            size_t len = strlen(inData);
            if (m_Size + len >= m_Capacity)
            {
                size_t newCapacity = (m_Size + len) * 2;
                resize(newCapacity);
            }
            strcpy(m_Data, inData);
            m_Size += len;
        }

        void set(size_t index, char character)
        {
            if (index > m_Capacity)
                throw std::out_of_range("Index is out of range!");

            m_Data[index] = character;
        }


        bool active()
        {
            if (strlen(m_Data) > 0)
                return true;
            return false;
        }

        // Getters

        char* data() { return m_Data; }

        size_t& size() { return m_Size; }

        size_t& capacity() { return m_Capacity; }

        char operator[](size_t index)
        {
            if (index > m_Capacity || index < 0)
                throw std::out_of_range("Index is out of range!");
            return m_Data[index];
        }

    };
}
