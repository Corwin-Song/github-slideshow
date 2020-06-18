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
#ifndef HDDL_UNITE_API_INFERENCE_DATA_H
#define HDDL_UNITE_API_INFERENCE_DATA_H

#include <memory>
#include <unordered_map>

#include "HddlUnite.h"
#include "InferBlob.h"

namespace HddlUnite {
namespace Inference {
class HDDLUNITE_API InferData;

class HDDLUNITE_API IInferDoneCallback {
public:
    virtual void operator()(InferData& inferData, HddlStatusCode resultCode, void* userData)
    {
        UNUSED(inferData);
        UNUSED(resultCode);
        UNUSED(userData);
    }
};

using TaskHandle = uint64_t;

class HDDLUNITE_API InferData : public NonCopyable {
public:
    struct ProfileItem {
        double time; //the total time for all rois
        double fps;
    };

    struct ProfileData {
        ProfileItem infer; // the whole inference time include pp,nn and communication time in this infer request
        ProfileItem nn; // do inference time on vpu in this infer request
        ProfileItem pp; // image preprocess time in this infer request
    };

    using Ptr = std::shared_ptr<InferData>;

    InferData(const std::vector<AuxBlob::Type>& types, const WorkloadContext::Ptr& wlContext, uint32_t maxRoiNum);
    ~InferData() override;

    /**
     * @brief Get the blob instance for input usage.
     * @param blobName The name of the inBlob.
     * @return An input blob instance.
     */
    InBlob::Ptr getInputBlob(const std::string& blobName) noexcept;

    /**
     * @brief Get the blob instance for output usage.
     * @param blobName The name of the outBlob.
     * @return An output blob instance.
     */
    OutBlob::Ptr getOutputBlob(const std::string& blobName) noexcept;

    /**
     * @brief Get the blob instance which maintains the auxiliary information.
     * @return The non-const reference to AuxInfo blob.
     */
    AuxBlob& getAuxBlob() noexcept;

    /**
     * @brief Get a WorkloadContext instance for this inference.
     * @return A WorkloadContext instance.
     */
    WorkloadContext::Ptr getContext() noexcept;

    /**
     * @brief Set number of shaves for SIPP.
     * @param shaveNum The number of shaves for SIPP, the default value is 16.
     */
    void setPPShaveNumber(uint32_t shaveNum = 4) noexcept;

    /**
     * @brief Register a blob with blobName and precision.
     * @param blobName The name of the blob.
     * @param precision The precision of the blob.
     * @param isInputBlob True if this blob is inBlob, false if this blob is outBlob.
     * @return True if this operation succeeded.
     */
    bool createBlob(const std::string& blobName, const BlobDesc& blobDesc, bool isInputBlob) noexcept;

    /**
     * @brief Set the BlobDesc for NN input, only need to be set in videoWorkload case.
     * @param desc The desc of NN input.
     * @return True if this operation succeeded.
     */
    bool setNNInputDesc(const NNInputDesc& desc) noexcept;

    /**
     * @brief Get NN input desc.
     * @return Const-reference of NN input desc.
     */
    const NNInputDesc& getNNInputDesc() const noexcept;

    /**
     * @brief Set inference callback which will be invoked when asynchronous inference completes.
     * @param callback The callback function, which should be derived from IInferDoneCallback class.
     * @param userData The auxiliary parameter, could be nullptr.
     */
    template <typename InferDoneCallback>
    void setInferDoneCallback(const InferDoneCallback& callback, void* userData) noexcept
    {
        std::shared_ptr<IInferDoneCallback> cb { std::make_shared<InferDoneCallback>(callback) };
        setInferDoneCallbackInternal(cb, userData);
    }

    /**
     * @brief Invoke the inference callback. Only for implementation usage.
     * @param message The inference done response message.
     */
    void invokeInferDoneCallback(void* message) noexcept;

    /**
     * @brief Wait inference done.
     * @param timeout The waiting timeout.
     * @return The status code of inference result, HDDL_OK if wait success.
     */
    HddlStatusCode waitInferDone(int64_t timeout);

    /**
     * @brief Get all inBlobs.
     * @return Const-reference to all inBlobs.
     */
    const std::vector<std::pair<std::string, InBlob::Ptr>>& getInBlobs() const noexcept;

    /**
     * @brief Get all outBlobs.
     * @return Const-reference to all outBlobs.
     */
    const std::vector<std::pair<std::string, OutBlob::Ptr>>& getOutBlobs() const noexcept;

    /**
     * @brief Get profiling data for inference if it is successful.
     * @return A ProfileData struct which include time and fps from infer,pp and nn.
     */
    ProfileData getProfileData() noexcept;

    /**
     * @brief Set preprocess flag for inference request
     * @param flag The needPP value to indicate using preprocess or not.
     */
    void setPPFlag(bool flag) noexcept;

public: //Only for implementation usage.
    /**
     * @brief Export information of the InferData to transmission message. Only for implementation usage.
     * @param message Transmission message in proto.
     * @return HDDL_OK if export to message success.
     */
    HddlStatusCode exportToMessage(void* message) const noexcept;

private:
    void setInferDoneCallbackInternal(std::shared_ptr<IInferDoneCallback>& callback, void* userData) noexcept;

private:
    class Impl;
    std::unique_ptr<Impl> m_impl;
};

/**
 * @brief Create a InferData instance.
 * @param types The types of aux information.
 * @param wlContext The workloadContext of this inference, only need to be set in videoWorkload case.
 * @param needPP True if need do preprocess, false if not need.
 * @param maxRoiNum The max value of ROI number, only need to be set if you need do preprocess.
 * @return The created InferData instance.
 */
InferData::Ptr makeInferData(const std::vector<AuxBlob::Type>& types, const WorkloadContext::Ptr& wlContext = nullptr, uint32_t maxRoiNum = 1) noexcept;

} // namespace Inference
} // namespace HddlUnite

#endif //HDDL_UNITE_API_INFERENCE_DATA_H
