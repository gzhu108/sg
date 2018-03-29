#include "ResponseError.h"
#include "BinarySerializer.h"

using namespace sg::microreactor;
using namespace worldapi;


ResponseError::ResponseError()
{
    Id.set((int32_t)ID::Error);
}

ResponseError::~ResponseError()
{
}

bool ResponseError::Encode(std::ostream& stream) const
{
    BinarySerializer serializer;

    return serializer.Write(TrackId.cref(), stream)
        && serializer.Write(ResponseTimeout.cref(), stream)
        && serializer.Write(Result.cref(), stream)
        && serializer.Write(RequestId.cref(), stream)
        && serializer.Write(ErrorMessage.cref(), stream);
}

bool ResponseError::Decode(std::istream& stream)
{
    BinarySerializer serializer;

    serializer.ReadProperty(stream, TrackId);
    serializer.ReadProperty(stream, ResponseTimeout);
    serializer.ReadProperty(stream, Result);
    serializer.ReadProperty(stream, RequestId);
    serializer.ReadProperty(stream, ErrorMessage);

    return true;
}
