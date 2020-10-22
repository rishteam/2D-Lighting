//
// Created by lpc0503 on 2020/10/15.
//
#include <Renderer.h>
#include <Game.h>
#include <Vertices.h>

struct RendererData {

    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<IndexBuffer> indexBuffer;
};

static std::unique_ptr<RendererData> data;

void Renderer::Init() {

    data = std::make_unique<RendererData>();
    data->shader = std::make_shared<Shader>("shader/2D-Lighting.vs", "shader/2D-Lighting.fs");
    data->vertexArray = std::make_shared<VertexArray>();
    data->vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
    BufferLayout layout = {
            {ShaderDataType::Float3, "aPos"}
    };
    data->vertexBuffer->setLayout(layout);
    data->vertexArray->addVertexBuffer(data->vertexBuffer);
    data->indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices)/sizeof(uint32_t));
    data->vertexArray->setIndexBuffer(data->indexBuffer);

    data->vertexArray->unbind();
}

void Renderer::DrawQuad(p2 pos, p2 size, p4 color) {

    data->shader->bind();
    data->shader->setFloat4("ourColor", color);
    data->vertexArray->bind();
    glDrawElements(GL_TRIANGLES, data->vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
    data->vertexArray->unbind();
}

