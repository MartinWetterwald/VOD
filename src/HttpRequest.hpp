#ifndef _HTTP_REQUEST_HPP_
#define _HTTP_REQUEST_HPP_

#include <Tcp/ServerStream.hpp>

class HTTPServer;

class HttpRequest : public NetFlux::Tcp::ServerStream
{
public:
    HttpRequest ( int sock, const NetFlux::Net::InetAddress & address );
    virtual ~HttpRequest ( );

protected:
    virtual void readEventAction ( );
    virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & event );

    friend class HTTPServer;

private:
    bool reading;
    HTTPServer * mpserver;

    HttpRequest ( ) = delete;
};

#endif
