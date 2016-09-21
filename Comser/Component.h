#pragma once
#ifndef COMPONENT_H
#define COMPONENT_H

namespace Comser
{
    // A type of component (not a reference to a single one)
    typedef unsigned __int32            ComponentType;

    class Component abstract
    {
    public:
        // So whenever a Component* gets deleted, it deconstructs properly
        virtual ~Component() = 0 {}; // How I love some of the intricacies of C++
    };

    unsigned int assignId();
}

// This should be written in every Component derived class
#define COMSER_COMPONENT_DECLARE                            \
    public:                                                 \
        static Comser::ComponentType id(){ return _id; }    \
    private:                                                \
        static Comser::ComponentType _id;

#define COMSER_COMPONENT_INSTANTIATE(classname) Comser::ComponentType classname::_id = Comser::assignId();

#endif