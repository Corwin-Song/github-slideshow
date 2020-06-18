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
#ifndef HDDL_UNITE_API_REMOTE_PIPELINE_H
#define HDDL_UNITE_API_REMOTE_PIPELINE_H

#include <functional>

#include "ChannelUtils.h"
#include "HddlUnite.h"

namespace HddlUnite {
namespace RemotePipeline {

enum class Event {
    RemoteCrash,
    RemoteStop,
    UnknownEvent
};

using ROPCallback = std::function<void(uint64_t deviceId, uint64_t pipelineId, Event event, void* userData)>;

/**
 * @brief launch a remote offload pipeline instance.
 * @param device the device on which pipeline runs
 * @param channels The channel objects that allocated for this instance.
 * @param pipelineId The pipeline id will be allocated.
 * @return status code of the operation. HDDL_OK if succeeded.
*/
HDDLUNITE_API HddlStatusCode launchRemoteOffloadPipeline(const Device& device, const std::vector<ChannelID>& channels, uint64_t& pipelineId);

/**
 * @brief Register event callback, which will be called when specified event arrives from hddl service.
 * @param pipelineId the pipeline id on which callback will be set
 * @param callback The callback function that will be called.
 * @param userData The customized data that will be passed to callback function.
 * @return status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode setRemoteOffloadPipelineCallBack(uint64_t pipelineId, const ROPCallback& callback, void* userData);
}
}

#endif
