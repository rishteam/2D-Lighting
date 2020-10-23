//
// Created by lpc0503 on 2020/10/22.
//
#pragma once
#include <pch.h>

class Block {

public:

    Block(p2 pos, p2 size);

    std::vector<p2> getVertices();

    p2 pos;
    p2 size;

private:
};

