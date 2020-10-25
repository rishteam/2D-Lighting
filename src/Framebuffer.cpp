#pragma once
#include <Framebuffer.h>

Framebuffer::Framebuffer(const FramebufferSpecification &spec)
	: m_spec(spec)
{
    invalidate();
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &m_frameBufferID);
	glDeleteTextures(1, &m_colorAttachment);
	glDeleteTextures(1, &m_depthAttachment);
}

void Framebuffer::invalidate()
{
    if(m_frameBufferID)
	{
		glDeleteFramebuffers(1, &m_frameBufferID);
		glDeleteTextures(1, &m_colorAttachment);
		glDeleteTextures(1, &m_depthAttachment);
	}
    // Generate a framebuffer
	glCreateFramebuffers(1, &m_frameBufferID);
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);

    // Generate color attachment
	glCreateTextures(GL_TEXTURE_2D, 1, &m_colorAttachment);
	glBindTexture(GL_TEXTURE_2D, m_colorAttachment);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_spec.width, m_spec.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr); // specify settings of texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// future: gl4 API support
//	 glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, m_spec.width, m_spec.height);
	// glTextureParameteri(m_rendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTextureParameteri(m_rendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Attach the m_colorAttachment to the framebuffer
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttachment, 0);

    // Generate depth attachment
	glCreateTextures(GL_TEXTURE_2D, 1, &m_depthAttachment);
	glBindTexture(GL_TEXTURE_2D, m_depthAttachment);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_spec.width, m_spec.height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_depthAttachment, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferID);
	glViewport(0, 0, m_spec.width, m_spec.height);
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::resize(uint32_t width, uint32_t height)
{
	m_spec.width = width;
	m_spec.height = height;

	invalidate();
}

std::shared_ptr<Framebuffer> Framebuffer::Create(const FramebufferSpecification &spec)
{
    return std::make_shared<Framebuffer>(spec);
}
