#pragma once

#include "ecs_include.h"

struct Component1 : public ECS::Component<Component1>
{
    int value;
};

struct Component2 : public ECS::Component<Component2>
{
    float foo;
    char bar[32];
};
