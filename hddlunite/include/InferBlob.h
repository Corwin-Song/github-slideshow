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
#ifndef HDDL_UNITE_CLIENT_HDDLBLOB_H
#define HDDL_UNITE_CLIENT_HDDLBLOB_H

#include <memory>
#include <vector>

#include "HddlUnite.h"
#include "RemoteMemory.h"

namespace HddlUnite {
namespace Inference {

enum FourCC {
    NV12,
    BGRA,
    BGRX,
    BGRP,
    BGR,
    RGBA,
    RGBX,
    RGBP,
    RGBP_F32,
    I420
};

enum Precision {
    UNSPECIFIED, /**< Unspecified value. Used by default */
    MIXED, /**< Mixed value. Can be received from network. No applicable for tensors */
    FP32, /**< 32bit floating point value */
    FP16, /**< 16bit floating point value */
    Q78, /**< 16bit specific signed fixed point precision */
    I16, /**< 16bit signed integer value */
    U8, /**< 8bit unsigned integer value */
    I8, /**< 8bit signed integer value */
    U16, /**< 16bit unsigned integer value */
    I32, /**< 32bit signed integer value */
    I64, /**< 64bit signed integer value */
    BIN, /**< 1bit integer value */
    CUSTOM /**< custom precision has it's own name and size of elements */
};

//note:Now we only use these 2 resize Algorithms,if we add other Algorithm, Please correspond to OpenCV interpolation.
enum ResizeAlgorithm {
    RESIZE_ALGO_BILINEAR = 1, /** bilinear interpolation */
    RESIZE_ALGO_BICUBIC = 2 /** bicubic interpolation */
};

struct Rectangle {
    int32_t x;
    int32_t y;
    int32_t width;
    int32_t height;
};

class BlobDesc {
public:
    explicit BlobDesc(Precision precision = U8, bool isRemoteMem = true, bool needAllocate = false, uint64_t dataSize = 0, bool isCompound = false)
        : m_precision(precision)
        , m_isRemoteMem(isRemoteMem)
        , m_needAllocate(needAllocate)
        , m_isCompound(isCompound)
        , m_dataSize(dataSize)
    {
    }

    BlobDesc(const BlobDesc& blobDesc) = default;
    BlobDesc& operator=(const BlobDesc& blobDesc) = default;

public:
    Precision m_precision { U8 };
    bool m_isRemoteMem { true };
    bool m_needAllocate { false };
    bool m_isCompound { false };
    FourCC m_format { NV12 };
    ResizeAlgorithm m_resizeAlgo { RESIZE_ALGO_BILINEAR };
    int m_fd { -1 };
    int m_fdExt { -1 };
    void* m_srcPtr { nullptr };
    uint64_t m_dataSize { 0 };
    int32_t m_resWidth { 0 };
    int32_t m_resHeight { 0 };
    int32_t m_widthStride { 0 };
    int32_t m_planeStride { 0 };
    std::vector<Rectangle> m_rect;
};

class NNInputDesc {
public:
    explicit NNInputDesc(Precision precision = U8, bool isRemoteMem = true, bool needAllocate = true,
        uint64_t dataSize = 0, uint32_t batch = 1)
        : m_precision(precision)
        , m_isRemoteMem(isRemoteMem)
        , m_needAllocate(needAllocate)
        , m_dataSize(dataSize)
        , m_batch(batch)
    {
    }

public:
    Precision m_precision { U8 };
    bool m_isRemoteMem { true };
    bool m_needAllocate { true };
    uint64_t m_dataSize { 0 };
    uint32_t m_batch { 1 };
};

class HDDLUNITE_API InBlob : public NonCopyable {
public:
    using Ptr = std::shared_ptr<InBlob>;

    InBlob(uint32_t maxRoiNum, const BlobDesc& blobDesc, const WorkloadContext::Ptr& context);
    ~InBlob() override;

    /**
     * @brief Update BlobDesc for the InBlob.
     * @param desc The desc of the blob.
     * @return True if this operation succeeded.
     */
    bool updateBlob(const BlobDesc& desc) noexcept;

    /**
     * @brief Get the BlobDesc of the Blob.
     * @return The const-reference of the BlobDesc.
     */
    const BlobDesc& getBlobDesc() const noexcept;

