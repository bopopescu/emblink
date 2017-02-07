// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef COMPONENTS_USER_PREFS_TRACKED_TRACKED_PREFERENCE_HISTOGRAM_NAMES_H_
#define COMPONENTS_USER_PREFS_TRACKED_TRACKED_PREFERENCE_HISTOGRAM_NAMES_H_

namespace user_prefs {
namespace tracked {

extern const char kTrackedPrefHistogramUnchanged[];
extern const char kTrackedPrefHistogramCleared[];
extern const char kTrackedPrefHistogramMigratedLegacyDeviceId[];
extern const char kTrackedPrefHistogramChanged[];
extern const char kTrackedPrefHistogramInitialized[];
extern const char kTrackedPrefHistogramTrustedInitialized[];
extern const char kTrackedPrefHistogramNullInitialized[];
extern const char kTrackedPrefHistogramWantedReset[];
extern const char kTrackedPrefHistogramReset[];
extern const char kTrackedSplitPrefHistogramChanged[];
extern const char kTrackedPrefRegistryValidationSuffix[];

}  // namespace tracked
}  // namespace user_prefs

#endif  // COMPONENTS_USER_PREFS_TRACKED_TRACKED_PREFERENCE_HISTOGRAM_NAMES_H_
