//
// Created by icejj on 2020/10/15.
//
#pragma once

#ifndef RAY_CASTING_LIGHT_H
#define RAY_CASTING_LIGHT_H
#include <pch.h>

class Light {

public:

    Light(p2 pos, p4 color, float radius, float constant = 1.0f, float linear = 0.014, float quadratic = 0.0007);

    void onUpdate(float dt);

    p2 pos;
    p4 color;
    float radius;
    float constant;
    float linear;
    float quadratic;

    std::string tag;

    bool traceMouse = false;

private:

    static int id;
};




#endif //RAY_CASTING_LIGHT_H
