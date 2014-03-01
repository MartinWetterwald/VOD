#ifndef _HTTP_SERVER_HPP_
#define _HTTP_SERVER_HPP_

#include <Tcp/Server.hpp>

class HTTPServer : public NetFlux::Tcp::Server
{
public:
    HTTPServer ( ) = default;

protected:
    virtual void readEventAction ( );
    virtual void writeEventAction ( );
    virtual void exceptEventAction ( );

    virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & );
};

#endif

