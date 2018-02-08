#include "StreetGangMessageDecoder.h"
#include "StreetGangIds.h"
#include "StreetGangSessionManager.h"
#include "BinarySerializer.h"

#include "RequestErrorReactor.h"
#include "RequestGetVersionReactor.h"
#include "RequestByebyeReactor.h"
#include "RequestCreateWorldReactor.h"
#include "RequestGetSceneReactor.h"

#include "BinaryRequestByebye.h"
#include "BinaryRequestGetVersion.h"
#include "BinaryRequestCreateWorld.h"
#include "BinaryRequestGetScene.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


StreetGangMessageDecoder::StreetGangMessageDecoder()
{
    mResponder = std::make_shared<BinaryStreetGangResponder>();
}

StreetGangMessageDecoder::~StreetGangMessageDecoder()
{
}

std::shared_ptr<Reactor> StreetGangMessageDecoder::Decode(std::istream& stream, Connection& connection)
{
    BinarySerializer serializer;
    
    //stream->clear();
    //stream->ignore(std::numeric_limits<std::streamsize>::max(), MARKER);
    //if (stream->eof() || stream->fail() || stream->bad())
    //{
    //    return nullptr;
    //}

    int32_t id = (int32_t)ID::Unknown;
    if (!serializer.Read(stream, id))
    {
        // pushback the total number of characters in ID
        auto bytes = stream.gcount();
        auto position = stream.tellg() - bytes;
        stream.seekg(position, stream.beg);
        stream.clear();
        return nullptr;
    }

    uint64_t length = 0;
    if (!serializer.Read(stream, length) || GetStreamSize(stream) < length)
    {
        // pushback the total number of characters in ID + Length
        auto bytes = stream.gcount() + (std::streamsize)sizeof(int32_t);
        auto position = stream.tellg() - bytes;
        stream.seekg(position, stream.beg);
        stream.clear();
        return nullptr;
    }

    switch (id)
    {
        case (int32_t)(streetgangapi::ID::Byebye):
        {
            auto message = std::make_shared<BinaryRequestByebye>();
            if (message->Decode(stream))
            {
                auto reactor = std::make_shared<RequestByebyeReactor>(connection, message, mResponder);
                return reactor;
            }
            break;
        }

        case (int32_t)(streetgangapi::ID::GetVersionRequest) :
        {
            auto message = std::make_shared<BinaryRequestGetVersion>();
            if (message->Decode(stream))
            {
                auto reactor = std::make_shared<RequestGetVersionReactor>(connection, message, mResponder);
                return reactor;
            }
            break;
        }

        case (int32_t)(streetgangapi::ID::CreateWorldRequest):
        {
            auto message = std::make_shared<BinaryRequestCreateWorld>();
            if (message->Decode(stream))
            {
                auto reactor = std::make_shared<RequestCreateWorldReactor>(connection, message, mResponder);
                return reactor;
            }
            break;
        }

        case (int32_t)(streetgangapi::ID::GetSceneRequest) :
        {
            auto message = std::make_shared<BinaryRequestGetScene>();
            if (message->Decode(stream))
            {
                auto reactor = std::make_shared<RequestGetSceneReactor>(connection, message, mResponder);
                auto session = StreetGangSessionManager::GetInstance().GetSession(message->WorldId.cref());
                reactor->SetSession(session);
                return reactor;
            }
            break;
        }

        default:
        {
            mResponder->SendErrorResponse(connection, "Unknown track ID", ResultCode::ErrorNotImplemented, id, "Unknown request");
            return nullptr;
        }
    }

    mResponder->SendErrorResponse(connection, "Unknown track ID", ResultCode::ErrorBadRequest, id, "Invalid request");
    return nullptr;
}
