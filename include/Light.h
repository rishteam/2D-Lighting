//
// Created by icejj on 2020/10/15.
//
#pragma once

#ifndef RAY_CASTING_LIGHT_H
#define RAY_CASTING_LIGHT_H
#include <pch.h>

class Light {

public:

    Light(p2 pos, p4 color, float radius);

    void onUpdate(float dt);

    p2 pos;
    p4 color;
    float radius;

private:
};




#endif //RAY_CASTING_LIGHT_H
