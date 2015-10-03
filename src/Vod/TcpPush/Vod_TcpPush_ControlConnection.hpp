#ifndef VOD_TCPPUSH_STREAM_HPP_
#define VOD_TCPPUSH_STREAM_HPP_

#include "../Vod_Request.hpp"
#include "../Vod_TcpClient.hpp"
#include "../Vod_Buffer.hpp"

namespace Vod
{
    namespace TcpPush
    {
        class Server;

        class ControlConnection : public Request
        {
        public:
            ControlConnection ( int sock, const NetFlux::Net::InetAddress & address );
            ~ControlConnection ( );

        protected:
            virtual bool requestEventAction ( );
            virtual bool writeEventAction ( );
            virtual bool exceptEventAction ( );

            virtual bool timeoutEventAction ( );
            virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & event );

            void toString ( std::ostream & os ) const;

            Server * mpserver;

            Buffer buffer;
            unsigned int currentFrame;

            bool connected;
            TcpClient streamSocket;
            bool streamSocketWriting;

            bool streamSocketReadEventAction ( TcpClient * socket );
            bool streamSocketWriteEventAction ( TcpClient * socket );
            bool streamSocketExceptEventAction ( TcpClient * socket );
            bool streamSocketTimeoutEventAction ( TcpClient * socket );
            void streamSocketChooseSubscription ( TcpClient * socket, NetFlux::SocketIOEvent::Event & event);

        private:
            ControlConnection ( ) = delete;
            ControlConnection ( const ControlConnection & ) = delete;
            ControlConnection & operator= ( const ControlConnection & ) = delete;


            friend class Server;
        };
    }
}

#endif


