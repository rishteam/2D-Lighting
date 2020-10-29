//
// Created by lpc0503 on 2020/10/22.
//
#pragma once
#include <pch.h>

class Block {

public:

    Block(p2 pos, p2 size, p4 color = p4(1, 0, 0, 1));

    std::vector<p2> getVertices();

    void onUpdate(float dt);

    p2 pos;
    p2 size;
    p4 color;

    bool mouseTrace;

    std::string tag;

private:

    static int id;
};

