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

package com.intel.podm.discovery.external.matchers;

import com.intel.podm.business.entities.dao.EthernetInterfaceDao;
import com.intel.podm.business.entities.redfish.ComputerSystem;
import com.intel.podm.business.entities.redfish.EthernetInterface;
import com.intel.podm.client.api.ExternalServiceApiReaderException;
import com.intel.podm.client.api.reader.ResourceSupplier;
import com.intel.podm.client.api.resources.redfish.ComputerSystemResource;
import com.intel.podm.client.api.resources.redfish.EthernetInterfaceResource;
import com.intel.podm.common.logger.Logger;
import com.intel.podm.common.types.net.MacAddress;

import javax.enterprise.context.Dependent;
import javax.inject.Inject;
import java.util.ArrayList;
import java.util.List;
import java.util.Objects;

import static com.intel.podm.common.utils.StringRepresentation.fromList;
import static java.lang.String.format;
import static java.util.Collections.emptyList;

@Dependent
public class ComputerSystemFinder {

    @Inject
    private EthernetInterfaceDao ethernetInterfaceDao;

    @Inject
    private Logger logger;

    public ComputerSystem findByCorrelatedPsmeComputerSystem(ComputerSystemResource computerSystemResource) {
        List<MacAddress> macAddresses = getMacAddresses(computerSystemResource);
        return findByMacAddresses(macAddresses);
    }

    private ComputerSystem findByMacAddresses(List<MacAddress> macAddresses) {
        return macAddresses
                .stream()
                .map(this::findEthernetInterface)
                .filter(Objects::nonNull)
                .map(EthernetInterface::getComputerSystem)
                .filter(Objects::nonNull)
                .findFirst()
                .orElseThrow(
                        () -> {
                            String msg = fromList(macAddresses);
                            return new IllegalStateException(format("ComputerSystem has not been found for mac addresses '%s'", msg)
                            );
                        });
    }


    private EthernetInterface findEthernetInterface(MacAddress macAddress) {
        try {
            return ethernetInterfaceDao.getEthernetInterfaceByMacAddress(macAddress);
        } catch (IllegalStateException e) {
            logger.w("Found more than one EthernetInterface with MAC address: {}", macAddress);
            return null;
        }
    }

    private List<MacAddress> getMacAddresses(ComputerSystemResource computerSystemResource) {
        ArrayList<MacAddress> result = new ArrayList<>();

        for (ResourceSupplier supplier : getEthernetInterfaces(computerSystemResource)) {
            try {
                EthernetInterfaceResource ethernetInterface = (EthernetInterfaceResource) supplier.get();
                result.add(ethernetInterface.getMacAddress());
            } catch (ExternalServiceApiReaderException e) {
                continue;
            }
        }

        return result;
    }

    private Iterable<ResourceSupplier> getEthernetInterfaces(ComputerSystemResource computerSystemResource) {
        try {
            return computerSystemResource.getEthernetInterfaces();
        } catch (ExternalServiceApiReaderException e) {
            return emptyList();
        }
    }
}