    /**
     * @brief Get the remote memory of the Blob.
     * @return The pointer to RemoteMemory object.
     */
    SMM::RemoteMemory::Ptr getRemoteMemory() const noexcept;

    /**
     * @brief Set preprocess flag for inference request
     * @param flag The needPP value to indicate using preprocess or not.
     */
    void setPPFlag(bool flag) noexcept;

public: //Only for implementation usage.
    /**
     * @brief Export blob information to transmission message. Only for implementation usage.
     * @param message Transmission message in proto.
     * @return The status code of the exportToMessage operation. HDDL_OK if succeeded.
     */
    HddlStatusCode exportToMessage(void* message) const noexcept;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

class HDDLUNITE_API OutBlob : public NonCopyable {
public:
    using Ptr = std::shared_ptr<OutBlob>;

    OutBlob(uint32_t maxRoiNum, const BlobDesc& blobDesc, const WorkloadContext::Ptr& context);
    ~OutBlob() override;

    /**
     * @brief Get Output Blob result data after inference done
     * @return result data
     */
    std::string getData() noexcept;

    /**
    * @brief Get the BlobDesc of the Blob.
    * @return The const-reference of the BlobDesc.
    */
    const BlobDesc& getBlobDesc() const noexcept;

    /**
     * @brief Get the remote memory of the Blob.
     * @return The pointer to RemoteMemory object.
     */
    SMM::RemoteMemory::Ptr getRemoteMemory() const noexcept;

public: //Only for implementation usage.
    /**
     * @brief Export blob information to transmission message. Only for implementation usage.
     * @param message Transmission message in proto.
     * @return The status code of the exportToMessage operation. HDDL_OK if succeeded.
     */
    HddlStatusCode exportToMessage(void* message) noexcept;

    /**
     * @brief Import blob information from received message. Only for implementation usage.
     * @param message Received message in proto.
     * @return The status code of the importFromMessage operation. HDDL_OK if succeeded.
     */
    HddlStatusCode importFromMessage(void* message) noexcept;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

class HDDLUNITE_API AuxBlob : public NonCopyable {
public:
    enum class Type {
        TimeTaken,
        Thermal,
        Debug
    };

    AuxBlob();
    ~AuxBlob() override;

    /**
     * @brief Get the buffer containing specified auxiliary information.
     * @param type The type of the aux information to be acquired.
     * @return The const reference to related binary data stream.
     */
    const std::string& getAuxData(Type type) const noexcept;

    /**
     * @brief Set the auxiliary information that this buffer contains, which will allocate a Blob instance to hold these information.
     *        Only for makeInferData implementation. This method will throw exception if input parameter is invalid.
     * @param types The vector that contains the mapping relation of the auxiliary information type and auxiliary information size
     */
    void setAuxType(const std::vector<Type>& types);

public: //Only for implementation usage.
    /**
     * @brief Export the AuxInfo description to internal message. Only for implementation usage.
     * @param message The output destination of the blob description.
     * @return The status code of the exportToMessage operation. HDDL_OK if succeeded.
     */
    HddlStatusCode exportToMessage(void* message) const noexcept;

    /**
     * @brief Import the AuxInfo description from internal message. Only for implementation usage.
     * @param message The InferenceDone message.
     * @return The status code of the importFromMessage operation. HDDL_OK if succeeded.
     */
    HddlStatusCode importFromMessage(void* message) noexcept;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

//Only for implementation usage.
/**
 * @brief Create an instance of InBlob
 * @param blobDesc The blobDesc of this InBlob.
 * @param context The workloadContext of this inference, only need to be set in videoWorkload case.
 * @return An InBlob instance.
 */
InBlob::Ptr makeInBlob(uint32_t maxRoiNum, const BlobDesc& blobDesc, const WorkloadContext::Ptr& context = nullptr) noexcept;

/**
 * @brief Create an instance of OutBlob
 * @param blobDesc The blobDesc of this OutBlob.
 * @param context The workloadContext of this inference, only need to be set in videoWorkload case.
 * @return An OutBlob instance.
 */
OutBlob::Ptr makeOutBlob(uint32_t maxRoiNum, const BlobDesc& blobDesc, const WorkloadContext::Ptr& context = nullptr) noexcept;
} // namespace Inference
} // namespace HddlUnite

#endif //HDDL_SERVICE_HDDLBUFFER_H
