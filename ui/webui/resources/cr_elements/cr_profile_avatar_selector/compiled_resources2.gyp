# Copyright 2016 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.
{
  'targets': [
    {
      'target_name': 'cr_profile_avatar_selector',
      'dependencies': [
        '../../js/compiled_resources2.gyp:icon',
        'cr_profile_avatar_selector_grid',
      ],
      'includes': ['../../../../../third_party/closure_compiler/compile_js2.gypi'],
    },
    {
      'target_name': 'cr_profile_avatar_selector_grid',
      'dependencies': [
        '<(DEPTH)/third_party/polymer/v1_0/components-chromium/iron-menu-behavior/compiled_resources2.gyp:iron-menubar-behavior-extracted',
        '<(DEPTH)/ui/webui/resources/js/compiled_resources2.gyp:assert',
      ],
      'includes': ['../../../../../third_party/closure_compiler/compile_js2.gypi'],
    },
  ],
}
