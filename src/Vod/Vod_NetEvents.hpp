#ifndef VOD_NET_EVENTS_HPP_
#define VOD_NET_EVENTS_HPP_

#include <SocketIOEvent/SocketIOEvent_Event.hpp>

namespace Vod
{
    namespace NetEvent
    {
        class thiscall
        {
        };

        class EventMapping
        {
        public:
            template < class CLASS1, class CLASS2 >
            inline void map
            (
                CLASS1 * pobject,
                bool ( CLASS1::* pfunction ) ( CLASS2 * )
            )
            {
                object = ( thiscall * ) pobject;
                function = ( bool ( thiscall::* ) ( void * ) ) pfunction;
            }

            template < class CLASS1 >
            bool operator ( ) ( CLASS1 * socket )
            {
                if ( object )
                {
                    return ( ( * object ).* function ) ( socket );
                }
                return false;
            }

            EventMapping ( );
            virtual ~EventMapping ( );


        protected:
            thiscall * object;
            bool ( thiscall::* function ) ( void * );



        };

        class ChooseSubscriptionMapping
        {
        public:
            template < class CLASS1, class CLASS2 >
            inline void map
            (
                CLASS1 * pobject,
                void ( CLASS1::* pfunction ) ( CLASS2 *, NetFlux::SocketIOEvent::Event & )
            )
            {
                object = ( thiscall * ) pobject;
                function = ( void ( thiscall::* ) ( void *, NetFlux::SocketIOEvent::Event &event ) ) pfunction;
            }


            template < class CLASS1 >
            void operator ( ) ( CLASS1 * socket, NetFlux::SocketIOEvent::Event & e )
            {
                if ( object )
                {
                    ( ( * object ).* function ) ( socket, e );
                }
            }

            ChooseSubscriptionMapping ( );
            virtual ~ChooseSubscriptionMapping ( );


        protected:
            thiscall * object;
            void ( thiscall::* function ) ( void *, NetFlux::SocketIOEvent::Event & );
        };
    }
}

#endif
