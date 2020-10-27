//
// Created by lpc0503 on 2020/10/15.
//
#pragma once
#ifndef RAY_CASTING_RENDERER_H
#define RAY_CASTING_RENDERER_H
#include <pch.h>
#include <shader.h>
#include <VertexArray.h>
#include <Light.h>

class Renderer {

public:

    static void Init();

    static void DrawQuad(const p2 pos, const p2 size, const p4 color);

    static void DrawQuad(const p2 pos1, const p2 pos2, const p2 pos3, const p2 pos4, const p4 color);

    static void DrawLight(p2 pos1, p2 pos2, p2 pos3, p2 pos4, Light light);

    static void DrawTexture(uint32_t id, uint32_t id2, uint32_t id3);

    static void DrawSingleTexture(uint32_t id);

private:

    static void DrawQuad(p2 pos1, p2 pos2, p2 pos3, p2 pos4);
};


#endif //RAY_CASTING_RENDERER_H
