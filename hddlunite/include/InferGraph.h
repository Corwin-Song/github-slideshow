//
// Copyright (C) 2019 Intel Corporation.
//
// This software and the related documents are Intel copyrighted materials,
// and your use of them is governed by the express license under which they
// were provided to you (normally a Nondisclosure Agreement). Unless the
// License provides, otherwise, you may not use, modify, copy, publish,
// distribute, disclose or transmit this software or the related documents
// without Intel's prior written permission.
//
// This software and the related documents are provided as is, with no
// express or implied warranties, other than those that are expressly
// stated in the License.
//
#ifndef HDDL_UNITE_API_GRAPH_H
#define HDDL_UNITE_API_GRAPH_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "HddlUnite.h"
#include "InferData.h"

namespace HddlUnite {
namespace Inference {
class HDDLUNITE_API Graph : public NonCopyable {
public:
    using Ptr = std::shared_ptr<Graph>;

    explicit Graph(std::string name = {});
    ~Graph() override;

    /**
     * @brief Get graph name.
     * @return Graph name.
     */
    const std::string& getName() const noexcept;

    /**
     * @brief Get graph handle.
     * @return The graph handle.
     */
    uint64_t getHandle() const noexcept;

    /**
     * @brief Get the absolute path of the graph file.
     * @return The graph file path.
     */
    const std::string& getPath() const noexcept;

    /**
     * @brief Get the graph data.
     * @return The const reference to graph data.
     */
    const std::string& getData() const noexcept;

    /**
     * @brief Get the input data size.
     * @return The input data size.
     */
    size_t getInputSize() const noexcept;

    /**
     * @brief Get the output data size.
     * @return The output data size.
     */
    size_t getOutputSize() const noexcept;

    /**
     * @brief Get devices in graph
     * @return The device vector reference
     */
    const std::vector<Device>& getDevices() const noexcept;

    /**
     * @brief Set devices which loaded this graph
     * @param devices const Device vector
     */
    void setDevices(const std::vector<Device>& devices) noexcept;

public: //Only for implementation usage.
    /**
     * @brief Set graph data.  Only for load graph implementation.
     * @param data The graph data.
     */
    void setData(std::string&& data) noexcept;

    /**
     * @brief Set graph file path. Only for load graph implementation.
     * @param graphPath The graph file path.
     */
    void setPath(const std::string& graphPath) noexcept;

