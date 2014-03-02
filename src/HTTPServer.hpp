#ifndef _HTTP_SERVER_HPP_
#define _HTTP_SERVER_HPP_

#include <Tcp/Server.hpp>
#include <unordered_set>

#include "CatalogRequest.hpp"

class HTTPServer : public NetFlux::Tcp::Server
{
public:
    HTTPServer ( ) = default;
    virtual ~HTTPServer ( );

protected:
    virtual void readEventAction ( );
    virtual void writeEventAction ( );
    virtual void exceptEventAction ( );

    virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & );

    friend class HttpRequest;

private:
    std::unordered_set <HttpRequest *> mrequests;
};

#endif

