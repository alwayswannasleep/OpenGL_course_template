#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up) :
        position(position),
        up(up),
        direction(direction),
        movementSpeed(1.5f),
        rotationSensitivity(0.15f),
        yawValue(-90.0f),
        pitchValue(0.0f) {
    updateValues();
}

void Camera::moveCamera(const Direction &direction, const GLfloat &delta) {
    GLfloat calculatedOffset = movementSpeed * delta;

    switch (direction) {
        case FORWARD:
            position += calculatedOffset * this->direction;
            break;
        case BACKWARD:
            position -= calculatedOffset * this->direction;
            break;
        case LEFT:
            position -= calculatedOffset * this->right;
            break;
        case RIGHT:
            position += calculatedOffset * this->right;
            break;
        case UP:
            position += calculatedOffset * this->up;
            break;
        case DOWN:
            position -= calculatedOffset * this->up;
            break;
    }
}

void Camera::rotateCamera(const GLfloat &xOffset, const GLfloat &yOffset) {
    GLfloat yaw = rotationSensitivity * xOffset;
    GLfloat pitch = rotationSensitivity * yOffset;

    yawValue += yaw;
    pitchValue += pitch;

    if (pitchValue > 89.0f) {
        pitchValue = 89.0f;
    } else if (pitchValue < -89.0f) {
        pitchValue = -89.0f;
    }

    yawValue = glm::mod(yawValue, 360.0f);

    updateValues();
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + direction, cameraUp);
}

void Camera::updateValues() {
    GLfloat directionX = glm::cos(glm::radians(yawValue)) * glm::cos(glm::radians(pitchValue));
    GLfloat directionY = glm::sin(glm::radians(pitchValue));
    GLfloat directionZ = glm::sin(glm::radians(yawValue)) * glm::cos(glm::radians(pitchValue));

    direction = glm::normalize(glm::vec3(directionX, directionY, directionZ));

    right = glm::normalize(glm::cross(direction, up));
    cameraUp = glm::normalize(glm::cross(right, direction));
}