    /**
     * @brief Acquire content from message. Only for load graph implementation.
     * @param message The internal message.
     * @return True if operation success.
     */
    bool importFromMessage(const void* message) noexcept;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

/**
 * @brief Load a graph to hddldaemon from a file and let the daemon to schedule on MYX.
 * @param graph       Output parameter, nullptr if load graph failed.
 *                    NOTE: If config "UPDATE_RUNTIME_PRIORITY" is set, this api only updates the "RUNTIME_PRIORITY"
 *                          of existing graph and the graph pointer will not be set, and the application should not use it.
 * @param graphName   Graph name defined by user.
 * @param graphPath   Path to graph file.
 * @param vContext    The contexts contains devices that performs loading graph operation.
 * @param nnShaveNum  The shaveNum for NN flic, the default value is 16.
 * @param nnThreadNum The threadNum for NN flic, the default value is 1.
 * @param config      A map of configurations. Configurations includes:
 *                    "SUBCLASS":                Integer representing a firmware subclass, default: "0";
 *                    "GRAPH_TAG":               Arbitrary non-empty tag, if set to empty (""), equals to not set, default: ""; If set, the graph will be scheduled by Tag Scheduler.
 *                    "STREAM_ID":               Arbitrary non-empty id, if set to empty (""), equals to not set, default: ""; If set, the graph will be scheduled by Stream Scheduler.
 *                    "DEVICE_TAG":              Arbitrary non-empty tag, if set to empty (""), equals to not set, default: ""; If set, the graph will be scheduled by Bypass Scheduler.
 *                    "BIND_DEVICE":             "True"/"Yes" or "False"/"No", default: "False"; **Note: ONLY available when "DEVICE_TAG" is set.
 *                                               - If "True"/"Yes", inference through the Graph::Ptr returned, will only be run on one device
 *                                                 with the specified "DEVICE_TAG";
 *                                               - If "False"/"No", inference through the Graph::Ptr returned, will be run on all devices
 *                                                 (whose "BIND_DEVICE" is also set to "False" when they were loaded) with the same graph content.
 *                    "RUNTIME_PRIORITY":        Arbitrary integer, default: "0"; **Note: ONLY available when "DEVICE_TAG" is set and "BIND_DEVICE" is "False".
 *                                               When there are multiple devices running a certain graph (a same graph running on
 *                                               multiple devices in Bypass Scheduler), the device with a larger number has a higher priority,
 *                                               and more inference tasks will be fed to it with priority.
 *                    "UPDATE_RUNTIME_PRIORITY": "True"/"Yes" or "False"/"No", default: "False"; **Note: ONLY available when "DEVICE_TAG" is set.
 *                                               This config should be used only when the graph has been loaded already with the same graph content, the same
 *                                               "DEVICE_TAG" as used this time and "BIND_DEVICE" of the loaded graph has been set to "False".
 *                                               This config is only used to update the "RUNTIME_PRIORITY" of previous loaded graph, and the application should keep using
 *                                               the previous Graph::Ptr (instead of the one returned in this call) to do inference.
 *                                               - If "True"/"Yes": the "RUNTIME_PRIORITY" must be specified with a integer, and it will be set as the new
 *                                                 runtime priority for that graph on that device.
 *                                               - If "False"/"No": load graph to device.
 *                                               **Note: If "BIND_DEVICE" of the previously loaded graph was "True/Yes", the behavior of "update runtime priority" is undefined.
 *
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode loadGraph(Graph::Ptr& graph, const std::string& graphName, const std::string& graphPath, const std::vector<WorkloadContext>& vContext, int nnThreadNum = 1, int nnShaveNum = 16, const std::unordered_map<std::string, std::string>& config = {});

/**
 * @brief Load a graph to hddldaemon from a file and let the daemon to schedule on MYX.
 * @param graph         Output parameter, nullptr if load graph failed.
 *                      NOTE: If config "UPDATE_RUNTIME_PRIORITY" is set, this api only updates the "RUNTIME_PRIORITY"
 *                          of existing graph and the graph pointer will not be set, and the application should not use it.
 * @param graphName     Graph name defined by user.
 * @param graphData     Graph data in memory.
 * @param graphDataSize Graph data size.
 * @param vContext    The contexts contains devices that performs loading graph operation.
 * @param nnShaveNum    The shaveNum for NN flic, the default value is 16.
 * @param nnThreadNum   The threadNum for NN flic, the default value is 1.
 * @param config        A map of configurations.
 *                      Refer to: HddlStatusCode loadGraph(Graph::Ptr& graph, const std::string& graphName, const std::string& graphPath, const std::vector<Device>& devices = {}, int nnThreadNum = 1, int nnShaveNum = 16, const std::unordered_map<std::string, std::string>& config = {});
 *
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode loadGraph(Graph::Ptr& graph, const std::string& graphName, const void* graphData, size_t graphDataSize, const std::vector<WorkloadContext>& vContext, int nnThreadNum = 1, int nnShaveNum = 16, const std::unordered_map<std::string, std::string>& config = {});

/**
 * @brief Load a graph to hddldaemon from a file and let the daemon to schedule on MYX.
 * @param graph       Output parameter, nullptr if load graph failed.
 *                    NOTE: If config "UPDATE_RUNTIME_PRIORITY" is set, this api only updates the "RUNTIME_PRIORITY"
 *                          of existing graph and the graph pointer will not be set, and the application should not use it.
 * @param graphName   Graph name defined by user.
 * @param graphPath   Path to graph file.
 * @param devices     The devices that performs loading graph operation, can be empty.
 * @param nnShaveNum  The shaveNum for NN flic, the default value is 16.
 * @param nnThreadNum The threadNum for NN flic, the default value is 1.
 * @param config      A map of configurations. Configurations includes:
 *                    "SUBCLASS":                Integer representing a firmware subclass, default: "0";
 *                    "GRAPH_TAG":               Arbitrary non-empty tag, if set to empty (""), equals to not set, default: ""; If set, the graph will be scheduled by Tag Scheduler.
 *                    "STREAM_ID":               Arbitrary non-empty id, if set to empty (""), equals to not set, default: ""; If set, the graph will be scheduled by Stream Scheduler.
 *                    "DEVICE_TAG":              Arbitrary non-empty tag, if set to empty (""), equals to not set, default: ""; If set, the graph will be scheduled by Bypass Scheduler.
 *                    "BIND_DEVICE":             "True"/"Yes" or "False"/"No", default: "False"; **Note: ONLY available when "DEVICE_TAG" is set.
 *                                               - If "True"/"Yes", inference through the Graph::Ptr returned, will only be run on one device
 *                                                 with the specified "DEVICE_TAG";
 *                                               - If "False"/"No", inference through the Graph::Ptr returned, will be run on all devices
 *                                                 (whose "BIND_DEVICE" is also set to "False" when they were loaded) with the same graph content.
 *                    "RUNTIME_PRIORITY":        Arbitrary integer, default: "0"; **Note: ONLY available when "DEVICE_TAG" is set and "BIND_DEVICE" is "False".
 *                                               When there are multiple devices running a certain graph (a same graph running on
 *                                               multiple devices in Bypass Scheduler), the device with a larger number has a higher priority,
 *                                               and more inference tasks will be fed to it with priority.
 *                    "UPDATE_RUNTIME_PRIORITY": "True"/"Yes" or "False"/"No", default: "False"; **Note: ONLY available when "DEVICE_TAG" is set.
 *                                               This config should be used only when the graph has been loaded already with the same graph content, the same
 *                                               "DEVICE_TAG" as used this time and "BIND_DEVICE" of the loaded graph has been set to "False".
 *                                               This config is only used to update the "RUNTIME_PRIORITY" of previous loaded graph, and the application should keep using
 *                                               the previous Graph::Ptr (instead of the one returned in this call) to do inference.
 *                                               - If "True"/"Yes": the "RUNTIME_PRIORITY" must be specified with a integer, and it will be set as the new
 *                                                 runtime priority for that graph on that device.
 *                                               - If "False"/"No": load graph to device.
 *                                               **Note: If "BIND_DEVICE" of the previously loaded graph was "True/Yes", the behavior of "update runtime priority" is undefined.
 *
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode loadGraph(Graph::Ptr& graph, const std::string& graphName, const std::string& graphPath, const std::vector<Device>& devices = {}, int nnThreadNum = 1, int nnShaveNum = 16, const std::unordered_map<std::string, std::string>& config = {});

/**
 * @brief Load a graph to hddldaemon from a file and let the daemon to schedule on MYX.
 * @param graph         Output parameter, nullptr if load graph failed.
 *                      NOTE: If config "UPDATE_RUNTIME_PRIORITY" is set, this api only updates the "RUNTIME_PRIORITY"
 *                          of existing graph and the graph pointer will not be set, and the application should not use it.
 * @param graphName     Graph name defined by user.
 * @param graphData     Graph data in memory.
 * @param graphDataSize Graph data size.
 * @param devices       The devices that performs loading graph operation, can be empty.
 * @param nnShaveNum    The shaveNum for NN flic, the default value is 16.
 * @param nnThreadNum   The threadNum for NN flic, the default value is 1.
 * @param config        A map of configurations.
 *                      Refer to: HddlStatusCode loadGraph(Graph::Ptr& graph, const std::string& graphName, const std::string& graphPath, const std::vector<Device>& devices = {}, int nnThreadNum = 1, int nnShaveNum = 16, const std::unordered_map<std::string, std::string>& config = {});
 *
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode loadGraph(Graph::Ptr& graph, const std::string& graphName, const void* graphData, size_t graphDataSize, const std::vector<Device>& devices = {}, int nnThreadNum = 1, int nnShaveNum = 16, const std::unordered_map<std::string, std::string>& config = {});

/**
 * @brief Ask hddldaemon to unload a graph. Daemon will unload and release the graph when no more client using this graph.
 * @param graph   The graph instance to be unloaded.
 * @param devices The devices that performs unloading graph operation, can be empty.
 * @param config  A map of configurations.
 *                Refer to: HddlStatusCode loadGraph(Graph::Ptr& graph, const std::string& graphName, const std::string& graphPath, const std::vector<Device>& devices = {}, int nnThreadNum = 1, int nnShaveNum = 16, const std::unordered_map<std::string, std::string>& config = {});
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode unloadGraph(const Graph::Ptr& graph, const std::vector<Device>& devices = {}, const std::unordered_map<std::string, std::string>& config = {});
} // namesapce Inference
} // namespace HddlUnite
#endif
