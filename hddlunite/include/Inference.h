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
#ifndef HDDLUNITE_INFERENCE_H
#define HDDLUNITE_INFERENCE_H

#include "InferData.h"
#include "InferGraph.h"

namespace HddlUnite {
namespace Inference {
/**
 * @brief Issue an inference task to hddldaemon and not return until it's finished.
 * @param graph The graph this inference is to be done on.
 * @param inferData The inference to be executed.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode inferSync(const Graph& graph, InferData::Ptr& inferData);

/**
 * @brief Issue an inference task to hddldaemon and return immediately.
 * @param graph The graph this inference is to be done on.
 * @param inferData The inference to be executed.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode inferAsync(const Graph& graph, InferData::Ptr& inferData);

/**
 * @brief Wait inference to be finished.
 * @param inferData The inference to wait.
 * @param timeOut Control the behavior of waitInfer.
 *        timeOut = -1 - wait infinitely
 *        timeOut =  0 - check inference status
 *        timeOut >  0 - wait for <timeOut> ms.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode waitInfer(const InferData::Ptr& inferData, int64_t timeOut);
} // namespace Inference
} // namespace HddlUnite

#endif //HDDLUNITE_INFERENCE_H
