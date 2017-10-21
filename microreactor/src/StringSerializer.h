#pragma once
#ifndef sg_microreactor_StringSerializer
#define sg_microreactor_StringSerializer

#include "Serializer.h"


namespace sg { namespace microreactor
{
    class StringSerializer : public Serializer
    {
    public:
        StringSerializer();
        virtual ~StringSerializer();
        
    public:
        virtual bool ReadCount(std::istream& stream, uint16_t& count) override;
        virtual bool WriteCount(uint16_t count, std::ostream& stream) override;
    };
}}


#endif // sg_microreactor_StringSerializer
