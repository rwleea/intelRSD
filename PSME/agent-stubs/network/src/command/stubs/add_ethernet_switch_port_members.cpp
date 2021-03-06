/*!
 * @copyright
 * Copyright (c) 2015-2016 Intel Corporation
 *
 * @copyright
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * @copyright
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * @copyright
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * */

#include "agent-framework/command/network/add_ethernet_switch_port_members.hpp"
#include "agent-framework/logger_ext.hpp"

using namespace agent_framework;
using namespace agent_framework::command;
using namespace agent_framework::generic;

/*! Network command AddEthernetSwitchPortMembers */
class AddEthernetSwitchPortMembers final : public command::network::AddEthernetSwitchPortMembers {
public:
    /*! Command constructor */
    AddEthernetSwitchPortMembers() { }

    using agent_framework::command::network::AddEthernetSwitchPortMembers::execute;

    /*


     * @brief Execute command with given request and response argument
     *
     * @param[out]  response    Output response argument
     * */
    void execute(const Request&, Response&) {
        log_info(GET_LOGGER("rpc"),
            "AddEthernetSwitchPortMembers call, not implemented in the stub");
    }

    /*! Command destructor */
    ~AddEthernetSwitchPortMembers();
};

AddEthernetSwitchPortMembers::~AddEthernetSwitchPortMembers() { }

static Command::Register<AddEthernetSwitchPortMembers> g("Stubs");
