/*
 * Copyright (c) 2015 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.intel.podm.client.api.resources.redfish;

import com.intel.podm.client.api.ExternalServiceApiReaderException;
import com.intel.podm.client.api.reader.ResourceSupplier;
import com.intel.podm.client.api.resources.ExternalServiceResource;
import com.intel.podm.common.types.Status;
import com.intel.podm.common.types.net.MacAddress;

import java.util.List;

public interface EthernetInterfaceResource extends ExternalServiceResource {
    Status getStatus();
    MacAddress getPermanentMacAddress();
    MacAddress getMacAddress();
    String getHostName();
    String getFqdn();
    Integer getMaxIPv6StaticAddresses();
    String getIpV6DefaultGateway();
    List<String> getNameServers();
    List<IpV4AddressObject> getIpV4Addresses();
    List<IpV6AddressObject> getIpV6Addresses();
    List<IpV6AddressObject> getIpV6StaticAddresses();
    List<IpV6AddressPolicyObject> getIpV6AddressesPolicyTable();
    Integer getMtuSize();
    Boolean getVlanEnable();
    Integer getVlanId();

    ResourceSupplier getComputerSystem();
    Iterable<ResourceSupplier> getVlans() throws ExternalServiceApiReaderException;
}
