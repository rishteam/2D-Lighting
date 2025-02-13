//
// Created by lpc0503 on 2020/10/15.
//
#include <Renderer.h>
#include <Game.h>
#include <Vertices.h>
#include <OrthographicCamera.h>

p3 toNDC(p2 point){

    p3 res;

    res.x = (point.x - Game::Get().GetWidth()/2) / (Game::Get().GetWidth()/2);
    res.y = (Game::Get().GetHeight()/2 - point.y) / (Game::Get().GetHeight()/2);
    res.z = 0;

    return res;
}

struct RendererData {

    OrthographicCamera m_camera;
    p2 zoom;

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


    std::shared_ptr<VertexArray> textureVAO;
    std::shared_ptr<VertexBuffer> textureVBO;
    std::shared_ptr<Shader> textureShader;
    std::shared_ptr<Shader> singleTextureShader;
};

static std::unique_ptr<RendererData> data;

void Renderer::Init() {

    data = std::make_unique<RendererData>();
    data->shader = std::make_shared<Shader>("shader/2D-Lighting.vs", "shader/2D-Lighting.fs");
    data->lightShader = std::make_shared<Shader>("shader/light.vs", "shader/light.fs");
    data->irregularQuadShader = std::make_shared<Shader>("shader/irregularQuadShader.vs", "shader/irregularQuadShader.fs");


    data->polygonVertexArray = std::make_shared<VertexArray>();
    data->polygonVertexBuffer = std::make_shared<VertexBuffer>(18);
    data->lightVertexArray = std::make_shared<VertexArray>();

    data->vertexArray = std::make_shared<VertexArray>();
    data->vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));
    BufferLayout layout = {
            {ShaderDataType::Float3, "aPos"}
    };
    data->vertexBuffer->setLayout(layout);
    data->vertexArray->addVertexBuffer(data->vertexBuffer);

    data->indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices)/sizeof(uint32_t));
    data->vertexArray->setIndexBuffer(data->indexBuffer);
    data->lightVertexArray->setIndexBuffer(data->indexBuffer);
    data->polygonVertexArray->unbind();
    data->vertexArray->unbind();
    data->lightVertexArray->unbind();

    data->shader->bind();
    data->shader->setMat4("projection", data->m_camera.getProjectionMatrix());
    data->lightShader->bind();
    data->lightShader->setMat4("projection", data->m_camera.getProjectionMatrix());
    data->irregularQuadShader->bind();
    data->irregularQuadShader->setMat4("projection", data->m_camera.getProjectionMatrix());

    data->textureVAO = std::make_shared<VertexArray>();
    data->textureVBO = std::make_shared<VertexBuffer>(24);
    data->textureShader = std::make_shared<Shader>("shader/texture.vs", "shader/texture.fs");
    data->textureShader->bind();
    data->textureShader->setInt("lightTexture", 0);
    data->textureShader->setInt("worldGeometry", 1);
    data->textureVAO->unbind();
    data->singleTextureShader = std::make_shared<Shader>("shader/singleTexture.vs", "shader/singleTexture.fs");
    data->singleTextureShader->bind();
    data->singleTextureShader->setInt("screenTexture", 4);
    data->singleTextureShader->setMat4("projection", data->m_camera.getProjectionMatrix());
}

void Renderer::setCamera(OrthographicCamera& camera, p2 zoom) {

    data->m_camera = camera;
    data->zoom = zoom;
}

