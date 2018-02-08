#include "RequestErrorReactor.h"

using namespace sg::microreactor;
using namespace streetgangapi;
using namespace streetgangserver;


RequestErrorReactor::RequestErrorReactor(Connection& connection, std::shared_ptr<ResponseError> message, std::shared_ptr<StreetGangResponder> responder)
    : MessageReactor(connection, message)
    , mResponder(responder)
{
}

RequestErrorReactor::~RequestErrorReactor()
{
}

bool RequestErrorReactor::Process()
{
    return SendMessage(InputMessage());
    //return SubmitTask(std::bind(&RequestErrorReactor::SendMessage, this, response), "RequestErrorReactor::SendMessage");
}
