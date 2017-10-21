#pragma once
#ifndef streetgangapi_StreetGangApi
#define streetgangapi_StreetGangApi

#include "Api.h"
#include "StreetGangRequestEncoder.h"
#include "SessionId.h"
#include "Rectangle.h"


namespace streetgangapi
{
    class StreetGangApi : public sg::microreactor::Api<StreetGangRequestEncoder>
    {
    public:
        StreetGangApi(sg::microreactor::Connection& connection, std::shared_ptr<StreetGangRequestEncoder> messageEncoder);
        virtual ~StreetGangApi();

    public:
        // API
        bool Byebye();
        bool GetVersion();
        bool CreateWorld(const std::string& worldName);
        bool GetScene(const SessionId& worldId, const streetgangapi::Rectangle<float>& rect);

    protected:
        template <typename SendMessageType>
        bool SendMessage(std::shared_ptr<SendMessageType> message)
        {
            bool messageSent = sg::microreactor::Api<StreetGangRequestEncoder>::SendMessage(message);
            if (!messageSent)
            {
                //auto responseError = std::make_shared<ResponseError>();
                //responseError->Result.set((int32_t)ResultCode::ErrorServiceUnavailable);
                //responseError->RequestId.set(std::static_pointer_cast<MessageBase>(message)->Id.cref());
                //responseError->ErrorMessage.set("Failed to send request");

                // Send the error message to a callback?
                //auto responseErrorReactor = std::make_shared<ResponseErrorReactor>(mConnection, responseError);
                //responseErrorReactor->SetParent(message);
                //responseErrorReactor->Process();
            }

            return messageSent;
        }
   };
}

#endif // streetgangapi_StreetGangApi
