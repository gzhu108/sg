#pragma once
#ifndef streetgangapi_PBMessage
#define streetgangapi_PBMessage

#include "google/protobuf/stubs/common.h"
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "MessageHeader.pb.h"


namespace streetgangapi
{
    template <typename BaseMessage>
    class PBMessage : public BaseMessage
    {
    public:
        template <typename ProtoMessage>
        bool Serialize(const std::string& messageId, const ProtoMessage& message, std::ostream& stream) const
        {
            streetgangpb::MessageHeader header;
            header.set_message_id(messageId);

            google::protobuf::io::OstreamOutputStream ostreamOutputStream(&stream);
            google::protobuf::io::CodedOutputStream codedOutputStream(&ostreamOutputStream);

            uint32_t size = (uint32_t)header.ByteSize();
            codedOutputStream.WriteVarint32(size);
            header.SerializeWithCachedSizes(&codedOutputStream);

            size = (uint32_t)message.ByteSize();
            codedOutputStream.WriteVarint32(size);
            message.SerializeWithCachedSizes(&codedOutputStream);
            
            return true;
        }

        template <typename ProtoMessage>
        bool Deserialize(google::protobuf::io::CodedInputStream& codedInputStream, ProtoMessage& message)
        {
            google::protobuf::io::CodedInputStream::Limit previousSize = codedInputStream.ReadLengthAndPushLimit();
            bool result = message.ParseFromCodedStream(&codedInputStream);
            codedInputStream.PopLimit(previousSize);
            if (!result)
            {
                return false;
            }

            return true;
        }
    };
}


#endif // streetgangapi_PBMessage
