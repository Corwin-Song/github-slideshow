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
#ifndef HDDLUNITE_HDDLUNITEC_H
#define HDDLUNITE_HDDLUNITEC_H

#include <stddef.h>
#include <stdint.h>

#ifndef HDDLUNITE_API
#ifdef WIN32
#ifdef BUILD_HDDL_API
#define HDDLUNITE_API __declspec(dllexport)
#else
#define HDDLUNITE_API __declspec(dllimport)
#endif
#else
#define HDDLUNITE_API
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HDDL_TASK_NOT_FINISHED = 2, // Task not finished
    HDDL_TIMEOUT = 1, // Time out
    HDDL_OK = 0,

    HDDL_DEVICE_ERROR = -1, // Error occurs on keembay devices.
    HDDL_CONNECT_ERROR = -2, // Error occurs in communication between client and service.
    HDDL_GENERAL_ERROR = -3, // General error occurs.
    HDDL_INVALID_PARAM = -4, // Invalid parameter provided.
    HDDL_RESOURCE_BUSY = -5, // Computer resource is busy or not available.
    HDDL_OPERATION_ERROR = -6, // Incorrect operation.
    HDDL_ALLOC_ERROR = -7, // Allocate buffer failed.
    HDDL_PERMISSION_DENIED = -8, // Permission denied.
    HDDL_NOT_INITIALIZED = -9, // Some api used without initialize.
    HDDL_NOT_IMPLEMENTED = -10 // Feature not implemented.
} HddlStatusCode;

typedef uint64_t DeviceHandle;
typedef uint64_t WorkloadID;
typedef uint64_t TID;
typedef uint64_t PID;
typedef uint64_t PhyAddr;
typedef uint32_t ChannelID;

/**
 * @brief Get all available KMB device handles.
 *        There are two scenarios using this function:
 *          Scenarios 1. when deviceHandles == nullptr, it returns the number of all available KMB device handles.
 *          Scenarios 2. when deviceHandles != nullptr, it copies the device handles into the provided buffer.
 *        Note:
 *          Use this function twice in a row.
 * @param numEntries Input parameter, containing the total number of all available devices.
 * @param deviceHandles Output parameter, a array containing all available devices.
 * @param numDeviceHandles Output parameter, containing the total number of all available devices.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode getAvailableDevices(size_t numEntries, DeviceHandle* deviceHandles, size_t* numDeviceHandles);

/**
 * @brief Get all available sw device id of KMB device.
 *        There are two scenarios using this function:
 *          Scenarios 1. when numEntries >=  numDevice, it returns all device ids of all available KMB device.
 *          Scenarios 2. when numEntries <    numDevice, it return the first  numEntries device id of all available KMB device..
 * @param numEntries Input parameter, containing the space of deviceIdList.
 * @param deviceIdList Output parameter, a array containing memory of numEntries*sizeof(uint32_t).
 * @param numDevice Output parameter, real available device num.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode getAvailableDeviceIDs(size_t numEntries, uint32_t* deviceIdList, size_t* numDevice);

#ifdef __cplusplus
}
#endif

#endif //HDDLUNITE_HDDLUNITEC_H
