#pragma once
#include "CDoor.h"
class CNormalDoorLeft :
    public CDoor
{
public:
    CNormalDoorLeft();
    virtual ~CNormalDoorLeft();

    virtual void Update();
};

