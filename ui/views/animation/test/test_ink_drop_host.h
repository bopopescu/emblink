// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef UI_VIEWS_ANIMATION_TEST_TEST_INK_DROP_HOST_H_
#define UI_VIEWS_ANIMATION_TEST_TEST_INK_DROP_HOST_H_

#include "base/macros.h"
#include "ui/views/animation/ink_drop_host.h"

namespace views {

// A non-functional implementation of an InkDropHost that can be used during
// tests.  Tracks the number of hosted ink drop layers.
class TestInkDropHost : public InkDropHost {
 public:
  TestInkDropHost();
  ~TestInkDropHost() override;

  int num_ink_drop_layers() const { return num_ink_drop_layers_; }

  void set_should_show_highlight(bool should_show_highlight) {
    should_show_highlight_ = should_show_highlight;
  }

  void set_disable_timers_for_test(bool disable_timers_for_test) {
    disable_timers_for_test_ = disable_timers_for_test;
  }

  void set_ripple_overrides_highlight(bool overrides_highlight) {
    ripple_overrides_highlight_ = overrides_highlight;
  }

  // TestInkDropHost:
  void AddInkDropLayer(ui::Layer* ink_drop_layer) override;
  void RemoveInkDropLayer(ui::Layer* ink_drop_layer) override;
  std::unique_ptr<InkDropRipple> CreateInkDropRipple() const override;
  std::unique_ptr<InkDropHighlight> CreateInkDropHighlight() const override;

 private:
  int num_ink_drop_layers_;

  bool should_show_highlight_;

  bool ripple_overrides_highlight_;

  // When true, the InkDropRipple/InkDropHighlight instances will have their
  // timers disabled after creation.
  bool disable_timers_for_test_;

  DISALLOW_COPY_AND_ASSIGN(TestInkDropHost);
};

}  // namespace views

#endif  // UI_VIEWS_ANIMATION_TEST_TEST_INK_DROP_HOST_H_
