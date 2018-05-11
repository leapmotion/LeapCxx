#include "LeapC++.h"
#include "LeapImplementationC++.h"
#include "LeapC.h"
#include <gtest/gtest.h>

static_assert(std::is_destructible<Leap::FingerList::const_iterator>::value, "FingerList iterator must be destructable");

TEST(ConstIteratorTest, Algorithms) {
  std::vector<int> ids{0, 1, 2, 3, 4};
  std::vector<Leap::Finger> fingersVec;
  fingersVec.reserve(5);
  for (const auto id : ids) {
    LEAP_DIGIT digit{id};
    fingersVec.emplace_back(std::make_shared<Leap::FingerImplementation>(nullptr, digit).get());
  }
  Leap::FingerList fingers(std::make_shared<Leap::ListBaseImplementation<Leap::Finger>>(fingersVec));

  int index = 0;

  auto notfound_it = std::find_if(
    fingers.begin(),
    fingers.end(),
    [&index](const Leap::Finger& p) { int32_t id = index++; return !p.isValid() || id != p.id(); }
  );
  EXPECT_EQ(notfound_it, fingers.end()) << "Found a value that should not have been present in a finger collection";

  index = 0;
  for (const auto& finger : fingers) {
    EXPECT_EQ(ids[index], finger.id());
    index++;
  }
  EXPECT_EQ(static_cast<size_t>(index), ids.size());
}

TEST(ConstIteratorTest, DefaultNotEqual) {
  Leap::FingerList::const_iterator defaultctor_it;
  Leap::FingerList::const_iterator copyctor_it(defaultctor_it);
  EXPECT_EQ(defaultctor_it, copyctor_it);
}
