//
// Created by lpc0503 on 2020/10/22.
//
#pragma once
#ifndef INC_2D_LIGHTING_VERTICES_H
#define INC_2D_LIGHTING_VERTICES_H

#include <pch.h>

//float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
//        // positions   // texCoords
//        -1.0f,  1.0f,  0.0f, 1.0f,
//        -1.0f, -1.0f,  0.0f, 0.0f,
//        1.0f, -1.0f,  1.0f, 0.0f,
//
//        -1.0f,  1.0f,  0.0f, 1.0f,
//        1.0f, -1.0f,  1.0f, 0.0f,
//        1.0f,  1.0f,  1.0f, 1.0f
//};

float vertices[] = {
        -0.5f, 0.5f, 0.0f,  // 左上角
        -0.5f, -0.5f, 0.0f, // 左下角
        0.5f, 0.5f, 0.0f,  // 右上角
        0.5f, -0.5f, 0.0f // 右下角
};

uint32_t indices[] = {
        0, 1, 2, // 第一個三角形
        1, 2, 3  // 第二個三角形
};

#endif //INC_2D_LIGHTING_VERTICES_H
