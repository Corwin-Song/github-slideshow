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
#ifndef HDDLUNITE_API_SETMODE_H
#define HDDLUNITE_API_SETMODE_H

#include <functional>

#include "HddlUnite.h"

namespace HddlUnite {

enum class HDDLMode {
    BYPASS,
    STREAMING,
    AUTONOMOUS,
    UNDEFINED
};

/**
 * @brief Set hddl mode(bypass mode, stream mode or autonomous mode) to run on device.
 *        hddl_scheduler_service will start related process according to the mode.
 *        This api can only be called once.
 * @param mode the mode to be set on device
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode setHDDLMode(HDDLMode mode) noexcept;

}

#endif //HDDLUNITE_API_SETMODE_H
