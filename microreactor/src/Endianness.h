#pragma once
#ifndef microreactor_Endian
#define microreactor_Endian

#ifdef _MSC_VER
#include <WinSock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
#include <netinet/in.h>
#endif

#include <algorithm>
#include "Common.h"


namespace microreactor
{
    namespace Endian
    {
        enum class EndianType : uint32_t
        {
            LittleEndian,
            BigEndian,
        };

        template<typename T>
        class Endianness
        {
        public:
            explicit Endianness(EndianType endianType = EndianType::LittleEndian) : mEndianType(endianType) {}
            virtual bool SetEndian(EndianType endianType) = 0;
            virtual bool GetEndian(EndianType endianType, T& inout) const = 0;

        protected:
            EndianType mEndianType;
        };

        template<typename T>
        void NetOrder(const T* inPtr, void* outPtr, uint32_t size = sizeof(T))
        {
            switch(size)
            {
            case 1:
                *(uint8_t*)(outPtr) = *(uint8_t*)(inPtr);
                break;
            case 2:
                *(uint16_t*)(outPtr) = htons(*(uint16_t*)(inPtr));
                break;
            case 4:
                *(uint32_t*)(outPtr) = htonl(*(uint32_t*)(inPtr));
                break;
            default:
#if __BYTE_ORDER__ && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
                std::copy(&((uint8_t*)inPtr)[0], &((uint8_t*)inPtr)[size], &((uint8_t*)outPtr)[0]);
#else
                std::reverse_copy(&((uint8_t*)inPtr)[0], &((uint8_t*)inPtr)[size], &((uint8_t*)outPtr)[0]);
#endif
                break;
            }
        }

        template<typename T>
        void HostOrder(const void* inPtr, T* outPtr, uint32_t size = sizeof(T))
        {
            switch(size)
            {
            case 1:
                *(uint8_t*)(outPtr) = *(uint8_t*)(inPtr);
                break;
            case 2:
                *(uint16_t*)(outPtr) = ntohs(*(uint16_t*)(inPtr));
                break;
            case 4:
                *(uint32_t*)(outPtr) = ntohl(*(uint32_t*)(inPtr));
                break;
            default:
#if __BYTE_ORDER__ && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
                std::copy(&((uint8_t*)inPtr)[0], &((uint8_t*)inPtr)[size], &((uint8_t*)outPtr)[0]);
#else
                std::reverse_copy(&((uint8_t*)inPtr)[0], &((uint8_t*)inPtr)[size], &((uint8_t*)outPtr)[0]);
#endif
                break;
            }
        }

        template<typename T>
        T NetOrder(T in)
        {
            T out;
            NetOrder(&in, &out);
            return out;
        }

        template<typename T>
        T HostOrder(T in)
        {
            T out;
            HostOrder(&in, &out);
            return out;
        }

        template<typename T>
        bool NetOrder(const T& in, T& inout)
        {
            return in.GetEndian(EndianType::BigEndian, inout);
        }

        template<typename T>
        bool HostOrder(const T& in, T& inout)
        {
            return in.GetEndian(EndianType::LittleEndian, inout);
        }
    }
}


#endif //microreactor_Endian
