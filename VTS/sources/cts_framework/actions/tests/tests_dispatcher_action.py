"""
 * @section LICENSE
 *
 * @copyright
 * Copyright (c) 2016 Intel Corporation
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
 * @section DESCRIPTION
"""


from cts_framework.actions.action import Action
from cts_framework.actions.tests.tests_list_action import TestsListAction
from cts_framework.actions.tests.tests_show_action import TestsShowAction


class TestsDispatchAction(Action):
    ACTION = "tests"
    PARAM_NAME = "ACTION"

    def fill_parser_arguments(self):
        parsers = self.parser.add_subparsers()
        self.tests_list_action = TestsListAction(parsers)
        self.tests_show_action = TestsShowAction(parsers)

    def process_action(self, configuration):
        {TestsListAction.ACTION: self.tests_list_action, TestsShowAction.ACTION: self.tests_show_action}[
            configuration.TESTS_CMD].process_action(configuration)
