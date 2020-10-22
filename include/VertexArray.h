#pragma once

#include <memory>
#include <cstdint>
#include <cassert>

#include "buffer.h"

GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type);

class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    void bind() const;
    void unbind() const;

    void addVertexBuffer(std::shared_ptr<VertexBuffer>& vertexBuf);
    void setIndexBuffer(std::shared_ptr<IndexBuffer>& indexBuf);

    const std::shared_ptr<VertexBuffer>& getVertexBuffer() const { return m_vertexBuffer; }
    const std::shared_ptr<IndexBuffer>& getIndexBuffer() const { return m_indexBuffer; }
private:
    uint32_t vao;
    std::shared_ptr<VertexBuffer> m_vertexBuffer = nullptr;
    std::shared_ptr<IndexBuffer> m_indexBuffer = nullptr;
    uint32_t m_vertexBufferIndex = 0;
};
