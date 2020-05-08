#include "GETv1versionReactorBase.h"


namespace myserver
{
    GETv1versionReactorBase::GETv1versionReactorBase(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request)
        : RestReactor(connection, request)
    {
        nlohmann::json serializer(*request->mRawMessage);
        mRequestContent.Decode(serializer);
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

        auto buffer = std::make_shared<std::string>();
        if (mRequestContent.Write(*buffer))
        {
            SetHttpBody(buffer, response);
        }

        return response.Send(mConnection);
    }
}
