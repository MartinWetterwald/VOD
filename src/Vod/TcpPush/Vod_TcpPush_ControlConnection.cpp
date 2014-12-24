#include "Vod_TcpPush_ControlConnection.hpp"
#include "Vod_TcpPush_Server.hpp"
#include "../Vod_parsingUtils.hpp"

#include <iostream>
#include <cstring>
#include <sstream>

namespace Vod {
namespace TcpPush
{
    ControlConnection::ControlConnection ( int sock, const NetFlux::Net::InetAddress & address )
        : Request ( sock, address ), mpserver ( nullptr ), currentFrame ( 1 ), connected ( false ),
        streamSocketWriting ( false )
    {
        streamSocket.mapReadEvent ( this, & ControlConnection::streamSocketReadEventAction );
        streamSocket.mapWriteEvent ( this, & ControlConnection::streamSocketWriteEventAction );
        streamSocket.mapExceptEvent ( this, & ControlConnection::streamSocketExceptEventAction );
        streamSocket.mapTimeoutEvent ( this, & ControlConnection::streamSocketTimeoutEventAction );
        streamSocket.mapChooseSubscription ( this, & ControlConnection::streamSocketChooseSubscription );
    }

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

        std::string word, word2;
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

            notifier -> subscribe ( & streamSocket );
            connected = true;
            return true;
        }

        // Already connected
        iss >> word;
        iss >> word2;
        if ( ! iss.eof ( ) )
        {
            std::cout << * this << " : Invalid word after request from client -> killed" << std::endl;
            delete this;
            return false;
        }

        if ( word == "END" )
        {
            std::cout << * this << " : Client wants to exit -> killed" << std::endl;
            delete this;
            return false;
        }

        if ( word != "START" && word != "PAUSE" )
        {
            std::cout << * this << " : Invalid request from client -> killed" << std::endl;
            delete this;
            return false;
        }

        std::cout << * this << " : Requested action: " << word << std::endl;

        if ( word == "START" )
        {
            std::cout << * this << " : Frame to be sent: " << currentFrame << std::endl;
            streamSocketWriting = true;
        }
        else if ( word == "PAUSE" )
        {
            streamSocketWriting = false;
        }

        return true;
    }

    bool ControlConnection::writeEventAction ( ) { return true; }

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
        event.setExcept ( );
        event.setTimeout ( 100000000 );
    }

    bool ControlConnection::streamSocketReadEventAction ( TcpClient * )
    {
        std::cout << * this << " : Client has talked on the data connection -> killed" << std::endl;
        delete this;
        return false;
    }

    bool ControlConnection::streamSocketWriteEventAction ( TcpClient * streamSock )
    {
        if ( buffer.data ( ) == nullptr )
        {
            if ( ! mpserver -> loadFrame ( currentFrame, buffer ) )
            {
                std::cout << * this << " : Unable to load frame " << currentFrame << " -> killed" << std::endl;
                delete this;
                return false;
            }
        }

        uint64_t sizeLeft;
        char * data = buffer.leftDataPointer ( sizeLeft );
        ssize_t sent = streamSock -> send ( data, sizeLeft );
        if ( sent == INVALID )
        {
            std::cout << * this << " : Sending error on the data connection -> killed" << std::endl;
            delete this;
            return false;
        }

        buffer.acquit ( ( size_t ) sent );
        if ( buffer.end ( ) )
        {
            buffer.deallocate ( );

            if ( currentFrame == mpserver -> frameTotal ( ) )
            {
                currentFrame = 1;
            }
            else
            {
                ++currentFrame;
            }
        }
        return true;
    }

    bool ControlConnection::streamSocketExceptEventAction ( TcpClient * )
    {
        std::cout << * this << " : unexpected exception on the data connection -> killed" << std::endl;
        delete this;
        return false;
    }

    bool ControlConnection::streamSocketTimeoutEventAction ( TcpClient * ) { return true; }

    void ControlConnection::streamSocketChooseSubscription ( TcpClient *, NetFlux::SocketIOEvent::Event & e )
    {
        e.setRead ( );
        e.setExcept ( );

        if ( streamSocketWriting )
        {
            e.setWrite ( );
        }
    }


    void ControlConnection::toString ( std::ostream & os ) const
    {
        os << "TcpPush socket (";
        mpserver -> CatalogEntry::toTitleString ( os );
        os << ") ";
        NetFlux::Tcp::ServerStream::toString ( os );
    }
} }
