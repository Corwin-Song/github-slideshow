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
#ifndef HDDL_UNITE_DEVICELOG_H
#define HDDL_UNITE_DEVICELOG_H

#include <functional>
#include <string>

#include "HddlUnite.h"

namespace HddlUnite {
namespace DeviceLog {
using ILogCallback = std::function<void(const std::string& log, void* userData)>;

enum class KmbLogLevel {
    EMERG_LEVEL,
    ALERT_LEVEL,
    CRIT_LEVEL,
    ERR_LEVEL,
    WARNING_LEVEL,
    NOTICE_LEVEL,
    INFO_LEVEL,
    DEBUG_LEVEL
};

enum class KmbLogType {
    SYSTEM_LOG,
    KERNEL_LOG,
    USER_LOG,
    MSG_LOG,
    HDDL_LOG,
};
/**
 * @brief Get all logs from remote device.
 * @param device The device that provides the log.
 * @param saveToPath Save the log files to this local path (file name: KMBID_FileNameOnDevice)
 * @param logType The log file which we need to get from device.
 * @return Status code of the operation. SUCCESS if succeeded.
 */
HDDLUNITE_API HddlStatusCode getLogs(const Device& device, const std::string& saveToPath, int32_t logType);

/**
 * @brief Clear all logs on remote device.
 * @param device The device that do the clear operation.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode clearLogs(const Device& device);

/**
 * @brief Set the callback function for when the log (>= the specified level) appears.
 * @param device The target device.
 * @param callback The ILogCallback instance that to be registered.
 * @param level the log level value as boundary.
 * @param userData The user data along with callback, could be nullptr.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode setLogCallback(const Device& device, const ILogCallback& callback, int32_t level, void* userData);
}
}
#endif
