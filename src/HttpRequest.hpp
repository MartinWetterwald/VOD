#ifndef _HTTP_REQUEST_HPP_
#define _HTTP_REQUEST_HPP_

#include "Request.hpp"

#include <string>

class HttpServer;

class HttpRequest : public Request
{

public:
    HttpRequest ( int sock, const NetFlux::Net::InetAddress & address );
    virtual ~HttpRequest ( );


protected:
    virtual void requestEventAction ( );
    void answer ( const std::string & body, uint16_t code = 200 );

    static const uint16_t MAXURLSIZE = 255;
    struct
    {
        char method [ 7 ];
        char url [ MAXURLSIZE ];
        char version [ 8 ];
    } requestInfo;

    HttpServer * mpserver;


private:
    HttpRequest ( ) = delete;



    friend class HttpServer;
};

#endif
