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
 *
 * @file status/module_software_status.cpp
 * @brief ModuleSoftwareStatus stub implementation
 * */

#include "status/module_software_status.hpp"

using namespace agent::chassis::status;

ModuleSoftwareStatus::~ModuleSoftwareStatus() {}

agent_framework::status::ModuleStatus::Status ModuleSoftwareStatus::read_status() {
    return ModuleStatus::Status::PRESENT;
}
