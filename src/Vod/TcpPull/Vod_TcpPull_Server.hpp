#ifndef _VOD_TCPPULL_SERVER_HPP_
#define _VOD_TCPPULL_SERVER_HPP_

#include <Tcp/Tcp_Server.hpp>

#include "../Vod_Server.hpp"
#include "Vod_TcpPull_ControlConnection.hpp"

namespace Vod
{
    namespace TcpPull
    {
        class Server : public NetFlux::Tcp::Server
        {
        public:
            Server ( Vod::Server * server );
            virtual ~Server ( );


        protected:
            virtual bool readEventAction ( );
            virtual bool writeEventAction ( );
            virtual bool exceptEventAction ( );

            virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & );


        private:
            std::unordered_set <ControlConnection *> mstreams;
            Vod::Server * mpvodServer;

            Server ( const Server & ) = delete;
            Server & operator= ( const Server & ) = delete;

            friend class Vod::Server;
            friend class ControlConnection;
        };
    }
}

#endif
