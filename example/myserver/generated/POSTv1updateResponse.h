#pragma once
#ifndef myserver_POSTv1updateResponse
#define myserver_POSTv1updateResponse

#include "RestResponse.h"


namespace myserver
{
    class POSTv1updateResponse : public sg::microreactor::RestResponse
    {
    public:
        POSTv1updateResponse();
        virtual ~POSTv1updateResponse();

        
    public:
        virtual bool FlushToBuffer(std::string& buffer) override;
    };
}


#endif // myserver_POSTv1updateResponse
