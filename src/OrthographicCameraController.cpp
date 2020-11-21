//
// Created by icejj on 2020/11/19.
//
#include <OrthographicCameraController.h>

OrthographicCameraController::OrthographicCameraController(float aspect, bool rotate, bool moveByKeyboard)
        : m_aspect(aspect),
          m_zoom(3.f),
          m_bounds(-m_zoom, m_zoom, -m_zoom, m_zoom),
          m_camera(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top)
{
    m_isAbleToRotate = rotate;
    m_enableKeyboardMove = moveByKeyboard;
}

void OrthographicCameraController::onUpdate(float dt)
{
    // Early out if it is not enabled
//    if(!m_enableState)
//        return;

    if(m_enableKeyboardMove) {

        float rads = m_camera.getRotationRadians();
        // TODO: refactor these
        glm::mat2 rotationMatrix = {
                std::cos(rads), std::sin(rads),
                -std::sin(rads), std::cos(rads)
        };
        glm::vec2 dir[4] = {
                {0.f,  1.f},  // W
                {0.f,  -1.f},  // S
                {-1.f, 0.f},  // A
                {1.f,  0.f}   // D
        };
        for (int i = 0; i < 4; i++) {
            dir[i] = rotationMatrix * dir[i];
            dir[i] *= m_translateSpeed;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            m_position += glm::vec3(dir[0], 0.f) * dt;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            m_position += glm::vec3(dir[1], 0.f) * dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            m_position += glm::vec3(dir[2], 0.f) * dt;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            m_position += glm::vec3(dir[3], 0.f) * dt;

    }
    m_camera.setPosition(m_position);

    if(!m_isAbleToRotate)
        return;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
    {
        m_rotate -= m_rotateSpeed * dt;
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        m_rotate += m_rotateSpeed * dt;
    }
    if(m_rotate > 360.f || m_rotate < -360.f)
        m_rotate = std::fmod(m_rotate, 360.f);

    m_camera.setRotation(m_rotate);
}

//void OrthographicCameraController::onEvent(Event &e)
//{
//    EventDispatcher dispatcher(e);
//    dispatcher.dispatch<MouseScrolledEvent>(RL_BIND_EVENT_FUNC(OrthographicCameraController::onMouseScrolled));
//    dispatcher.dispatch<WindowResizeEvent>(RL_BIND_EVENT_FUNC(OrthographicCameraController::onWindowResized));
//}

void OrthographicCameraController::onImGuiRender()
{
    ImGui::Begin("OrthographicCameraController");
    ImGui::Text("Aspect: %.2f",m_aspect);
    ImGui::Checkbox("Enable Camera", &m_enableState);
    ImGui::Checkbox("Enable Move by Keyboard", &m_enableKeyboardMove);
    //
    if(ImGui::DragFloat("Zoom", &m_zoom, 0.1))
    {
        invalidate();
    }
    //
    ImGui::DragFloat3("Position", glm::value_ptr(m_position));
    ImGui::DragFloat("Translate Speed", &m_translateSpeed);
    //
    ImGui::Checkbox("Enable Rotate", &m_isAbleToRotate);
    ImGui::DragFloat("Rotate", &m_rotate);
    ImGui::DragFloat("Rotate Speed", &m_rotateSpeed);
    //

    auto &cam = m_camera.getViewProjectionMatrix();
    ImGui::Text("View Projection Matrix");
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            ImGui::PushID(&cam[i][j]);
            ImGui::Text("%.2f", static_cast<double>(cam[i][j])); ImGui::SameLine();
            ImGui::PopID();
        }
        ImGui::NewLine();
    }
    ImGui::End();
}

void OrthographicCameraController::invalidate()
{
    m_bounds = OrthographicCameraBounds{-m_zoom, m_zoom, -m_zoom, m_zoom};
    m_camera.resizeCamera(m_bounds.left, m_bounds.right, m_bounds.bottom, m_bounds.top);
}

void OrthographicCameraController::onResize(float width, float height)
{
    m_aspect = (float)width / (float)height;
    //
    invalidate();
}

//bool OrthographicCameraController::onMouseScrolled(MouseScrolledEvent &e)
//{
//    m_zoom -= e.yOffset * zoomSpeed();
//    m_zoom = std::max(m_zoom, 0.20f);
//    //
//    invalidate();
//    return false;
//}
//
//bool OrthographicCameraController::onWindowResized(WindowResizeEvent &e)
//{
//    onResize(e.m_width, e.m_height);
//    return false;
//}

float OrthographicCameraController::zoomSpeed() const
{
    if(m_zoom < 1.f)
        return 0.05f;
    else if(m_zoom < 10.f)
        return 0.2f;
    else if(m_zoom < 50.f)
        return 0.5f;
    else if(m_zoom < 100.f)
        return 1.f;
    else
        return 5.f;
}
