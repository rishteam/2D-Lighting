//
// Created by lpc0503 on 2020/10/22.
//
#pragma once
#ifndef INC_2D_LIGHTING_VERTICES_H
#define INC_2D_LIGHTING_VERTICES_H

#include <pch.h>

float vertices[] = {
        0.5f, 0.5f, 0.0f,  // 右上角
        0.5f, -0.5f, 0.0f, // 右下角
        -0.5f, -0.5f, 0.0f, // 左下角
        -0.5f, 0.5f, 0.0f  // 左上角
};

uint32_t indices[] = {
        0, 1, 3, // 第一個三角形
        1, 2, 3  // 第二個三角形
};

#endif //INC_2D_LIGHTING_VERTICES_H
