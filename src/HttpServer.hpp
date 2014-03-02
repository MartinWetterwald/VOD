#ifndef _HTTP_SERVER_HPP_
#define _HTTP_SERVER_HPP_

#include <Tcp/Server.hpp>
#include <unordered_set>

#include "CatalogRequest.hpp"

class HttpServer : public NetFlux::Tcp::Server
{
public:
    HttpServer ( ) = default;
    virtual ~HttpServer ( );

protected:
    virtual void readEventAction ( );
    virtual void writeEventAction ( );
    virtual void exceptEventAction ( );

    virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & );

private:
    std::unordered_set <HttpRequest *> mrequests;



    friend class HttpRequest;
};

#endif

