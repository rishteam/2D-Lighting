//
// Created by icejj on 2020/11/19.
//
#pragma once

#ifndef INC_2D_LIGHTING_ORTHOGRAPHICCAMERACONTROLLER_H
#define INC_2D_LIGHTING_ORTHOGRAPHICCAMERACONTROLLER_H

#include <pch.h>
#include <OrthographicCamera.h>

struct OrthographicCameraBounds
{
    OrthographicCameraBounds(float Left, float Right, float Bottom, float Top)
    {
        left = Left;
        right = Right;
        bottom = Bottom;
        top = Top;
    }

    float left, right;
    float top, bottom;

    float getWidth() const { return right - left; }
    float getHeight() const { return top - bottom; }
};

/**
 * @brief Orthographic camera controller
 */
class OrthographicCameraController
{
    public:
    OrthographicCameraController(float aspect, bool rotate=false, bool moveByKeyboard=false);
    ///////////////////////////////////////
    // Main functions
    ///////////////////////////////////////
    void onUpdate(float dt);
//    void onEvent(Event &e);
    void onImGuiRender();

    ///////////////////////////////////////
    // Events
    ///////////////////////////////////////
    void onResize(float width, float height);

    void setState(bool state) { m_enableState = state; }
    bool getState() const     { return m_enableState; }

    OrthographicCamera& getCamera()                   { return m_camera; }
    const OrthographicCamera& getCamera() const       { return m_camera; }
    //
    const OrthographicCameraBounds& getBounds() const { return m_bounds; }
    //
    float getRotate() const                           { return m_rotate; }
    glm::vec3 getPosition() const                     { return m_position; }
    void setPosition(const glm::vec2 &p)              { m_position = glm::vec3(p, 0.f); invalidate(); /*m_camera.setPosition(m_position);*/ }
    float getZoom() const                             { return m_zoom; }
    void setZoom(float z)                             { m_zoom = z; invalidate(); }
    //
    void move(const glm::vec2 &p)                     { m_position += glm::vec3(p, 0.f); }

    // Public fields
    float m_translateSpeed = 1.5f;
    float m_rotateSpeed = 180.f;
    private:
//    bool onMouseScrolled(MouseScrolledEvent &e);
//    bool onWindowResized(WindowResizeEvent &e);
    //
    void invalidate();
    //
    bool m_enableState        = true;  ///< Is the CameraController enable?
    bool m_enableKeyboardMove = true; ///< Is the keyboard be able to move the CameraController
    float m_aspect = 1280.f / 720.f;
    // Zoom
    float m_zoom = .5f;
    float zoomSpeed() const;
    //
    OrthographicCameraBounds m_bounds;
    OrthographicCamera m_camera;

    bool m_isAbleToRotate;
    float m_rotate = 0.f;  // degrees, counter-clock wise
    glm::vec3 m_position{0.f, 0.f, 0.f};
};
#endif //INC_2D_LIGHTING_ORTHOGRAPHICCAMERACONTROLLER_H
