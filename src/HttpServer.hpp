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

    void generateCatalog ( );

protected:
    virtual void readEventAction ( );
    virtual void writeEventAction ( );
    virtual void exceptEventAction ( );

    virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & );

    std::string mcatalog;

private:
    std::unordered_set <CatalogRequest *> mrequests;



    friend class CatalogRequest;
};

#endif

