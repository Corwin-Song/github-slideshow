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
#ifndef HDDL_UNITE_HDDL_UNITE_H
#define HDDL_UNITE_HDDL_UNITE_H

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "HddlUniteC.h"

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

#ifndef UNUSED
#define UNUSED(x) (void)x
#endif

namespace HddlUnite {
class HDDLUNITE_API NonCopyable {
public:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = delete;

    NonCopyable& operator=(const NonCopyable&) = delete;
    NonCopyable& operator=(NonCopyable&&) = delete;

protected:
    NonCopyable() = default;
    virtual ~NonCopyable() = default;
};

class HDDLUNITE_API Device {
public:
    using Ptr = std::shared_ptr<Device>;

    Device() = default;

    Device(uint64_t handle, std::string name, uint32_t swDeviceId)
        : m_handle(handle)
        , m_name(std::move(name))
        , m_swDeviceId(swDeviceId)
    {
    }

    Device(const Device& device) = default;
    Device& operator=(const Device& device) = default;

    /**
     * @brief Get the device handle.
     * @return The device handle.
     */
    uint64_t getHandle() const { return m_handle; }

    /**
     * @brief Get the device name.
     * @return The device name.
     */
    std::string getName() const { return m_name; }

    /**
     * @brief Get the sw device id.
     * @return The sw device id.
     */
    uint32_t getSwDeviceId() const { return m_swDeviceId; }

    /**
     * @brief Set the device name.
     * @param The device name.
     */
    void setName(std::string name) { m_name = std::move(name); }

    /**
     * @brief Set the device handle.
     * @param handle The device handle.
     */
    void setHandle(uint64_t handle) { m_handle = handle; }

    /**
     * @brief Get the device name.
    * @return The device name.
    */
    const char* getNameStr() const { return m_name.data(); }

    /**
     * @brief Compare two device instances.
     * @param other The other device instance.
     * @return Return true if success.
     */
    bool operator==(const Device& other) const noexcept
    {
        return m_name == other.getName() && m_handle == other.getHandle();
    }

private:
    uint64_t m_handle { 0 };
    std::string m_name;
    uint32_t m_swDeviceId;
};

/**
 * @brief Get all available KMB devices.
 * @param devices Output parameter, a vector containing all available devices.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode getAvailableDevices(std::vector<Device>& devices);

/**
 * @brief Check if hddl_scheduler_service is running.
 * @return True if hddl_scheduler_service is running.
 */
HDDLUNITE_API bool isServiceRunning();

}
#endif
