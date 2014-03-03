#ifndef _HTTP_REQUEST_HPP_
#define _HTTP_REQUEST_HPP_

#include <Tcp/ServerStream.hpp>

class HttpServer;

class HttpRequest : public NetFlux::Tcp::ServerStream
{
public:
    HttpRequest ( int sock, const NetFlux::Net::InetAddress & address );
    virtual ~HttpRequest ( );

    virtual void toString ( std::ostream & os ) const;


protected:
    virtual void readEventAction ( );
    virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & event );

    bool reading;
    HttpServer * mpserver;


private:
    HttpRequest ( ) = delete;

    uint8_t endRequest;



    friend class HttpServer;
};

#endif
