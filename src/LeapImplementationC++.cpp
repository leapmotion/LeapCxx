/******************************************************************************\
* Copyright (C) 2012-2018 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include "LeapImplementationC++.h"

namespace Leap {

// ImageImplementation

ImageImplementation::ImageImplementation(const std::shared_ptr<ControllerImplementation>& controllerImpl, const LEAP_IMAGE_EVENT& image_event, int32_t imageId) :
  m_weakControllerImpl(controllerImpl), m_image_event(image_event), m_imageId(imageId), m_name([&image_event, imageId]{
    std::ostringstream oss;
    oss << "Image Frame Id:" << image_event.info.frame_id << ", Id: " << imageId;
    return oss.str();
  }())
{
  if (controllerImpl) {
    m_ref = controllerImpl->getSharedBufferReference(m_image_event.image[m_imageId].data);
  }
}

Vector ImageImplementation::rectify(const Vector& uv) const {
  auto controllerImpl = m_weakControllerImpl.lock();
  if (!controllerImpl)
    return {0.0f, 0.0f, 1.0f};

  return controllerImpl->rectify(m_imageId == 0 ? eLeapPerspectiveType_stereo_left : eLeapPerspectiveType_stereo_right, uv);
}

Vector ImageImplementation::warp(const Vector& xy) const {
  auto controllerImpl = m_weakControllerImpl.lock();
  if (!controllerImpl)
    return {0.0f, 0.0f, 1.0f};

  return controllerImpl->warp(m_imageId == 0 ? eLeapPerspectiveType_stereo_left : eLeapPerspectiveType_stereo_right, xy);
}

// BoneImplementation

LEAP_BONE BoneImplementation::s_invalid;

// FingerImplementation

FingerImplementation::FingerImplementation(const std::shared_ptr<HandImplementation>& handImpl, const LEAP_DIGIT& digit) :
  m_weakHandImpl(handImpl), m_digit(digit), m_id((handImpl ? handImpl->id()*10 : 0) + m_digit.finger_id), m_name([this]{
    std::ostringstream oss;
    oss << "Finger Id:" << m_id;
    return oss.str();
  }()), m_isLeftHand(handImpl ? handImpl->isLeft() : false) {}
Frame FingerImplementation::frame() const { auto handImpl = m_weakHandImpl.lock(); return handImpl ? handImpl->frame() : Frame::invalid(); }
Hand FingerImplementation::hand() const { auto handImpl = m_weakHandImpl.lock(); return handImpl ? Hand(handImpl.get()) : Hand::invalid(); }
float FingerImplementation::timeVisible() const { auto handImpl = m_weakHandImpl.lock(); return handImpl ? handImpl->timeVisible() : 0.0f; }
LEAP_DIGIT FingerImplementation::s_invalid{0};

// HandImplementation

LEAP_HAND HandImplementation::s_invalid{static_cast<uint32_t>(-1)};

// DeviceImplementation

float DeviceImplementation::distanceToBoundary(const Vector& position) const {
  const Vector pos = position;
  const Vector center(0, 0, 0);

  if (!isValid() || (pos - center).magnitudeSquared() > range()*range()) {
    return 0.0f;
  }

  const auto closestPointOnBoundary = [this, &position] {
    const float tanH = std::tan(horizontalViewAngle()*0.5f);
    const float tanV = std::tan(verticalViewAngle()*0.5f);

    // Check horizontal walls
    float m = 1.0f / tanH;
    const float x = (m * position.y + fabs(position.x)) / (m*m + 1.0f);
    float y = m * x;
    Vector horiz(x, y, position.z);
    if (position.x < 0) { horiz.x *= -1; }

    // Check vertical walls
    m = 1.0f / tanV;
    const float z = (m * position.y + std::fabs(position.z)) / (m*m + 1.0f);
    y = m * z;
    Vector vert(position.x, y, z);
    if (position.z < 0) { vert.z *= -1; }

    // Check squared distances
    const Vector dirNorm = position.normalized();
    const double distSqTop = (dirNorm*range() - position).magnitudeSquared();
    const double distSqHoriz = (horiz - position).magnitudeSquared();
    const double distSqVert = (vert - position).magnitudeSquared();

    // Return the closest one
    if (distSqTop < distSqHoriz && distSqTop < distSqVert) {
      return dirNorm * range();
    } else if (distSqHoriz < distSqVert) {
      return horiz;
    } else {
      return vert;
    }
  };

  const auto point = closestPointOnBoundary();
  return (pos - point).magnitude();
}

}
