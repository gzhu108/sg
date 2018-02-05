#pragma once
#ifndef myserver_POSTv1updateResponse
#define myserver_POSTv1updateResponse

#include "Microreactor.h"


namespace myserver
{
    class POSTv1updateResponse : public sg::microreactor::RestResponse
    {
    public:
        POSTv1updateResponse();
        virtual ~POSTv1updateResponse();

        // TODO: Add Response properties here
    };
}


#endif // myserver_POSTv1updateResponse