void Renderer::DrawQuad(const p2 pos1, const p2 pos2, const p2 pos3, const p2 pos4, const p4 color) {

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions
            pos1.x, pos1.y, 0,
            pos2.x, pos2.y, 0,
            pos3.x, pos3.y, 0,

            pos1.x, pos1.y, 0,
            pos3.x, pos3.y, 0,
            pos4.x, pos4.y, 0
        };

    data->polygonVertexBuffer->setData(quadVertices, sizeof(quadVertices));
    BufferLayout layout = {
            {ShaderDataType::Float3, "aPos"}
    };

    data->polygonVertexBuffer->setLayout(layout);
    data->polygonVertexArray->addVertexBuffer(data->polygonVertexBuffer);

    data->irregularQuadShader->bind();
    data->irregularQuadShader->setFloat4("ourColor", color);
    data->irregularQuadShader->setMat4("projection", data->m_camera.getViewProjectionMatrix());

    data->polygonVertexArray->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
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
        pos1.x, pos1.y, 0, light.pos.x, light.pos.y,
        pos2.x, pos2.y, 0, light.pos.x, light.pos.y,
        pos4.x, pos4.y, 0, light.pos.x, light.pos.y,
        pos3.x, pos3.y, 0, light.pos.x, light.pos.y
    };

    data->polygonVertexBuffer->setData(vertices_, sizeof(vertices_));
    BufferLayout layout = {
            {ShaderDataType::Float3, "aPos"},
            {ShaderDataType::Float2, "pos"}
    };

    data->polygonVertexBuffer->setLayout(layout);
    data->lightVertexArray->addVertexBuffer(data->polygonVertexBuffer);

    data->lightShader->bind();
    data->lightShader->setFloat4("lightColor", light.color);
    data->lightShader->setFloat("lightRadius", light.radius);
    data->lightShader->setFloat("constant", light.constant);
    data->lightShader->setFloat("linear", light.linear);
    data->lightShader->setFloat("quadratic", light.quadratic);
    data->lightShader->setMat4("projection", data->m_camera.getViewProjectionMatrix());
    data->lightShader->setFloat("zoomx", data->zoom.x);
    data->lightShader->setFloat("zoomy", data->zoom.y);
    data->lightVertexArray->bind();
    glDrawElements(GL_TRIANGLES, data->vertexArray->getIndexBuffer()->getCount(), GL_UNSIGNED_INT, nullptr);
    data->lightVertexArray->unbind();
}

void Renderer::BindTexture(p2 center, p2 size, uint32_t id, uint32_t id2) {

    p2 si = size;

    float quadVertices[] = {
        // positions   // texCoords
        center.x - si.x, center.y + si.y,  0.0f, 1.0f,
        center.x - si.x, center.y - si.y,  0.0f, 0.0f,
        center.x + si.x, center.y - si.y,  1.0f, 0.0f,

        center.x - si.x, center.y + si.y,  0.0f, 1.0f,
        center.x + si.x, center.y - si.y,  1.0f, 0.0f,
        center.x + si.x, center.y + si.y,  1.0f, 1.0f
    };


    data->textureVBO->setData(quadVertices, sizeof(quadVertices));
    BufferLayout layout2 = {
            {ShaderDataType::Float2, "aPos"},
            {ShaderDataType::Float2, "aTexCoord"}
    };
    data->textureVBO->setLayout(layout2);
    data->textureVAO->addVertexBuffer(data->textureVBO);

    data->textureShader->bind();
    data->textureShader->setMat4("projection", data->m_camera.getViewProjectionMatrix());
    // Bind Texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, id2);

    // Draw QUad
    data->textureVAO->bind();
    glDrawArrays(GL_TRIANGLES, 0, 6);
    data->textureVAO->unbind();
}

void Renderer::DrawSingleTexture(p2 pos1, p2 pos2, p2 pos3, p2 pos4, uint32_t id) {

    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            pos1.x, pos1.y, 0.0f, 1.0f,
            pos2.x, pos2.y, 0.0f, 0.0f,
            pos3.x, pos3.y, 1.0f, 0.0f,

            pos1.x, pos1.y, 0.0f, 1.0f,
            pos3.x, pos3.y, 1.0f, 0.0f,
            pos4.x, pos4.y, 1.0f, 1.0f
    };

    data->textureVBO->setData(quadVertices, sizeof(quadVertices));
    BufferLayout layout2 = {
            {ShaderDataType::Float2, "aPos"},
            {ShaderDataType::Float2, "aTexCoord"}
    };
    data->textureVBO->setLayout(layout2);
    data->textureVAO->addVertexBuffer(data->textureVBO);

    data->singleTextureShader->bind();
    data->singleTextureShader->setMat4("projection", data->m_camera.getViewProjectionMatrix());
    data->textureVAO->bind();
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    data->textureVAO->unbind();
}
