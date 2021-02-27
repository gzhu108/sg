#pragma once
#ifndef microreactor_Client
#define microreactor_Client

#include "Message.h"
#include "Connection.h"


namespace microreactor
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
}


#endif // microreactor_Client
