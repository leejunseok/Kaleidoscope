/* -*- mode: c++ -*-
 * Copyright (C) 2020  Eric Paniagua (epaniagua@google.com)
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "testing/setup-googletest.h"

SETUP_GOOGLETEST();

namespace kaleidoscope {
namespace testing {
namespace {

using ::testing::IsEmpty;

class KeyboardReports : public VirtualDeviceTest {};


#define KEY_1 0,1
#define KEY_2 0,2
#define KEY_3 0,3

// This test suite tests to make sure that when we send multiple key events 
// within one cycle, we only generate one HID report
TEST_F(KeyboardReports, MultipleKeysPerCycle) {
  // Press one key inside a cycle and we should only get one key report
  sim_.Press(KEY_1);
  auto state = RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    Contains(Key_1));

  sim_.Release(KEY_1);
  state = RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    IsEmpty());

  state = RunCycle();
  // 2 cycles after releasing the key
  EXPECT_EQ(state->HIDReports()->ConsumerControl().size(), 0);




   // Press two keys inside a cycle and we should only get one key report

  sim_.Press(KEY_1);
  sim_.Press(KEY_2);
  state = RunCycle();


  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1) << "Two key presses in a single cycle should only generate a single HID report";
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    Contains(Key_1));
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    Contains(Key_2));

  sim_.Release(KEY_1);
  sim_.Release(KEY_2);
  state = RunCycle();

  ASSERT_EQ(state->HIDReports()->Keyboard().size(), 1);
  EXPECT_THAT(
    state->HIDReports()->Keyboard(0).ActiveKeycodes(),
    IsEmpty());

  state = RunCycle();
  // 2 cycles after releasing the key
  EXPECT_EQ(state->HIDReports()->ConsumerControl().size(), 0);



}

}  // namespace
}  // namespace testing
}  // namespace kaleidoscope
