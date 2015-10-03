#ifndef VOD_TCPPUSH_SERVER_HPP_
#define VOD_TCPPUSH_SERVER_HPP_

#include <Tcp/Tcp_Server.hpp>
#include <unordered_set>

#include "../Vod_CatalogEntry.hpp"
#include "Vod_TcpPush_ControlConnection.hpp"

namespace Vod
{
    namespace TcpPush
    {
        class Server : public NetFlux::Tcp::Server, public CatalogEntry
        {
        public:
            Server ( ) = default;
            virtual ~Server ( );

            void toTitleString ( std::ostream & os ) const;
            bool start ( );

        protected:
            virtual bool readEventAction ( );
            virtual bool writeEventAction ( );
            virtual bool exceptEventAction ( );

            virtual void chooseSubscription ( NetFlux::SocketIOEvent::Event & );


        private:
            std::unordered_set <ControlConnection *> mstreams;

            Server ( const Server & ) = delete;
            Server & operator= ( const Server & ) = delete;

            friend class ControlConnection;
        };
    }
}

#endif
