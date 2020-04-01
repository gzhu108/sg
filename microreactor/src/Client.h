#pragma once
#ifndef sg_microreactor_Client
#define sg_microreactor_Client

#include "Message.h"
#include "Connection.h"


namespace sg { namespace microreactor
{
    class Client
    {
    public:
        Client();
        virtual ~Client();

        PROPERTY(ClientName, std::string);

    public:
        virtual void Initialize(std::shared_ptr<Connection> connection);
        virtual std::shared_ptr<Connection> GetConnection();

	protected:
		std::shared_ptr<Connection> mConnection;
    };
}}


#endif // sg_microreactor_Client
