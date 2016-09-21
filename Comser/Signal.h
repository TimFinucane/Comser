#pragma once
#ifndef SIGNAL_H
#define SIGNAL_H

#include <sigc++/sigc++.h>

namespace Comser
{
    namespace Event
    {
        namespace Signal
        {
            using   Default         = sigc::signal1<void,void*>;

            using   Signal0         = sigc::signal0<void>;
            using   Slot0           = sigc::slot0<void>;

            /*template< typename RET, class CLASS >
            using   MemberSlot0     = sigc::mem_fun0<RET, CLASS>;
            template< typename ARG, typename RET, class CLASS >
            using   MemberSlot1     = sigc::mem_fun1<ARG, RET, CLASS>;
            template< typename ARG1, typename ARG2, typename RET, class CLASS >
            using   MemberSlot2     = sigc::mem_fun2<ARG1, ARG2, RET, CLASS>;*/

            template< typename T >
            using   Signal1         = sigc::signal1<void,T>;
            template< typename T >
            using   Slot1           = sigc::slot1<void,T>;

            template< typename T, typename U >
            using   Signal2         = sigc::signal2<void,T,U>;
            template< typename T, typename U >
            using   Slot2           = sigc::slot2<void,T,U>;
        }
    }
}

#endif