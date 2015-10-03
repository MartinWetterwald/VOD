#ifndef VOD_TCP_CLIENT_HPP_
#define VOD_TCP_CLIENT_HPP_

#include <Tcp/Tcp_Client.hpp>
#include "Vod_NetEvents.hpp"

namespace Vod
{
    class TcpClient : public NetFlux::Tcp::Client
    {
    public:
        TcpClient ( );
        virtual ~TcpClient ( );

        template < class CLASS1, class CLASS2 >
        inline void mapChooseSubscription
        (
            CLASS1 * pobject,
            void ( CLASS1::* pfunction ) ( CLASS2 *, NetFlux::SocketIOEvent::Event & )
        )
        {
            chooseSubscriptionMapping.map ( pobject, pfunction );
        }

        template < class CLASS1, class CLASS2 >
        inline void mapReadEvent
        (
            CLASS1 * pobject,
            bool ( CLASS1::* pfunction ) ( CLASS2 * )
        )
        {
            readEventMapping.map ( pobject, pfunction );
        }

        template < class CLASS1, class CLASS2 >
        inline void mapWriteEvent
        (
            CLASS1 * pobject,
            bool ( CLASS1::* pfunction ) ( CLASS2 * )
        )
        {
            writeEventMapping.map ( pobject, pfunction );
        }

        template < class CLASS1, class CLASS2 >
        inline void mapExceptEvent
        (
            CLASS1 * pobject,
            bool ( CLASS1::* pfunction ) ( CLASS2 * )
        )
        {
            exceptEventMapping.map ( pobject, pfunction );
        }

        template < class CLASS1, class CLASS2 >
        inline void mapTimeoutEvent
        (
            CLASS1 * pobject,
            bool ( CLASS1::* pfunction ) ( CLASS2 * )
        )
        {
            timeoutEventMapping.map ( pobject, pfunction );
        }


    protected:
        NetEvent::ChooseSubscriptionMapping chooseSubscriptionMapping;
        NetEvent::EventMapping readEventMapping;
        NetEvent::EventMapping writeEventMapping;
        NetEvent::EventMapping exceptEventMapping;
        NetEvent::EventMapping timeoutEventMapping;

        bool readEventAction ( );
        bool writeEventAction ( );
        bool exceptEventAction ( );
        bool timeoutEventAction ( );
        void chooseSubscription ( NetFlux::SocketIOEvent::Event & );

    private:
        TcpClient ( const TcpClient & ) = delete;
        TcpClient & operator= ( const TcpClient & ) = delete;
    };
}

#endif
