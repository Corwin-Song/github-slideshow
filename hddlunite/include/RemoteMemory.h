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
// SMS(Shared/Remote memory service in bypass mode)
// on IA-Host, application / libs will call this service to, allocate/free/sync remote buffer.
//
// Every domain has its own wrapper to image buffer, composed of meta and memory pointer,
// convert between them is easy because they all support build from meta + pointer, and they
// all provide methods to get meta+pointer. for bypass mode, we just replace the pointer with
// an unique keembay buffer id (32bit is enough to encode  keembay_ID + Buf_id, the buf_id is
// keembay linux system-wide unique across multiple processes and also kernel space accessible).
//
// Device service will maintain the life-cycle of the buffer, every DMAbuf allocated on ARM and accessible
// by HDDL bypass IA-Host should be registered into device service and get a unique buffer ID, a kernel
// module (vpusmm) can be used to register a buffer into it and return corresponding unique ID, and also
// supports unregister to free the reference to DMABuf and release the ID. any process on ARM can get an
// access to the DMABuf fd from the unique ID. also there is a map recording relationships between unique ID
// and process fd, so if the same buffer is.
// So every domain needs extension to create vaSurface/RMat/RemoteBlob from meta+id. and api to return meta+id.
//
// app-crash: Since allocation and free request all go through HDDLUnite HOST service, it can handle app-crash by
// maintaining a data structure which maps process to all its allocated remote memory object.

#ifndef HDDL_UNITE_REMOTE_MEMORY_H
#define HDDL_UNITE_REMOTE_MEMORY_H

#include <memory>

#include "HddlUnite.h"
#include "WorkloadContext.h"

namespace HddlUnite {
namespace SMM {

class HDDLUNITE_API RemoteMemory : public NonCopyable {
public:
    using Ptr = std::shared_ptr<RemoteMemory>;

    /**
     * @brief Create a RemoteMemory instance, only used for video workload.
     * @param workloadContext The workloadContext should contain a device on which workload running on.
     *        If not device set to this workloadContext, the construct will failed, so user should
     *        check wlContext.getDevice() is not null before construct RemoteMemory by this method.
     * @param dmaBufFd The DMA buffer handle.
     * @param size The buffer size.
     * @param physAddr The physical address, it can be 0 if not get it.
     * @param isOwner True if this RemoteMemory is allocated by api allocate(...) interface, and will be released automatically in deconstruct function.
     *        False if this RemoteMemory by registered by VAAPI and need to be released by self.
     */
    RemoteMemory(const WorkloadContext& workloadContext, int dmaBufFd, size_t size = 0, PhyAddr physAddr = 0, bool isOwner = false);

    /**
     * @brief Destructor function which releases resource automatically.
     */
    ~RemoteMemory() override;

    /**
     * @brief Get the device that this remote memory belongs to.
     * @return The const reference of the device instance.
     */
    const Device& getDevice() const noexcept;

    /**
     * @brief Get the dma buf fd.
     * @return The returned dma buf fd.
     */
    int getDmaBufFd() const noexcept;

    /**
     * @brief Get the dma buf size.
     * @return The returned dma buf size.
     */
    size_t getBufSize() const noexcept;

    /**
     * @brief Get the dma buf physical address.
     * @return The returned dma buf physical address.
     */
    PhyAddr getPhysAddr() const noexcept;

    /**
     * @brief Update the dma buf fd.
     * @param fd The new fd of this remote memory.
     */
    void updateDmaBufFd(int32_t fd) noexcept;

    /**
     * @brief Push the content of local buffer to a buffer on device.
     * @param data The beginning address of the local buffer.
     * @param size The copy size.
     * @return Status code of the operation. HDDL_OK if succeeded.
     */
    HddlStatusCode syncToDevice(const void* data, size_t size) const noexcept;

    /**
     * @brief Fetch the content of a buffer on device to a local buffer.
     * @param data The beginning address of the local buffer for receiving.
     * @param size The size of the local buffer for receiving.
     * @return Status code of the operation. HDDL_OK if succeeded.
     */
    HddlStatusCode syncFromDevice(void* data, size_t size) const noexcept;

    /**
     * @brief Get workloadContext that the remoteMemory belong to.
     * @return the WorkloadContext instance, nullptr if not registered.
     */
    WorkloadContext::Ptr getWorkloadContext() const noexcept;

    /**
      * @brief set or get MetaData for this remoteMemory - only store but not parse them
      * @return Status code of the operation. HDDL_OK if succeeded.
      */
    HddlStatusCode setMetaData(const void* ptr, uint32_t size) noexcept;
    HddlStatusCode getMetaData(const void*& ptr, uint32_t& size) const noexcept;

public: // Only for implementation usage.
    /**
     * @brief Fetch meta data of a buffer on device to a local buffer. Only for implementation usage.
     * @param buffer The local buffer for receiving.
     */
    void syncFromDevice(std::string& buffer) const noexcept;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

/**
 * @brief Allocate a remote buffer on device with specified buffer size.
 * @param device The device that performs the allocation.
 * @param workloadContext The workload context contain a device on which the workload run,
 *        the buffer will be allocated on this device.
 *        Note: workloadContext is only used for video workload.
 * @param size The size of buffer to be allocated.
 * @return A RemoteMemory instance, nullptr if the allocation failed.
 */
HDDLUNITE_API RemoteMemory::Ptr allocate(const WorkloadContext& workloadContext, size_t size);

}
}
#endif
