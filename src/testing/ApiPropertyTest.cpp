#include "LeapC++.h"
#include <gtest/gtest.h>

TEST(ApiPropertyTest, ValidateVectorProperties) {
  const float* f = nullptr;

  f = Leap::Vector::right().toFloatPointer();
  EXPECT_EQ(1, f[0]);
  EXPECT_EQ(0, f[1]);
  EXPECT_EQ(0, f[2]);

  f = Leap::Vector::up().toFloatPointer();
  EXPECT_EQ(0, f[0]);
  EXPECT_EQ(1, f[1]);
  EXPECT_EQ(0, f[2]);

  f = Leap::Vector::backward().toFloatPointer();
  EXPECT_EQ(0, f[0]);
  EXPECT_EQ(0, f[1]);
  EXPECT_EQ(1, f[2]);

  EXPECT_EQ(Leap::Vector(1, 0, 0), Leap::Vector::xAxis());
  EXPECT_EQ(Leap::Vector(0, 1, 0), Leap::Vector::yAxis());
  EXPECT_EQ(Leap::Vector(0, 0, 1), Leap::Vector::zAxis());
  EXPECT_EQ(Leap::Vector(0, 0, 0), Leap::Vector::zero());
}

TEST(ApiPropertyTest, ValidateMatrixProperties) {
  Leap::Matrix identity = Leap::Matrix::identity();
  EXPECT_EQ(Leap::Matrix::identity(), identity);
}

TEST(ApiPropertyTest, InvalidFrameTest) {
  Leap::Controller controller;
  Leap::Frame outOfRange = controller.frame(1000000);
  Leap::Frame invalidFrame = Leap::Frame::invalid();

  EXPECT_NE(invalidFrame, outOfRange);
  EXPECT_FALSE(outOfRange.isValid());
  EXPECT_EQ(-1, outOfRange.id());
  EXPECT_EQ(0, outOfRange.hands().count());
  EXPECT_EQ(0, outOfRange.fingers().count());
  EXPECT_FALSE(outOfRange.finger(1).isValid());
  EXPECT_FALSE(outOfRange.hand(1).isValid());
  EXPECT_EQ(0, invalidFrame.images().count()) << "Invalid frames appear to have images attached";
}

TEST(ApiPropertyTest, InvalidFingerTest) {
  Leap::Frame invalidFrame = Leap::Frame::invalid();

  Leap::Finger badFinger1 = invalidFrame.finger(1);
  Leap::Finger badFinger2 = Leap::Finger::invalid();
  EXPECT_NE(badFinger1, badFinger2);
  EXPECT_FALSE(badFinger2.isValid());
  EXPECT_EQ(-1, badFinger2.id());
  EXPECT_EQ(Leap::Finger::TYPE_THUMB, badFinger1.type());
}

TEST(ApiPropertyTest, InvalidBoneTest) {
  Leap::Frame invalidFrame = Leap::Frame::invalid();

  Leap::Bone badBone1 = invalidFrame.finger(1).bone(Leap::Bone::TYPE_METACARPAL);
  Leap::Bone badBone2 = Leap::Finger::invalid().bone(Leap::Bone::TYPE_METACARPAL);
  EXPECT_NE(badBone1, badBone2);
  EXPECT_FALSE(badBone2.isValid());
  EXPECT_EQ(Leap::Matrix::identity(), badBone2.basis());
}

TEST(ApiPropertyTest, InvalidHandTest) {
  Leap::Frame invalidFrame = Leap::Frame::invalid();

  Leap::Hand badHand1 = invalidFrame.hand(1);
  Leap::Hand badHand2 = Leap::Hand::invalid();
  EXPECT_NE(badHand1, badHand2);
  EXPECT_FALSE(badHand2.isValid());
  EXPECT_EQ(-1, badHand2.id());
  EXPECT_EQ(Leap::Matrix::identity(), badHand2.basis());
  EXPECT_EQ(Leap::Matrix::identity(), badHand1.basis());
  EXPECT_EQ(0, badHand2.fingers().count());
  EXPECT_EQ(Leap::Vector::zero(), badHand2.direction());
  EXPECT_EQ(Leap::Vector::zero(), badHand2.palmNormal());
  EXPECT_FALSE(badHand2.frame().isValid());
}

TEST(ApiPropertyTest, InvalidArmTest) {
  Leap::Frame invalidFrame = Leap::Frame::invalid();

  Leap::Arm badArm1 = invalidFrame.hand(1).arm();
  Leap::Arm badArm2 = Leap::Arm::invalid();
  EXPECT_NE(badArm1, badArm2);
  EXPECT_FALSE(badArm2.isValid());
  EXPECT_EQ(Leap::Matrix::identity(), badArm2.basis());
  EXPECT_EQ(Leap::Matrix::identity(), badArm1.basis());
  EXPECT_EQ(Leap::Vector::zero(), badArm2.center());
  EXPECT_TRUE(badArm2.direction() == -Leap::Vector(0,0,1));
  EXPECT_EQ(Leap::Vector::zero(), badArm2.elbowPosition());
  EXPECT_EQ(Leap::Vector::zero(), badArm2.wristPosition());
}

TEST(ApiPropertyTest, InvalidImageTest) {
  Leap::Image invalidImage = Leap::Image::invalid();

  EXPECT_FALSE(invalidImage.isValid());
  EXPECT_EQ(1, invalidImage.bytesPerPixel());
  EXPECT_EQ(nullptr, invalidImage.data());
  EXPECT_ANY_THROW(invalidImage.distortion());
  EXPECT_EQ(Leap::Image::INFRARED, invalidImage.format());
  EXPECT_EQ(0, invalidImage.height());
  EXPECT_EQ(-1, invalidImage.id());
  EXPECT_EQ(-1, invalidImage.sequenceId());
  EXPECT_EQ(0, invalidImage.timestamp());
  EXPECT_EQ(0, invalidImage.width());
}

TEST(ApiPropertyTest, TimestampTest) {
  Leap::Controller controller;
  const auto now = controller.now();
  EXPECT_TRUE(now > 0);
}
