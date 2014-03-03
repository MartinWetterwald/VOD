#ifndef _REQUEST_HPP_
#define _REQUEST_HPP_

#include <Tcp/ServerStream.hpp>

class HttpServer;

class Request : public NetFlux::Tcp::ServerStream
{
public:
    Request ( int sock, const NetFlux::Net::InetAddress & address );
    virtual ~Request ( );

    virtual void toString ( std::ostream & os ) const;


protected:
    virtual void readEventAction ( );
    virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & event );

    virtual void requestEventAction ( ) = 0;

    bool reading;


private:
    Request ( ) = delete;

    uint8_t endRequest;



};

#endif
