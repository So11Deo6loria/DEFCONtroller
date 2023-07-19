#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

#ifdef DEVBOARDBUILD
#include "main.h"
#endif

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

#ifdef DEVBOARDBUILD

#endif

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
