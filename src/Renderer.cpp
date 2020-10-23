//
// Created by lpc0503 on 2020/10/15.
//
#include <Renderer.h>
#include <Game.h>
#include <Vertices.h>

p3 toNDC(p2 point){

    p3 res;

    res.x = (point.x - Game::Get().GetWidth()/2) / (Game::Get().GetWidth()/2);
    res.y = (Game::Get().GetHeight()/2 - point.y) / (Game::Get().GetHeight()/2);
    res.z = 0;

    return res;
}

struct RendererData {

    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<Shader> shader;
    std::shared_ptr<IndexBuffer> indexBuffer;

    std::shared_ptr<Shader> lightShader;


    std::shared_ptr<VertexArray> polygonVertexArray;
    std::shared_ptr<VertexBuffer> polygonVertexBuffer;

    glm::mat4 projection = glm::ortho(0.0f, Game::Get().GetWidth(), Game::Get().GetHeight(), 0.f, -1.0f, 1.0f);
};

static std::unique_ptr<RendererData> data;

void Renderer::Init() {

    data = std::make_unique<RendererData>();
    data->shader = std::make_shared<Shader>("shader/2D-Lighting.vs", "shader/2D-Lighting.fs");
    data->lightShader = std::make_shared<Shader>("shader/light.vs", "shader/light.fs");
    data->polygonVertexArray = std::make_shared<VertexArray>();
    data->polygonVertexBuffer = std::make_shared<VertexBuffer>(12);
    data->vertexArray = std::make_shared<VertexArray>();
    data->vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
    BufferLayout layout = {
            {ShaderDataType::Float3, "aPos"}
    };
    data->vertexBuffer->setLayout(layout);
    data->vertexArray->addVertexBuffer(data->vertexBuffer);
    data->indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices)/sizeof(uint32_t));
    data->vertexArray->setIndexBuffer(data->indexBuffer);
    data->polygonVertexArray->setIndexBuffer(data->indexBuffer);
    data->polygonVertexArray->unbind();
    data->vertexArray->unbind();

    data->shader->bind();
    data->shader->setMat4("projection", data->projection);
    data->lightShader->bind();
    data->lightShader->setMat4("projection", data->projection);
}

void Renderer::DrawQuad(const p2 pos1, const p2 pos2, const p2 pos3, const p2 pos4, const p4 color) {

    p3 res1 = toNDC(pos1);
    p3 res2 = toNDC(pos2);
    p3 res3 = toNDC(pos3);
    p3 res4 = toNDC(pos4);

    float vertices_[] = {
            -0.5f,  0.5f, 0.0f,  // 左上角
            -0.5f, -0.5f, 0.0f, // 左下角
             0.5f,  0.5f, 0.0f,  // 右上角
             0.5f, -0.5f, 0.0f // 右下角
    };
//
//    float vertices_[] = {
//
//            res1.x, res1.y, 0,
//            res2.x, res2.y, 0,
//            res3.x, res3.y, 0,
//            res4.x, res4.y, 0
//    };

    data->polygonVertexBuffer->setData(vertices_, sizeof(vertices_));
    BufferLayout layout = {
            {ShaderDataType::Float3, "aPos"}
    };

    data->polygonVertexBuffer->setLayout(layout);
    data->polygonVertexArray->addVertexBuffer(data->polygonVertexBuffer);

    data->shader->bind();
    data->shader->setFloat4("ourColor", color);

    glm::mat4 model(1.0f);
    model = glm::translate(model, p3(100, 100, 0));
    model = glm::scale(model, p3(200, 200, 0));
    data->shader->setMat4("model", model);
    data->polygonVertexArray->bind();
    glDrawElements(GL_TRIANGLES, data->polygonVertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
    data->polygonVertexArray->unbind();
}

void Renderer::DrawQuad(p2 pos, p2 size, p4 color) {

    data->shader->bind();
    data->shader->setFloat4("ourColor", color);

    glm::mat4 model(1.0f);
    model = glm::translate(model, p3(pos, 0));
    model = glm::scale(model, p3(size, 0));

    data->shader->setMat4("model", model);
    data->vertexArray->bind();
    glDrawElements(GL_TRIANGLES, data->vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
    data->vertexArray->unbind();
}

void Renderer::DrawLight(const p2 pos, const p4 color) {

    data->lightShader->bind();
    data->shader->setFloat4("lightColor", color);

    glm::mat4 model(1.0f);
    model = glm::translate(model, p3(pos, 0));
    data->shader->setMat4("model", model);
    data->vertexArray->bind();
    glDrawElements(GL_TRIANGLES, data->vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
    data->vertexArray->unbind();
}

