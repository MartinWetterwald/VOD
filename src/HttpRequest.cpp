#include "Request.hpp"
#include "HttpServer.hpp"
#include <sstream>

HttpRequest::HttpRequest ( int sock, const NetFlux::Net::InetAddress & address )
    : Request ( sock, address ), mpserver ( nullptr ) { }

HttpRequest::~HttpRequest ( )
{
    if ( mpserver )
    {
        mpserver -> mrequests.erase ( this );
    }
}

void HttpRequest::answer ( const std::string & body, uint16_t code )
{
    std::stringstream responseSs;


    {
        if ( strlen ( requestInfo.version ) == 0 )
        {
            responseSs << "HTTP/1.1";
        }
        else
        {
            responseSs << requestInfo.version;
        }

        responseSs << " " << code << " ";

        switch ( code )
        {
            case 200:
                responseSs << "OK";
                break;

            case 403:
                responseSs << "Forbidden";
                break;

            case 404:
                responseSs << "Not Found";
                break;

            default:
                responseSs << "UNKNOWN";
                break;
        }

        responseSs << "\r\n";
    }


    responseSs << "Content-Type: text/plain; charset=utf-8\r\n";
    responseSs << "Content-Length: " << body.length ( ) << "\r\n";
    responseSs << "\r\n";

    responseSs << body;

    std::string response ( responseSs.str ( ) );
    send ( response.c_str ( ), response.length ( ) );
}

void HttpRequest::requestEventAction ( )
{
    reading = false;
}
