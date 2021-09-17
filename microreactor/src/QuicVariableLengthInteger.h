#pragma once
#ifndef microreactor_QuicVariableLengthInteger
#define microreactor_QuicVariableLengthInteger

#include "QuicMessage.h"


namespace microreactor
{
    class QuicVariableLengthInteger : public QuicMessage
    {
    public:
        enum IntegerType : int8_t
        {
            Int6Bit = 0x00,
            Int14Bit = 0x01,
            Int30Bit = 0x02,
            Int62Bit = 0x03,
        };

        QuicVariableLengthInteger() {}
        QuicVariableLengthInteger(int64_t value) : mValue(value) {}
        virtual ~QuicVariableLengthInteger() {}

    public:
        virtual char *EncodeMessage(char *first, char *last) const override;
        virtual const char *DecodeMessage(const char *first, const char *last) override;

        IntegerType Type() const;
        const int64_t& Value() const { return mValue; }

    protected:
        int64_t mValue {0};
    };
}


#endif //microreactor_QuicVariableLengthInteger
