/******************************************************************************\
* Copyright (C) 2012-2018 Leap Motion, Inc. All rights reserved.               *
* Leap Motion proprietary and confidential. Not for distribution.              *
* Use subject to the terms of the Leap Motion SDK Agreement available at       *
* https://developer.leapmotion.com/sdk_agreement, or another agreement         *
* between Leap Motion and you, your company or other organization.             *
\******************************************************************************/

#include "LeapImplementationC++.h"

// Linear min or max scan through the list members
#define MINIMIZE_FILTER(type, parameter) \
const std::vector<type>& items = as<ListBaseImplementation<type>>()->data(); \
float minx = std::numeric_limits<float>::max(); int minIndex = -1; \
for (int i=0; i<static_cast<int>(items.size()); i++) { \
  const float x = parameter; \
  if (x < minx) { minx = x; minIndex = i; } \
} \
return (minIndex == -1 ? type::invalid() : items[minIndex]);

namespace Leap {

struct FilterExtended {
  inline bool operator()(const Leap::Finger& finger) const { return !finger.isExtended(); }
};

struct FilterThumb  { inline bool operator()(const Leap::Finger& finger) const { return finger.type() != Leap::Finger::Type::TYPE_THUMB;  } };
struct FilterIndex  { inline bool operator()(const Leap::Finger& finger) const { return finger.type() != Leap::Finger::Type::TYPE_INDEX;  } };
struct FilterMiddle { inline bool operator()(const Leap::Finger& finger) const { return finger.type() != Leap::Finger::Type::TYPE_MIDDLE; } };
struct FilterRing   { inline bool operator()(const Leap::Finger& finger) const { return finger.type() != Leap::Finger::Type::TYPE_RING;   } };
struct FilterPinky  { inline bool operator()(const Leap::Finger& finger) const { return finger.type() != Leap::Finger::Type::TYPE_PINKY;  } };
struct FilterAll    { inline bool operator()(const Leap::Finger& finger) const { return true; } };

// Arm

Arm::Arm(HandImplementation* impl) : Interface(impl ? impl->shared_from_this() : std::make_shared<HandImplementation>()) {}
Arm::Arm() : Interface(std::make_shared<HandImplementation>()) {}
float Arm::width() const { return as<HandImplementation>()->armWidth(); }
Vector Arm::direction() const { return as<HandImplementation>()->armDirection(); }
Matrix Arm::basis() const { return as<HandImplementation>()->armBasis(); }
Vector Arm::elbowPosition() const { return as<HandImplementation>()->elbowPosition(); }
Vector Arm::wristPosition() const { return as<HandImplementation>()->wristPosition(); }
Vector Arm::center() const { return as<HandImplementation>()->armCenter(); }
const Arm& Arm::invalid() { static Arm* s_invalid = new Arm(); return *s_invalid; } // Expected to leak in order to live longer
bool Arm::isValid() const { return as<HandImplementation>()->isValid(); }
bool Arm::operator==(const Arm& rhs) const { return as<HandImplementation>() == rhs.as<HandImplementation>() && isValid(); }
bool Arm::operator!=(const Arm& rhs) const { return as<HandImplementation>() != rhs.as<HandImplementation>() || !isValid(); }
std::ostream& operator<<(std::ostream& out, const Arm& arm) { return out << arm.toString(); }

// Device

Device::Device(DeviceImplementation* impl) : Interface(impl ? impl->shared_from_this() : std::make_shared<DeviceImplementation>()) {}
Device::Device() : Interface(std::make_shared<DeviceImplementation>()) {}
float Device::horizontalViewAngle() const { return as<DeviceImplementation>()->horizontalViewAngle(); }
float Device::verticalViewAngle() const { return as<DeviceImplementation>()->verticalViewAngle(); }
float Device::range() const { return as<DeviceImplementation>()->range(); }
float Device::baseline() const { return as<DeviceImplementation>()->baseline(); }
float Device::distanceToBoundary(const Vector& position) const { return as<DeviceImplementation>()->distanceToBoundary(position); }
bool Device::isStreaming() const { return as<DeviceImplementation>()->isStreaming(); }
bool Device::isSmudged() const { return as<DeviceImplementation>()->isSmudged(); }
bool Device::isLightingBad() const { return as<DeviceImplementation>()->isLightingBad(); }
const Device& Device::invalid() { static Device* s_invalid = new Device(); return *s_invalid; } // Expected to leak in order to live longer
bool Device::isValid() const { return as<DeviceImplementation>()->isValid(); }
bool Device::operator==(const Device& rhs) const { return as<DeviceImplementation>() == rhs.as<DeviceImplementation>() && isValid(); }
bool Device::operator!=(const Device& rhs) const { return as<DeviceImplementation>() != rhs.as<DeviceImplementation>() || !isValid(); }
std::ostream& operator<<(std::ostream& out, const Device& device) { return out << device.toString(); }
const char* Device::toCString(size_t& length) const {
  const auto& str = as<DeviceImplementation>()->toString();
  length = str.size();
  return str.c_str();
}
const char* Device::serialNumberCString(size_t& length) const {
  const auto& str = as<DeviceImplementation>()->serialNumber();
  length = str.size();
  return str.c_str();
}
Device::Type Device::type() const {
  return as<DeviceImplementation>()->type();
}

const char* Device::typeString(Device::Type t) {
  switch (t) {
    default:
      return "TYPE_UNKNOWN";
    case Leap::Device::TYPE_PERIPHERAL:
      return "TYPE_PERIPHERAL";
    case Leap::Device::TYPE_NIGHTCRAWLER:
      return "TYPE_NIGHTCRAWLER";
    case Leap::Device::TYPE_RIGEL:
      return "TYPE_RIGEL";
  }
}

// FailedDevice

FailedDevice::FailedDevice(FailedDeviceImplementation* impl) : Interface(impl ? impl->shared_from_this() : std::make_shared<FailedDeviceImplementation>()) {}
FailedDevice::FailedDevice() : Interface(std::make_shared<FailedDeviceImplementation>()) {}
FailedDevice::FailureType FailedDevice::failure() const { return as<FailedDeviceImplementation>()->failure(); }
const FailedDevice& FailedDevice::invalid() { static FailedDevice* s_invalid = new FailedDevice(); return *s_invalid; } // Expected to leak in order to live longer
bool FailedDevice::isValid() const { return as<FailedDeviceImplementation>()->isValid(); }
bool FailedDevice::operator==(const FailedDevice& rhs) const { return as<FailedDeviceImplementation>() == rhs.as<FailedDeviceImplementation>() && isValid(); }
bool FailedDevice::operator!=(const FailedDevice& rhs) const { return as<FailedDeviceImplementation>() != rhs.as<FailedDeviceImplementation>() || !isValid(); }
const char* FailedDevice::pnpIdCString(size_t& length) const {
  const auto& str = as<FailedDeviceImplementation>()->pnpId();
  length = str.size();
  return str.c_str();
}

// Bone

Bone::Bone(BoneImplementation* impl) : Interface(impl ? impl->shared_from_this() : std::make_shared<BoneImplementation>()) {}
Bone::Bone() : Interface(std::make_shared<BoneImplementation>()) {}
Vector Bone::prevJoint() const { return as<BoneImplementation>()->prevJoint(); }
Vector Bone::nextJoint() const { return as<BoneImplementation>()->nextJoint(); }
Vector Bone::center() const { return as<BoneImplementation>()->center(); }
Vector Bone::direction() const { return as<BoneImplementation>()->direction(); }
float Bone::length() const { return as<BoneImplementation>()->length(); }
float Bone::width() const { return as<BoneImplementation>()->width(); }
Bone::Type Bone::type() const { return as<BoneImplementation>()->type(); }
Matrix Bone::basis() const { return as<BoneImplementation>()->basis(); }
const Bone& Bone::invalid() { static Bone* s_invalid = new Bone(); return *s_invalid; } // Expected to leak in order to live longer
bool Bone::isValid() const { return as<BoneImplementation>()->isValid(); }
bool Bone::operator==(const Bone& rhs) const { return as<BoneImplementation>() == rhs.as<BoneImplementation>() && isValid(); }
bool Bone::operator!=(const Bone& rhs) const { return as<BoneImplementation>() != rhs.as<BoneImplementation>() || !isValid(); }
std::ostream& operator<<(std::ostream& out, const Bone& bone) { return out << bone.toString(); }
const char* Bone::toCString(size_t& length) const {
  const auto& str = as<BoneImplementation>()->toString();
  length = str.size();
  return str.c_str();
}

// Finger

Finger::Finger(FingerImplementation* impl) : Interface(impl ? impl->shared_from_this() : std::make_shared<FingerImplementation>()) {}
Finger::Finger() : Interface(std::make_shared<FingerImplementation>()) {}
Frame Finger::frame() const { return as<FingerImplementation>()->frame(); }
Hand Finger::hand() const { return as<FingerImplementation>()->hand(); }
Finger::Type Finger::type() const { return as<FingerImplementation>()->type(); }
int32_t Finger::id() const { return as<FingerImplementation>()->id(); }
Bone Finger::bone(Bone::Type boneIx) const { return as<FingerImplementation>()->bone(boneIx); }
Vector Finger::tipPosition() const { return as<FingerImplementation>()->tipPosition(); }
Vector Finger::direction() const { return as<FingerImplementation>()->direction(); }
float Finger::width() const { return as<FingerImplementation>()->width(); }
float Finger::length() const { return as<FingerImplementation>()->length(); }
bool Finger::isExtended() const { return as<FingerImplementation>()->isExtended(); }
float Finger::timeVisible() const {  return as<FingerImplementation>()->timeVisible(); }
const Finger& Finger::invalid() { static Finger* s_invalid = new Finger(); return *s_invalid; } // Expected to leak in order to live longer
bool Finger::isValid() const { return as<FingerImplementation>()->isValid(); }
bool Finger::operator==(const Finger& rhs) const { return as<FingerImplementation>() == rhs.as<FingerImplementation>() && isValid(); }
bool Finger::operator!=(const Finger& rhs) const { return as<FingerImplementation>() != rhs.as<FingerImplementation>() || !isValid(); }
std::ostream& operator<<(std::ostream& out, const Finger& finger) { return out << finger.toString(); }
const char* Finger::toCString(size_t& length) const {
  const auto& str = as<FingerImplementation>()->toString();
  length = str.size();
  return str.c_str();
}

// Hand

Hand::Hand(HandImplementation* impl) : Interface(impl ? impl->shared_from_this() : std::make_shared<HandImplementation>()) {}
Hand::Hand() : Interface(std::make_shared<HandImplementation>()) {}
int32_t Hand::id() const { return as<HandImplementation>()->id(); }
Frame Hand::frame() const { return as<HandImplementation>()->frame(); }
FingerList Hand::fingers() const { return as<HandImplementation>()->fingers(); }
Finger Hand::finger(int32_t id) const { return as<HandImplementation>()->finger(id); }
Vector Hand::palmPosition() const { return as<HandImplementation>()->palmPosition(); }
Vector Hand::stabilizedPalmPosition() const { return as<HandImplementation>()->stabilizedPalmPosition(); }
Vector Hand::palmVelocity() const { return as<HandImplementation>()->palmVelocity(); }
Vector Hand::palmNormal() const { return as<HandImplementation>()->palmNormal(); }
float Hand::palmWidth() const { return as<HandImplementation>()->palmWidth(); }
Vector Hand::direction() const { return as<HandImplementation>()->direction(); }
Matrix Hand::basis() const { return as<HandImplementation>()->basis(); }
Arm Hand::arm() const { return as<HandImplementation>()->arm(); }
Vector Hand::wristPosition() const { return as<HandImplementation>()->wristPosition(); }
float Hand::pinchDistance() const { return as<HandImplementation>()->pinchDistance(); }
float Hand::grabAngle() const { return as<HandImplementation>()->grabAngle(); }
float Hand::pinchStrength() const { return as<HandImplementation>()->pinchStrength(); }
float Hand::grabStrength() const { return as<HandImplementation>()->grabStrength(); }
float Hand::timeVisible() const { return as<HandImplementation>()->timeVisible(); }
float Hand::confidence() const { return as<HandImplementation>()->confidence(); }
bool Hand::isLeft() const { return as<HandImplementation>()->isLeft(); }
bool Hand::isRight() const { return as<HandImplementation>()->isRight(); }
bool Hand::isValid() const { return as<HandImplementation>()->isValid(); }
const Hand& Hand::invalid() { static Hand* s_invalid = new Hand(); return *s_invalid; } // Expected to leak in order to live longer
bool Hand::operator==(const Hand& rhs) const { return as<HandImplementation>() == rhs.as<HandImplementation>() && isValid(); }
bool Hand::operator!=(const Hand& rhs) const { return as<HandImplementation>() != rhs.as<HandImplementation>() || !isValid(); }
std::ostream& operator<<(std::ostream& out, const Hand& hand) { return out << hand.toString(); }
const char* Hand::toCString(size_t& length) const {
  const auto& str = as<HandImplementation>()->toString();
  length = str.size();
  return str.c_str();
}

// Frame

Frame::Frame(FrameImplementation* impl) : Interface(impl ? impl->shared_from_this() : std::make_shared<FrameImplementation>()) {}
Frame::Frame() : Interface(std::make_shared<FrameImplementation>()) {}
int64_t Frame::id() const { return as<FrameImplementation>()->id(); }
int64_t Frame::timestamp() const { return as<FrameImplementation>()->timestamp(); }
HandList Frame::hands() const { return as<FrameImplementation>()->hands(); }
Hand Frame::hand(int32_t id) const { return as<FrameImplementation>()->hand(id); }
FingerList Frame::fingers() const { return as<FrameImplementation>()->fingers(); }
Finger Frame::finger(int32_t id) const { return as<FrameImplementation>()->finger(id); }
ImageList Frame::images() const { return as<FrameImplementation>()->images(); }
ImageList Frame::rawImages() const { return as<FrameImplementation>()->rawImages(); }
MapPointList Frame::mapPoints() const { return as<FrameImplementation>()->mapPoints(); }
float Frame::currentFramesPerSecond() const { return as<FrameImplementation>()->currentFramesPerSecond(); }
bool Frame::isValid() const { return as<FrameImplementation>()->isValid(); }
const Frame& Frame::invalid() { static Frame* s_invalid = new Frame(); return *s_invalid; } // Expected to leak in order to live longer
bool Frame::operator==(const Frame& rhs) const { return as<FrameImplementation>() == rhs.as<FrameImplementation>() && isValid(); }
bool Frame::operator!=(const Frame& rhs) const { return as<FrameImplementation>() != rhs.as<FrameImplementation>() || !isValid(); }
std::ostream& operator<<(std::ostream& out, const Frame& frame) { return out << frame.toString(); }
const char* Frame::toCString(size_t& length) const {
  const auto& str = as<FrameImplementation>()->toString();
  length = str.size();
  return str.c_str();
}

// HeadPose

HeadPose::HeadPose(HeadPoseImplementation* impl) : Interface(impl ? impl->shared_from_this() : std::make_shared<HeadPoseImplementation>()) {}
HeadPose::HeadPose() : Interface(std::make_shared<HeadPoseImplementation>()) {}
int64_t HeadPose::timestamp() const { return as<HeadPoseImplementation>()->timestamp(); }
Vector HeadPose::position() const { return as<HeadPoseImplementation>()->position(); }
Quaternion HeadPose::orientation() const { return as<HeadPoseImplementation>()->orientation(); }

// Image

Image::Image(ImageImplementation* impl) : Interface(impl ? impl->shared_from_this() : std::make_shared<ImageImplementation>()) {}
Image::Image() : Interface(std::make_shared<ImageImplementation>()) {}
int64_t Image::sequenceId() const { return as<ImageImplementation>()->sequenceId(); }
int32_t Image::id() const { return as<ImageImplementation>()->id(); }
const unsigned char* Image::data() const { return as<ImageImplementation>()->data(); }
const float* Image::distortion() const { return as<ImageImplementation>()->distortion(); }
int Image::width() const { return as<ImageImplementation>()->width(); }
int Image::height() const { return as<ImageImplementation>()->height(); }
int Image::bytesPerPixel() const { return as<ImageImplementation>()->bytesPerPixel(); }
Image::FormatType Image::format() const { return as<ImageImplementation>()->format(); }
int Image::distortionWidth() const { return as<ImageImplementation>()->distortionWidth(); }
int Image::distortionHeight() const { return as<ImageImplementation>()->distortionHeight(); }
float Image::rayOffsetX() const { return as<ImageImplementation>()->rayOffsetX(); }
float Image::rayOffsetY() const { return as<ImageImplementation>()->rayOffsetY(); }
float Image::rayScaleX() const { return as<ImageImplementation>()->rayScaleX(); }
float Image::rayScaleY() const { return as<ImageImplementation>()->rayScaleY(); }
Vector Image::rectify(const Vector& uv) const { return as<ImageImplementation>()->rectify(uv); }
Vector Image::warp(const Vector& xy) const { return as<ImageImplementation>()->warp(xy); }
int64_t Image::timestamp() const { return as<ImageImplementation>()->timestamp(); }
bool Image::isValid() const { return as<ImageImplementation>()->isValid(); }
const Image& Image::invalid() { static Image* s_invalid = new Image(); return *s_invalid; } // Expected to leak in order to live longer
bool Image::operator==(const Image& rhs) const { return as<ImageImplementation>() == rhs.as<ImageImplementation>() && isValid(); }
bool Image::operator!=(const Image& rhs) const { return as<ImageImplementation>() != rhs.as<ImageImplementation>() || !isValid(); }
std::ostream& operator<<(std::ostream& out, const Image& image) { return out << image.toString(); }
const char* Image::toCString(size_t& length) const {
  const auto& str = as<ImageImplementation>()->toString();
  length = str.size();
  return str.c_str();
}

Config::Config(ControllerImplementation* impl) : Interface(impl ? impl->shared_from_this() : std::shared_ptr<Leap::Interface::Implementation>(nullptr)) {}
Config::Value Config::value(const char* key, uint32_t timeoutMilliseconds) const { return as<ControllerImplementation>()->getConfigValue(key, std::chrono::milliseconds(timeoutMilliseconds)); }
bool Config::setValue(const char* key, const Config::Value& v) { return as<ControllerImplementation>()->setConfigValue(key, v); }
// Controller

Controller::Controller(ControllerImplementation* impl) : Interface(impl ? impl->shared_from_this() : std::make_shared<ControllerImplementation>(*this)) {}
Controller::Controller(const char* server_namespace) : Interface(std::make_shared<ControllerImplementation>(*this, server_namespace)) {}
Controller::~Controller() {}
Controller::Controller(Listener& listener, const char* server_namespace) : Interface(std::make_shared<ControllerImplementation>(*this, server_namespace)) { addListener(listener); }
bool Controller::isConnected() const { return as<ControllerImplementation>()->isConnected(); }
bool Controller::isServiceConnected() const { return as<ControllerImplementation>()->isServiceConnected(); }
Controller::PolicyFlag Controller::policyFlags() const { return as<ControllerImplementation>()->policyFlags(); }
void Controller::setPolicyFlags(PolicyFlag flags) const { as<ControllerImplementation>()->setPolicyFlags(flags); }
void Controller::setPolicy(PolicyFlag policy) const { as<ControllerImplementation>()->setPolicy(policy); }
void Controller::clearPolicy(PolicyFlag policy) const { as<ControllerImplementation>()->clearPolicy(policy); }
bool Controller::isPolicySet(PolicyFlag policy) const { return as<ControllerImplementation>()->isPolicySet(policy); }
bool Controller::addListener(Listener& listener) { return as<ControllerImplementation>()->addListener(listener); }
bool Controller::removeListener(Listener& listener) { return as<ControllerImplementation>()->removeListener(listener); }
Frame Controller::frame(int history) const { return as<ControllerImplementation>()->frame(history); }
HeadPose Controller::headPose(int64_t timestamp) const { return as<ControllerImplementation>()->headPose(timestamp); }
ImageList Controller::images() const { return as<ControllerImplementation>()->images(); }
ImageList Controller::rawImages() const { return as<ControllerImplementation>()->rawImages(); }
Config Controller::config() const { return as<ControllerImplementation>()->config(); }
DeviceList Controller::devices() const { return as<ControllerImplementation>()->devices(); }
FailedDeviceList Controller::failedDevices() const { return as<ControllerImplementation>()->failedDevices(); }
void Controller::setPaused(bool pause) { as<ControllerImplementation>()->setPaused(pause); }
bool Controller::isPaused() const { return as<ControllerImplementation>()->isPaused(); }
int64_t Controller::now() const { return LeapGetNow(); }

// FingerList

FingerList::FingerList(const std::shared_ptr<ListBaseImplementation<Finger>>& rhs) : Interface(rhs) {}
FingerList::FingerList() : Interface(std::static_pointer_cast<Implementation>(std::make_shared<ListBaseImplementation<Finger>>())) {}
int FingerList::count() const { return as<ListBaseImplementation<Finger>>()->count(); }
bool FingerList::isEmpty() const { return as<ListBaseImplementation<Finger>>()->empty(); }
Finger FingerList::operator[](int index) const { return as<ListBaseImplementation<Finger>>()->at(index); }
FingerList& FingerList::append(const FingerList& rhs) { as<ListBaseImplementation<Finger>>()->append(*(rhs.as<ListBaseImplementation<Finger>>())); return *this; }
Finger FingerList::leftmost() const { MINIMIZE_FILTER(Finger, items[i].tipPosition().x); }
Finger FingerList::rightmost() const { MINIMIZE_FILTER(Finger, -items[i].tipPosition().x); }
Finger FingerList::frontmost() const { MINIMIZE_FILTER(Finger, items[i].tipPosition().z); }
FingerList FingerList::extended() const { return std::make_shared<ListBaseImplementation<Finger>>(as<ListBaseImplementation<Finger>>()->filter<FilterExtended>()); }
FingerList FingerList::fingerType(Finger::Type type) const {
  switch(type) {
    case Finger::Type::TYPE_THUMB:
      return std::make_shared<ListBaseImplementation<Finger>>(as<ListBaseImplementation<Finger>>()->filter<FilterThumb>());
    case Finger::Type::TYPE_INDEX:
      return std::make_shared<ListBaseImplementation<Finger>>(as<ListBaseImplementation<Finger>>()->filter<FilterIndex>());
    case Finger::Type::TYPE_MIDDLE:
      return std::make_shared<ListBaseImplementation<Finger>>(as<ListBaseImplementation<Finger>>()->filter<FilterMiddle>());
    case Finger::Type::TYPE_RING:
      return std::make_shared<ListBaseImplementation<Finger>>(as<ListBaseImplementation<Finger>>()->filter<FilterRing>());
    case Finger::Type::TYPE_PINKY:
      return std::make_shared<ListBaseImplementation<Finger>>(as<ListBaseImplementation<Finger>>()->filter<FilterPinky>());
    default:
      return std::make_shared<ListBaseImplementation<Finger>>(as<ListBaseImplementation<Finger>>()->filter<FilterAll>());
  }
}
FingerList::const_iterator FingerList::begin() const { return const_iterator(*this, 0); }
FingerList::const_iterator FingerList::end() const { return const_iterator(*this, count()); }

// HandList

HandList::HandList(const std::shared_ptr<ListBaseImplementation<Hand>>& rhs) : Interface(rhs) {}
HandList::HandList() : Interface(std::static_pointer_cast<Implementation>(std::make_shared<ListBaseImplementation<Hand>>())) {}
int HandList::count() const { return as<ListBaseImplementation<Hand>>()->count(); }
bool HandList::isEmpty() const { return as<ListBaseImplementation<Hand>>()->empty(); }
Hand HandList::operator[](int index) const { return as<ListBaseImplementation<Hand>>()->at(index); }
HandList& HandList::append(const HandList& rhs) {
  as<ListBaseImplementation<Hand>>()->append(*(rhs.as<ListBaseImplementation<Hand>>())); return *this; }
Hand HandList::leftmost() const { MINIMIZE_FILTER(Hand, items[i].palmPosition().x); }
Hand HandList::rightmost() const { MINIMIZE_FILTER(Hand, -items[i].palmPosition().x); }
Hand HandList::frontmost() const { MINIMIZE_FILTER(Hand, items[i].palmPosition().z); }
HandList::const_iterator HandList::begin() const { return const_iterator(*this, 0); }
HandList::const_iterator HandList::end() const { return const_iterator(*this, count()); }

// ImageList

ImageList::ImageList(const std::shared_ptr<ListBaseImplementation<Image>>& rhs) : Interface(rhs) {}
ImageList::ImageList() : Interface(std::static_pointer_cast<Implementation>(std::make_shared<ListBaseImplementation<Image>>())) {}
int ImageList::count() const { return as<ListBaseImplementation<Image>>()->count(); }
bool ImageList::isEmpty() const { return as<ListBaseImplementation<Image>>()->empty(); }
Image ImageList::operator[](int index) const { return as<ListBaseImplementation<Image>>()->at(index); }
ImageList& ImageList::append(const ImageList& rhs) { as<ListBaseImplementation<Image>>()->append(*(rhs.as<ListBaseImplementation<Image>>())); return *this; }
ImageList::const_iterator ImageList::begin() const { return const_iterator(*this, 0); }
ImageList::const_iterator ImageList::end() const { return const_iterator(*this, count()); }

// DeviceList

DeviceList::DeviceList(const std::shared_ptr<ListBaseImplementation<Device>>& rhs) : Interface(rhs) {}
DeviceList::DeviceList() : Interface(std::static_pointer_cast<Implementation>(std::make_shared<ListBaseImplementation<Device>>())) {}
int DeviceList::count() const { return as<ListBaseImplementation<Device>>()->count(); }
bool DeviceList::isEmpty() const { return as<ListBaseImplementation<Device>>()->empty(); }
Device DeviceList::operator[](int index) const { return as<ListBaseImplementation<Device>>()->at(index); }
DeviceList& DeviceList::append(const DeviceList& rhs) { as<ListBaseImplementation<Device>>()->append(*(rhs.as<ListBaseImplementation<Device>>())); return *this; }
DeviceList::const_iterator DeviceList::begin() const { return const_iterator(*this, 0); }
DeviceList::const_iterator DeviceList::end() const { return const_iterator(*this, count()); }

// FailedDeviceList

FailedDeviceList::FailedDeviceList(const std::shared_ptr<ListBaseImplementation<FailedDevice>>& rhs) : Interface(rhs) {}
FailedDeviceList::FailedDeviceList() : Interface(std::static_pointer_cast<Implementation>(std::make_shared<ListBaseImplementation<FailedDevice>>())) {}
int FailedDeviceList::count() const { return as<ListBaseImplementation<FailedDevice>>()->count(); }
bool FailedDeviceList::isEmpty() const { return as<ListBaseImplementation<FailedDevice>>()->empty(); }
FailedDevice FailedDeviceList::operator[](int index) const { return as<ListBaseImplementation<FailedDevice>>()->at(index); }
FailedDeviceList& FailedDeviceList::append(const FailedDeviceList& rhs) { as<ListBaseImplementation<FailedDevice>>()->append(*(rhs.as<ListBaseImplementation<FailedDevice>>())); return *this; }
FailedDeviceList::const_iterator FailedDeviceList::begin() const { return const_iterator(*this, 0); }
FailedDeviceList::const_iterator FailedDeviceList::end() const { return const_iterator(*this, count()); }

// MapPointList

MapPointList::MapPointList(const std::shared_ptr<ListBaseImplementation<MapPoint>>& rhs) : Interface(rhs) {}
MapPointList::MapPointList() : Interface(std::static_pointer_cast<Implementation>(std::make_shared<ListBaseImplementation<MapPoint>>())) {}
int MapPointList::count() const { return as<ListBaseImplementation<MapPoint>>()->count(); }
bool MapPointList::isEmpty() const { return as<ListBaseImplementation<MapPoint>>()->empty(); }
MapPoint MapPointList::operator[](int index) const { return as<ListBaseImplementation<MapPoint>>()->at(index); }
MapPointList& MapPointList::append(const MapPointList& rhs) { as<ListBaseImplementation<MapPoint>>()->append(*(rhs.as<ListBaseImplementation<MapPoint>>())); return *this; }
MapPointList::const_iterator MapPointList::begin() const { return const_iterator(*this, 0); }
MapPointList::const_iterator MapPointList::end() const { return const_iterator(*this, count()); }

}
