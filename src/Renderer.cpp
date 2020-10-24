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

    std::shared_ptr<VertexArray> lightVertexArray;
    std::shared_ptr<Shader> lightShader;
    std::shared_ptr<Shader> irregularQuadShader;

    glm::mat4 projection = glm::ortho(0.0f, Game::Get().GetWidth(), Game::Get().GetHeight(), 0.f, -1.0f, 1.0f);
    std::shared_ptr<VertexArray> polygonVertexArray;

    std::shared_ptr<VertexBuffer> polygonVertexBuffer;
};

static std::unique_ptr<RendererData> data;

void Renderer::Init() {

    data = std::make_unique<RendererData>();
    data->shader = std::make_shared<Shader>("shader/2D-Lighting.vs", "shader/2D-Lighting.fs");
    data->lightShader = std::make_shared<Shader>("shader/light.vs", "shader/light.fs");
    data->irregularQuadShader = std::make_shared<Shader>("shader/irregularQuadShader.vs", "shader/irregularQuadShader.fs");
    data->polygonVertexArray = std::make_shared<VertexArray>();
    data->polygonVertexBuffer = std::make_shared<VertexBuffer>(12);
    data->vertexArray = std::make_shared<VertexArray>();
    data->lightVertexArray = std::make_shared<VertexArray>();
    data->vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
    BufferLayout layout = {
            {ShaderDataType::Float3, "aPos"}
    };
    data->vertexBuffer->setLayout(layout);
    data->vertexArray->addVertexBuffer(data->vertexBuffer);
    data->indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices)/sizeof(uint32_t));
    data->vertexArray->setIndexBuffer(data->indexBuffer);
    data->polygonVertexArray->setIndexBuffer(data->indexBuffer);
    data->lightVertexArray->setIndexBuffer(data->indexBuffer);
    data->polygonVertexArray->unbind();
    data->vertexArray->unbind();
    data->lightVertexArray->unbind();

    data->shader->bind();
    data->shader->setMat4("projection", data->projection);
    data->lightShader->bind();
    data->lightShader->setMat4("projection", data->projection);
    data->irregularQuadShader->bind();
    data->irregularQuadShader->setMat4("projection", data->projection);
}

void Renderer::DrawQuad(const p2 pos1, const p2 pos2, const p2 pos3, const p2 pos4, const p4 color) {

    float vertices_[] = {

            pos1.x, pos1.y, 0,
            pos2.x, pos2.y, 0,
            pos4.x, pos4.y, 0,
            pos3.x, pos3.y, 0
    };

    data->polygonVertexBuffer->setData(vertices_, sizeof(vertices_));
    BufferLayout layout = {
            {ShaderDataType::Float3, "aPos"}
    };

    data->polygonVertexBuffer->setLayout(layout);
    data->polygonVertexArray->addVertexBuffer(data->polygonVertexBuffer);

    data->irregularQuadShader->bind();
    data->irregularQuadShader->setFloat4("ourColor", color);

    glm::mat4 model(1.0f);
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
//
void Renderer::DrawLight(p2 pos1, p2 pos2, p2 pos3, p2 pos4, Light light) {

    float vertices_[] = {

        pos1.x, pos1.y, 0,
        pos2.x, pos2.y, 0,
        pos4.x, pos4.y, 0,
        pos3.x, pos3.y, 0
    };

    data->polygonVertexBuffer->setData(vertices_, sizeof(vertices_));
    BufferLayout layout = {
            {ShaderDataType::Float3, "aPos"}
    };

    data->polygonVertexBuffer->setLayout(layout);
    data->lightVertexArray->addVertexBuffer(data->polygonVertexBuffer);

    data->lightShader->bind();
    data->lightShader->setFloat4("lightColor", light.color);
    data->lightShader->setFloat2("lightPos", p2(light.pos.x, Game::Get().GetHeight() - light.pos.y));
    data->lightShader->setFloat("lightRadius", light.radius);
    data->lightVertexArray->bind();
    glDrawElements(GL_TRIANGLES, data->vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
    data->lightVertexArray->unbind();
}

void Renderer::DrawQuad(p2 pos1, p2 pos2, p2 pos3, p2 pos4) {


}

