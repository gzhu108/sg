#include "GETv1versionReactorBase.h"
#include "GETv1versionMessage.h"


namespace myserver
{
    GETv1versionReactorBase::GETv1versionReactorBase(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request)
        : RestReactor(connection, request)
    {
    }

    GETv1versionReactorBase::~GETv1versionReactorBase()
    {
    }

    bool GETv1versionReactorBase::Process()
    {
        if (InputMessage() == nullptr)
        {
            LOG("Invalid request [ReqId=%s]\n", InputMessage()->TrackId.cref().c_str());
            return sg::microreactor::RestResponse::SendErrorWith(mConnection, 400, "Invalid request");
        }

        sg::microreactor::RestResponse response;
        response.mHeaders.emplace_back(sg::microreactor::HttpHeader("Content-Type", "application/json"));

        return response.Send(mConnection, GETv1versionMessage(InputMessage()));
    }
}
