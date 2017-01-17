// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "device/bluetooth/test/mock_bluetooth_gatt_characteristic.h"

#include "device/bluetooth/test/mock_bluetooth_gatt_service.h"

using testing::Return;
using testing::ReturnRefOfCopy;
using testing::_;

namespace device {

MockBluetoothGattCharacteristic::MockBluetoothGattCharacteristic(
    MockBluetoothGattService* service,
    const std::string& identifier,
    const BluetoothUUID& uuid,
    bool is_local,
    Properties properties,
    Permissions permissions) {
  ON_CALL(*this, GetIdentifier()).WillByDefault(Return(identifier));
  ON_CALL(*this, GetUUID()).WillByDefault(Return(uuid));
  ON_CALL(*this, IsLocal()).WillByDefault(Return(is_local));
  ON_CALL(*this, GetValue())
      .WillByDefault(ReturnRefOfCopy(std::vector<uint8_t>()));
  ON_CALL(*this, GetService()).WillByDefault(Return(service));
  ON_CALL(*this, GetProperties()).WillByDefault(Return(properties));
  ON_CALL(*this, GetPermissions()).WillByDefault(Return(permissions));
  ON_CALL(*this, IsNotifying()).WillByDefault(Return(false));
  ON_CALL(*this, GetDescriptors())
      .WillByDefault(Return(std::vector<BluetoothRemoteGattDescriptor*>()));
}

MockBluetoothGattCharacteristic::~MockBluetoothGattCharacteristic() {
}

}  // namespace device
