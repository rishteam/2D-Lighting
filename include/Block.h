//
// Created by lpc0503 on 2020/10/22.
//
#pragma once
#ifndef INC_2D_LIGHTING_BLOCK_H
#define INC_2D_LIGHTING_BLOCK_H
#include <pch.h>
#include <Game.h>

class Block {

public:

    Block(p2 pos, float width, float height);

    std::vector<p2> getVertices();

private:

    p2 pos;
    float width;
    float height;
};

#endif //INC_2D_LIGHTING_BLOCK_H
