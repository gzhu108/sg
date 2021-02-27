#include "ResponseError.h"

using namespace microreactor;
using namespace streetgangapi;


ResponseError::ResponseError()
{
    Id.set((int32_t)ID::Error);
}

ResponseError::~ResponseError()
{
}
