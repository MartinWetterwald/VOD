#ifndef _HTTP_REQUEST_HPP_
#define _HTTP_REQUEST_HPP_

#include "Request.hpp"

class HttpServer;

class HttpRequest : public Request
{
public:
    HttpRequest ( int sock, const NetFlux::Net::InetAddress & address );
    virtual ~HttpRequest ( );


protected:
    virtual void requestEventAction ( );
    HttpServer * mpserver;


private:
    HttpRequest ( ) = delete;


    friend class HttpServer;
};

#endif
