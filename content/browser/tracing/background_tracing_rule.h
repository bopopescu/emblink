// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CONTENT_BROWSER_TRACING_BACKGROUND_TRACING_RULE_H_
#define CONTENT_BROWSER_TRACING_BACKGROUND_TRACING_RULE_H_

#include "base/macros.h"
#include "content/browser/tracing/background_tracing_config_impl.h"
#include "content/common/content_export.h"

namespace base {
class DictionaryValue;
}  // namespace base

namespace content {

class CONTENT_EXPORT BackgroundTracingRule {
 public:
  BackgroundTracingRule();
  explicit BackgroundTracingRule(int trigger_delay);

  virtual ~BackgroundTracingRule();

  void Setup(const base::DictionaryValue* dict);
  BackgroundTracingConfigImpl::CategoryPreset category_preset() const {
    return category_preset_;
  }
  void set_category_preset(
      BackgroundTracingConfigImpl::CategoryPreset category_preset) {
    category_preset_ = category_preset;
  }

  virtual void Install() {}
  virtual void IntoDict(base::DictionaryValue* dict) const;
  virtual bool ShouldTriggerNamedEvent(const std::string& named_event) const;
  virtual void OnHistogramTrigger(const std::string& histogram_name) const {}

  // Seconds from the rule is triggered to finalization should start.
  virtual int GetTraceDelay() const;

  // Probability that we should allow a tigger to  happen.
  double trigger_chance() const { return trigger_chance_; }

  bool stop_tracing_on_repeated_reactive() const {
    return stop_tracing_on_repeated_reactive_;
  }

  static std::unique_ptr<BackgroundTracingRule> CreateRuleFromDict(
      const base::DictionaryValue* dict);

 private:
  DISALLOW_COPY_AND_ASSIGN(BackgroundTracingRule);

  double trigger_chance_;
  int trigger_delay_;
  bool stop_tracing_on_repeated_reactive_;
  BackgroundTracingConfigImpl::CategoryPreset category_preset_;
};

}  // namespace content

#endif  // CONTENT_BROWSER_TRACING_BACKGROUND_TRACING_RULE_H_
