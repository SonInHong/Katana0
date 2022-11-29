#pragma once
#include "CDoor.h"
class CNormalDoorRight :
    public CDoor
{
public:
    CNormalDoorRight();
    virtual ~CNormalDoorRight();

    virtual void Update();
};

