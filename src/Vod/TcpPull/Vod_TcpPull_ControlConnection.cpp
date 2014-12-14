#include "Vod_TcpPull_ControlConnection.hpp"
#include "Vod_TcpPull_Server.hpp"
#include "../Vod_parsingUtils.hpp"

#include <iostream>
#include <cstring>
#include <sstream>

namespace Vod {
namespace TcpPull
{
    ControlConnection::ControlConnection ( int sock, const NetFlux::Net::InetAddress & address )
        : Request ( sock, address ), mpserver ( nullptr ), mcursor ( 0 ), connected ( false ) { }

    ControlConnection::~ControlConnection ( )
    {
        if ( mpserver )
        {
            mpserver -> mstreams.erase ( this );
        }
    }

    bool ControlConnection::requestEventAction ( )
    {
        std::string req;
        if ( ! readRequest ( req ) )
        {
            std::cout << * this << " : unexpected reading failure" << std::endl;
            return false;
        }
        std::cout << req << std::endl;
        std::istringstream iss ( req );

        std::string word;
        int32_t id;

        if ( ! connected )
        {
            iss >> word;
            if ( word != "GET" )
            {
                std::cout << * this << " : Invalid method keyword from client -> killed" << std::endl;
                delete this;
                return false;
            }

            iss >> id;
            if ( id < 0 || ( uint32_t ) id != mpserver -> mid )
            {
                std::cout << * this << " : Invalid stream ID from client -> killed" << std::endl;
                std::cout << " Got: '" << id << "' ; Expected: '" << "'" << std::endl;
                delete this;
                return false;
            }

            iss >> word;
            if ( word != "LISTEN_PORT" )
            {
                std::cout << * this << " : Invalid listen_port keyword from client -> killed" << std::endl;
                delete this;
                return false;
            }

            uint16_t port;
            iss >> port;
            if ( port < VOD_MIN_PORT || port > VOD_MAX_PORT )
            {
                std::cout << * this << " : Invalid port (out of range) from client -> killed" << std::endl;
                delete this;
                return false;
            }

            iss >> word;

            if ( ! iss.eof ( ) )
            {
                std::cout << * this << " : Invalid word after connection request from client -> killed" << std::endl;
                delete this;
                return false;
            }

            if ( ! streamSocket.connectTo ( mip , port ) )
            {
                std::cout << * this << " : Data connection to client failed -> killed" << std::endl;
                delete this;
                return false;
            }

            mpserver -> notifier -> subscribe ( & streamSocket );
            connected = true;
            return true;
        }

        // Already connected
        iss >> word;
        if ( word == "END" )
        {
            std::cout << * this << " : Client wants to exit -> killed" << std::endl;
            delete this;
            return false;
        }
        if ( word != "GET" )
        {
            std::cout << * this << " : Invalid request from client -> killed" << std::endl;
            delete this;
            return false;
        }

        iss >> id;
        streamSocket.writing = true;
        return true;
    }

    bool ControlConnection::writeEventAction ( )
    {
        return true;
    }

    bool ControlConnection::exceptEventAction ( )
    {
        std::cout << * this << " : unexpected exception -> killed" << std::endl;
        delete this;
        return false;
    }

    bool ControlConnection::timeoutEventAction ( )
    {
        std::cout << * this << " : hasn't talked for a long time -> killed" << std::endl;
        delete this;
        return false;
    }

    void ControlConnection::chooseSubscription ( NetFlux::SocketIOEvent::Event & event )
    {
        event.setRead ( );
        event.setTimeout ( 5000000 );
    }

    void ControlConnection::toString ( std::ostream & os ) const
    {
        os << "TcpPull socket (";
        mpserver -> CatalogEntry::toTitleString ( os );
        os << ") ";
        NetFlux::Tcp::ServerStream::toString ( os );
    }
} }
