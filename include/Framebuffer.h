/**
 * @file Framebuffer.h
 * @author icejj (lpc0503@gmail.com), roy4801 (me@roy4801.tw)
 * @brief Header of framebuffer
 * @version 0.1
 * @date 2020-07-22
 */
#pragma once

#include <pch.h>

/**
 * @brief Framebuffer Settings
 */
struct FramebufferSpecification
{
	uint32_t width{};
	uint32_t height{};
	uint32_t sample = 1;
};

/**
 * @brief Frame Buffer Object
 */
class Framebuffer
{
public:
    /**
     * @brief ctor
     * @param spec Framebuffer Settings
     */
	Framebuffer(const FramebufferSpecification &spec);

	/**
	 * @brief dtor
	 */
	~Framebuffer();

	/**
	 * @brief Recreate the framebuffer using the specification
	 */
	void invalidate();

	void bind() const;
	void unbind();

	void resize(uint32_t width, uint32_t height);

	uint32_t getColorAttachmentRendererID() const { return m_colorAttachment; }
	FramebufferSpecification getSpecification() const { return m_spec; }
	uint32_t getWidth() const { return m_spec.width; }
    uint32_t getHeight() const { return m_spec.height; }

	static std::shared_ptr<Framebuffer> Create(const FramebufferSpecification &spec);
private:
	uint32_t m_frameBufferID = 0;
	uint32_t m_colorAttachment = 0;
	uint32_t m_depthAttachment = 0;
	FramebufferSpecification m_spec;
};
