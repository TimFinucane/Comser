#include "Component.h"

// Effectively static
unsigned int _counter = 0;

unsigned int Comser::assignId()
{
    return _counter++;
}
