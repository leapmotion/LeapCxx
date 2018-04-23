#include "LeapC++/LeapC++.h"
#include "PipelineInterface/ScorchReason.h"

#define ASSERT_ENUM_EQ(a, b) static_assert((int)a == (int)b, "Mismatched Enums: " #a " != " #b)

ASSERT_ENUM_EQ(Leap::FailedDevice::FAIL_UNKNOWN, ScorchReason::UNKNOWN);
ASSERT_ENUM_EQ(Leap::FailedDevice::FAIL_CALIBRATION, ScorchReason::CALIBRATION);
ASSERT_ENUM_EQ(Leap::FailedDevice::FAIL_FIRMWARE, ScorchReason::FIRMWARE);
ASSERT_ENUM_EQ(Leap::FailedDevice::FAIL_TRANSPORT, ScorchReason::TRANSPORT);
ASSERT_ENUM_EQ(Leap::FailedDevice::FAIL_CONTROL, ScorchReason::CONTROL);
ASSERT_ENUM_EQ(Leap::FailedDevice::FAIL_COUNT, ScorchReason::COUNT);
