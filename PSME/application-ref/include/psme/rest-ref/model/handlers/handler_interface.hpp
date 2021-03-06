/*!
 * @section LICENSE
 *
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
 * */

#pragma once

#include "psme/rest-ref/endpoints/utils.hpp"

#include "agent-framework/module-ref/enum/common.hpp"
#include "agent-framework/eventing/event_data.hpp"

namespace psme {
namespace rest {
namespace model {
namespace handler {

template<typename Request, typename Model, typename IdPolicy>
class GenericHandler;

using agent_framework::eventing::EventData;

/*!
* @brief Base class for all handlers.
*
* Base class for handlers that handle events (ADD, REMOVE, UPDATE) from agents.
* Along with handling events (handle(...) method) it is used for polling (poll(...) method).
* */
struct HandlerInterface {
public:
    using Component = agent_framework::model::enums::Component::Component_enum;
    using Collection = agent_framework::model::attribute::Collection;
    using CollectionType = agent_framework::model::enums::CollectionType;
    using JsonAgent = psme::core::agent::JsonAgent;
    using JsonAgentSPtr = psme::core::agent::JsonAgentSPtr;
    using Event = psme::rest::eventing::Event;
    using EventVec = psme::rest::eventing::EventVec;
    using EventType = psme::rest::eventing::EventType;
    using ComponentStack = std::stack<Component>;
    template<typename T>
    using Array = agent_framework::model::attribute::Array<T>;

    /*!
     * @brief data that holds state of operation exexuted on handler
     * It is passed down during processing.
     */
    struct Context {
        enum class Mode : char {
            EVENT = 'E', POLLING = 'P', LOADING = 'L'
        };
        Mode mode{};
        ComponentStack stack{};

        Component get_parent_component() {
            return stack.size() > 0 ? stack.top() : Component::None;
        }

        std::string indent{};

        /*!
         * @brief json agent to talk to
         * */
        JsonAgent *agent{nullptr};
        EventVec events{};
        // this flag is used to limit number of emited events
        // Only event that comes from top-level added/removed resource should be reported.
        bool do_not_emit_from_descendats{false};

        u_int32_t num_added{0};
        u_int32_t num_removed{0};
        u_int32_t num_updated{0};

        void add_event(Component component, EventType event_type, const std::string &uuid) {
            events.emplace_back(Event(event_type,
                                      psme::rest::endpoint::utils::get_component_url(component, uuid)));
        }
    };

    /*!
     * @brief - constructor
     */
    HandlerInterface() { }

    virtual ~HandlerInterface();

    /*!
     * @brief removes a component and all its descendants from the model
     *
     * @param[in] uuid component's uuid
     * */
    virtual void remove(const std::string &uuid) = 0;

    /*!
     * @brief Remove from model all data pertaining to a given agent
     *
     * @param[in] gami_id GAMI ID of agent which data should be removed
     * */
    virtual void remove_agent_data(const std::string &gami_id) = 0;

    /*!
     * @brief ::handle() dispatches request to the right method: add(), remove() or update()
     *
     * @param[in] agent - json agent that is originator of the event
     * @param[in] event - event data (tells us WHAT and WHY)
     * @return status of handle - true if success
     *
     * 1. If add(..):
     * - it fetches out new resource from agent using parent_uuid and uuid given in request
     * - it recursively fetches (DFS) all subcomponents.
     * (Handler does not have knowledge about hierarchy of resources. I knows nothing about
     * the fact that System has Processors. It simply retrieves every known sub-collection
     * reported by resource)
     * 2. If remove(...):
     * - it removes the node from the tree as well as all its descendants
     * 3. If update():
     * - fetches out resource form agent and updates local model
     */
    virtual bool handle(JsonAgentSPtr agent, const EventData &event) = 0;

    /*!
     * @brief Similar to handle(...) but used to poll data asynchronously.
     *
     * @param[in] agent - json agent to talk to during polling
     * @param[in] parent_uuid - uuid of parent node
     * @param[in] uuid - start polling from this node
     *
     * */
    virtual void poll(JsonAgentSPtr agent, const std::string &parent_uuid, const std::string &uuid) = 0;

    /*!
     * @brief can be used by client to load resource on-demand.
     *
     * @param[in] agent - json agent to talk to while polling
     * @param[in] parent - uuid of parent node
     * @param[in] uuid - uuid of the node to reload
     * @param[in] recursively - do you want to retrieve descendats as well?
     *
     * Will be used by post,put,patch endpoints to reload data from agent
     */
    virtual std::uint64_t load(JsonAgentSPtr agent, const std::string &parent,
                               const std::string &uuid, bool recursively = false) = 0;

    /*!
     * @brief gets the current epoch from manager bound to this handler
     *
     * @return manager's current epoch
     */
    virtual std::uint64_t get_manager_epoch() = 0;

    /*!
     * @brief can be used by client to load resource's collection on-demand.
     *
     * @param[in] agent - json agent to talk to while loading
     * @param[in] parent - uuid of parent node
     * @param[in] collection_type - collection type we want to load
     * @param[in] recursively - do you want to retrieve descendats as well?
     *
     * Will be used by post,put,patch endpoints to reload data from agent
     */
    virtual void load_collection(JsonAgentSPtr agent, const std::string &parent,
                                 const CollectionType &collection_type, bool recursively) = 0;

protected:

    /*!
     * @brief Returns component for which this handler is targeted
     * @return component
     */
    virtual Component get_component() = 0;

    /*!
     * @brief Fetches from agent all components with given parent uuid
     *
     * @param[in] ctx State of the handler passed down when handling request
     * @param[in] parent_uuid uuid of parent component
     * @param[in] collection_name name of the collection to be fetched
     */
    virtual void fetch_siblings(Context &ctx, const std::string &parent_uuid,
                                const std::string &collection_name) = 0;

    /*!
     * @brief fetches list of all sibling uuids for given parent in collection with
     * given name
     *
     * @param[in] ctx State of the handler passed down when handling request
     * @param[in] parent_uuid uuid of parent component
     * @param[in] collection_name name of the collection
     *
     * @return list of uuids being siblings of parent in specific collection
     */
    virtual agent_framework::model::attribute::Array<agent_framework::model::attribute::SubcomponentEntry>
            fetch_sibling_uuid_list(Context &ctx, const std::string &parent_uuid,
                                    const std::string &collection_name) = 0;

    /*!
     * @brief removes all components that have not been touched after given epoch
     *
     * @param[in] ctx State of the handler passed down when handling request
     * @param[in] parent_uuid uuid of parent component
     * @param[in] epoch epoch to remove before
     */
    virtual void remove_untouched(Context &ctx, const std::string &parent_uuid, std::uint64_t epoch) = 0;

    /*!
     * @brief updates a single component
     *
     * @param[in] ctx State of the handler passed down when handling request
     * @param[in] parent uuid of parent component
     * @param[in] uuid uuid of updated component
     *
     * @return component's REST id
     */
    virtual std::uint64_t update(Context &ctx, const std::string &parent, const std::string &uuid) = 0;

    /*!
     * @brief removes all childrens of given parent (including descendants)
     *
     * @param[in] parent uuid of parent whose children should be removed
     */
    virtual void remove_all(const std::string &parent) = 0;

    template<typename Request, typename Model, typename IdPolicy>
    friend
    class psme::rest::model::handler::GenericHandler;
};

}
}
}
}
