#include "POSTv1updateReactorBase.h"


namespace myserver
{
    POSTv1updateReactorBase::POSTv1updateReactorBase(sg::microreactor::Connection& connection, std::shared_ptr<sg::microreactor::RestRequest> request)
        : RestReactor(connection, request)
    {
        nlohmann::json serializer(*request->mRawMessage);
        mRequestContent.Decode(serializer);
    }

    POSTv1updateReactorBase::~POSTv1updateReactorBase()
    {
    }

    bool POSTv1updateReactorBase::Process()
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
