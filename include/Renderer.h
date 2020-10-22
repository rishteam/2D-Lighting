//
// Created by lpc0503 on 2020/10/15.
//
#pragma once
#ifndef RAY_CASTING_RENDERER_H
#define RAY_CASTING_RENDERER_H
#include <pch.h>
#include <shader.h>
#include <VertexArray.h>

class Renderer {

public:

    static void Init();

    static void DrawQuad(const p2 pos, const p2 size, const p4 color);

private:

    std::shared_ptr<Shader> shader;
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<IndexBuffer> indexBuffer;
};


#endif //RAY_CASTING_RENDERER_H
