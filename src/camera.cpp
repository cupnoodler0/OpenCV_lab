#include "camera.h"

Camera::Camera( glm::vec3 position, glm::vec3 up, float yaw, float pitch ) {
    m_Position = position;
    m_WorldUp = up;
    m_Yaw = yaw;
    m_Pitch = pitch;
    UpdateCameraVectors();
}

Camera::Camera( float posX, float posY, float posZ, float upX, float upY,
                float upZ, float yaw, float pitch ) {
    m_Position = glm::vec3( posX, posY, posZ );
    m_WorldUp = glm::vec3( upX, upY, upZ );
    m_Yaw = yaw;
    m_Pitch = pitch;
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt( m_Position, m_Position + m_Front, m_Up );
}

void Camera::ProcessKeyboard( CameraMovement direction, float deltaTime ) {
    float velocity = m_MovementSpeed * deltaTime;
    if ( direction == FORWARD ) {
        m_Position += m_Front * velocity;
    }
    if ( direction == BACKWARD ) {
        m_Position -= m_Front * velocity;
    }
    if ( direction == RIGHT ) {
        m_Position += m_Right * velocity;
    }
    if ( direction == LEFT ) {
        m_Position -= m_Right * velocity;
    }
}

void Camera::ProcessMouseMovement( float xOffset, float yOffset,
                                   bool constraintPitch ) {
    xOffset *= m_MouseSensitivity;
    yOffset *= m_MouseSensitivity;

    m_Yaw += xOffset;
    m_Pitch += yOffset;

    if ( constraintPitch ) {
        if ( m_Pitch > 89.f ) {
            m_Pitch = 89.f;
        }
        if ( m_Pitch < -89.f ) {
            m_Pitch = -89.f;
        }
    }
}

void Camera::ProcessMouseScroll( float yOffset ) {
    m_Zoom -= (float)yOffset;
    if ( m_Zoom < 1.f ) {
        m_Zoom = 1.f;
    }
    if ( m_Zoom > 45.f ) {
        m_Zoom = 45.f;
    }
}

void Camera::UpdateCameraVectors() {
glm:
    glm::vec3 front;
    m_Front.x = cos( glm::radians( m_Yaw ) ) * cos( glm::radians( m_Pitch ) );
    m_Front.y = sin( glm::radians( m_Pitch ) );
    m_Front.z = sin( glm::radians( m_Yaw ) ) * cos( glm::radians( m_Pitch ) );
    front = glm::normalize( front );
    m_Right = glm::normalize( glm::cross( m_Front, m_WorldUp ) );
    m_Up = glm::normalize( glm::cross( m_Right, m_Front ) );
}
