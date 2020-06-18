//
//Copyright (C) 2019 Intel Corporation
//
//SPDX-License-Identifier: MIT
//
#ifndef HDDL_UNITE_QUERY_TYPE_H
#define HDDL_UNITE_QUERY_TYPE_H

namespace HddlUnite {
enum class QueryType {
    service_fps, // float
    max_waiting_req_num, // uint32
    device_name, // std::vector string
    device_fps, // std::vector float
    device_thermal, // std::vector float
    device_id, // std::vector uint64
    device_subclass, // std::vector uint32
    device_vpu_usage, // std::vector float
    device_cpu_usage, //std::vector float
    device_memory_used, // std::vector uint32
    device_memory_total, // std::vector uint32
    device_graph_map, // std::vector<std::vector<unit64>>
    device_tag, // std::vector string
    graph_tag, // std::vector string
    stream_id, // std::vector string
    graph_name, // std::vector string
    graph_id, // std::vector uint64
    graph_waiting_req_num, // std::vector uint32
    graph_running_device_num, // std::vector uint32
};
}

#endif
