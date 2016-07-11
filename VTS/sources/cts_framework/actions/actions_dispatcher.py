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

from argparse import ArgumentParser

from cts_framework.actions.execute.execute_dispatch_action import ExecuteDispatchAction
from cts_framework.actions.show_version_action import ShowVersionAction
from cts_framework.actions.status.status_dispatcher_action import StatusDispatchAction
from cts_framework.actions.tests.tests_dispatcher_action import TestsDispatchAction
from cts_framework.db.database_connection_handler import DatabaseConnectionHandler


class ActionsDispatcher:
    ACTIONS_HANDLERS = [ExecuteDispatchAction, TestsDispatchAction, StatusDispatchAction, ShowVersionAction]

    def __init__(self):
        self.parser = ArgumentParser()
        subparsers = self.parser.add_subparsers()
        self.action_handlers = {action_handler.ACTION: action_handler(subparsers) for action_handler in
                                self.ACTIONS_HANDLERS}

    def process_aplication(self):
        DatabaseConnectionHandler.initialize_db_connection_handler()
        configuration = self.parser.parse_args()
        self.action_handlers[configuration.ACTION].process_action(configuration)
