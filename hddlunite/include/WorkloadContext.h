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
#ifndef HDDLUNITE_WORKLOADCONTEXT_H
#define HDDLUNITE_WORKLOADCONTEXT_H

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "HddlUnite.h"
#include "WorkloadContextC.h"

#ifdef DEBUG_WORKLOAD_CONTEXT
#define DEBUG_PRINT(format, ...) printf(format, __VA_ARGS__)
#else
#define DEBUG_PRINT(format, ...)
#endif

namespace HddlUnite {
class HDDLUNITE_API WorkloadContext {
public:
    using Ptr = std::shared_ptr<WorkloadContext>;
    WorkloadContext();
    ~WorkloadContext();

    WorkloadContext(const WorkloadContext&);
    WorkloadContext& operator=(const WorkloadContext&) = delete;

public:
    enum class Hint {
        DEVICE,
        MEDIA_DECODE_TYPE,
        MEDIA_DECODE_RESOLUTION_W,
        MEDIA_DECODE_RESOLUTION_H,
        MEDIA_DECODE_BITRATE,
        MEDIA_DECODE_FPS,
        INFERENCE_NETWORK_NAME,
        INFERENCE_NETWORK_HASH,
        INFERENCE_NETWORK_TOPS,
        INFERENCE_NETWORK_FPS,
        SCHEDULE_POLICY_TYPE,
        INTERNAL_WORKLOAD_CONTEXT
    };

    /**
     * @brief Provide hints of the pipeline.
        MEDIA_DECODE_TYPE           MEDIA_DECODE_TYPE_H264,MEDIA_DECODE_TYPE_H265, ...?
        MEDIA_DECODE_RESOLUTION_W   int_width
        MEDIA_DECODE_RESOLUTION_H   int_height
        MEDIA_DECODE_BITRATE        float
        MEDIA_DECODE_FPS            int
        INFERENCE_NETWORK_NAME      string
        INFERENCE_NETWORK_HASH      string (md5)
        INFERENCE_NETWORK_TOPS      float
        INFERENCE_NETWORK_FPS       float
        DEVICE_NAME                 string
        SCHEDULE_POLICY_TYPE        SCHEDULE_POLICY_TYPE_ROUNDROBIN, ...
    */

    template <typename T, typename... Args>
    bool setHint(Hint hint, T value, Args... args)
    {
        if (setHintCheck(hint, value, args...)) {
            DEBUG_PRINT("All args check successfully.");
            return _setHint(hint, value, args...);
        } else {
            std::cerr << "Error: Some Args did not pass check, please review the args you set." << std::endl;
            return false;
        }
    }

    template <typename T>
    bool getHint(Hint hint, T& value) const
    {
        return _getHint(hint, value);
    }

    /**
     * @brief Send this workload object to Scheduler together with the PID+TID of this thread.
     * Scheduler caches these workload objects and do scheduling when a scheduling
     * request with the same PID+TID comes.
     * The scheduling request usually comes from a decode component (the beginning of a
     * processing pipeline), the Scheduler combines the request with the workload object,
     * and do scheduling based on the workload object for the coming request.
     * Scheduler deletes this object from scheduler when the application process exits.
     * @param workloadId the returned workload id.
     * @return Status code of the operation. HDDL_OK if succeeded.
     */
    HddlStatusCode setContext(WorkloadID& workloadId);

    WorkloadID getWorkloadContextID() const noexcept;
    PID getWorkloadContextPid() const noexcept;
    TID getWorkloadContextTid() const noexcept;
    Device::Ptr getDevice() const noexcept;
    uint32_t getSwDeviceId() const noexcept;

    HddlStatusCode allocateVAChannelId(std::vector<ChannelID>& channelIds, int count) noexcept;
    HddlStatusCode getVAChannelId(std::vector<ChannelID>& channelIds) const noexcept;
    HddlStatusCode releaseVAChannelId(ChannelID channelId) noexcept;

private:
    HddlStatusCode eraseContext();

    template <typename T, typename... Args>
    bool setHintCheck(Hint hint, T value, Args... args)
    {
        return _setHintCheck(hint, value) && setHintCheck(args...);
    }

    static bool setHintCheck();
    static bool _setHintCheck(Hint hint, uint32_t value);
    static bool _setHintCheck(Hint hint, float value);
    static bool _setHintCheck(Hint hint, const std::string& value);
    static bool _setHintCheck(Hint hint, MediaDecodeType value);
    static bool _setHintCheck(Hint hint, SchedulePolicyType value);
    static bool _setHintCheck(Hint hint, Device::Ptr value);

    template <typename T, typename... Args>
    bool _setHint(Hint hint, T value, Args... args)
    {
        return __setHint(hint, value) && _setHint(args...);
    }

    static bool _setHint();
    bool __setHint(Hint hint, uint32_t value);
    bool __setHint(Hint hint, float value);
    bool __setHint(Hint hint, const std::string& value);
    bool __setHint(Hint hint, MediaDecodeType value);
    bool __setHint(Hint hint, SchedulePolicyType value);
    bool __setHint(Hint hint, Device::Ptr value);

    bool _getHint(Hint hint, uint32_t& value) const;
    bool _getHint(Hint hint, float& value) const;
    bool _getHint(Hint hint, std::string& value) const;
    bool _getHint(Hint hint, MediaDecodeType& value) const;
    bool _getHint(Hint hint, SchedulePolicyType& value) const;
    bool _getHint(Hint hint, const char*& value) const;
    bool _getHint(Hint hint, Device::Ptr& value) const;

    void copyData(const WorkloadContext&) noexcept;

private:
    WorkloadID m_workloadID { 0 };
    Device::Ptr m_device { nullptr };
    MediaDecodeType m_mediaDecodeType { MEDIA_DECODE_TYPE_H264 };
    SchedulePolicyType m_schedulePolicyType { SchedulePolicyType::SCHEDULE_POLICY_TYPE_UNDEFINED };
    uint32_t m_mediaDecodeResolutionW { 0 };
    uint32_t m_mediaDecodeResolutionH { 0 };
    float m_mediaDecodeBitrate { 0 };
    float m_mediaDecodeFps { 0 };
    std::string m_inferenceNetworkName;
    std::string m_inferenceNetworkHash;
    float m_inferenceNetworkTops { 0 };
    float m_inferenceNetworkFps { 0 };
    uint32_t m_internalWorkload { 0 };

    PID m_pid { 0 };
    TID m_tid { 0 };
    std::vector<ChannelID> m_vaChannelIDs;

    class Counter;
    std::shared_ptr<Counter> m_counter;
};

WorkloadContext::Ptr createWorkloadContext();
HddlStatusCode registerWorkloadContext(const WorkloadContext::Ptr& workloadContext);
void unregisterWorkloadContext(WorkloadID workloadId);
WorkloadContext::Ptr queryWorkloadContext(WorkloadID workloadId);
HddlStatusCode bindWorkloadContext(PID pid, TID tid, WorkloadID workloadId);
}

#endif //HDDLUNITE_WORKLOADCONTEXT_H
