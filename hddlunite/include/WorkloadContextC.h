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
#ifndef HDDLUNITE_WORKLOADCONTEXTC_H
#define HDDLUNITE_WORKLOADCONTEXTC_H

#include <stdint.h>

#include "HddlUniteC.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MEDIA_DECODE_TYPE_H264,
    MEDIA_DECODE_TYPE_H265
} MediaDecodeType;

typedef enum {
    SCHEDULE_POLICY_TYPE_ROUNDROBIN,
    SCHEDULE_POLICY_TYPE_SMART,
    SCHEDULE_POLICY_TYPE_UNDEFINED
} SchedulePolicyType;

HDDLUNITE_API HddlStatusCode getDeviceName(WorkloadID workloadId, const char** networkName);
HDDLUNITE_API HddlStatusCode getSwDeviceId(WorkloadID workloadId, uint32_t* deviceId);
HDDLUNITE_API HddlStatusCode getMediaType(WorkloadID workloadId, MediaDecodeType* mediaDecodeType);
HDDLUNITE_API HddlStatusCode getMediaBitrate(WorkloadID workloadId, float* mediaBitrate);
HDDLUNITE_API HddlStatusCode getResolutionWidth(WorkloadID workloadId, uint32_t* ResolutionWidth);
HDDLUNITE_API HddlStatusCode getResolutionHeight(WorkloadID workloadId, uint32_t* ResolutionHeight);
HDDLUNITE_API HddlStatusCode getMediaFps(WorkloadID workloadId, float* mediaFps);

/** APIs for VAAPI only */
/**
 * @brief get existed workload Id based on pid and tid, if cannot get one, then generate an unique workload Id
 *         based on pid and tid, workload Id can be used to identify the workloadContext.
 *     Below 3 terms can be looked as the same thing, other 2 can be get from another one.
 *       1).workloadContext
 *       2).pid & tid
 *       3).workloadId
 * @param pid The pid where this VAAPI caller running.
 * @param tid The tid where this VAAPI caller running.
 * @param workloadId The returned workload id for this workloadContext based on tid & pid.
 * @return Status code of the operation. HDDL_OK if succeeded.
 *
 * Supposed: To call setContext() and getWorkloadId() must be in the same PID/TID
 */
HDDLUNITE_API HddlStatusCode getWorkloadContextId(PID pid, TID tid, WorkloadID* workloadId);

/**
 * @brief allocate XLink channel id for this workload, it will trigger a callback to VAAPI KMB shim.
 * @param workloadId The workload Id for this codec session to which the VASurface belong.
 * @param channelId The channel id allocated for this workload.
 * @param size The number of channels allocated.
 * @return Status code of the operation. HDDL_OK if succeeded.
 *
 * Supposed: one codec session only need one xlink channel.
 *
 * The channel allocation for VAAPI shim is different with the generic channel allocation, VAAPI IA Shim
 * request channel allocation and the allocated channel id will be sent to VAAPI KMB shim by hddlunite device service.
 */
HDDLUNITE_API HddlStatusCode allocateVAChannelId(WorkloadID workloadId, ChannelID* channelId, int size);

/**
 * @brief get XLink channel id for this workload.
 *        There are two scenarios using this function:
 *          Scenarios 1. when channelId == nullptr, it returns the number of XLink channel id.
 *          Scenarios 2. when channelId != nullptr, it copies XLink channel id into the provided buffer.
 *        Note:
 *          Use this function twice in a row.
 * @param numEntries Input parameter, containing the total number of XLink channel id.
 * @param channelId Output parameter, a array containing all XLink channel id.
 * @param numChannels Output parameter, containing the total number of XLink channel id.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode getVAChannelId(WorkloadID workloadId, size_t numEntries, ChannelID* channelId, size_t* numChannels);

/**
 * @brief release XLink channel id of this workload.
 * @param workloadId The workload Id for this codec session to which the VASurface belong.
 * @param channelId The channel id will be released.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode releaseVAChannelId(WorkloadID workloadId, ChannelID channelId);

/**
 * @brief query the physical address of a VA surface, which is described by dmaBuf fd.
 * @param workloadId The workload Id for this codec session to which the VASurface belong.
 * @param dmaBufFd The dmaBuf Fd of this VASurface, the fd can be got by calling vaExportSurfaceHandle.
 * @param phyAddress The physical address for this dmaBuf of VASurface.
 * @param size The buffer size of VASurface.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode queryVASurfacePhyAddress(WorkloadID workloadId, int32_t dmaBufFd, PhyAddr* phyAddress, uint64_t* size);

/**
 * @brief release this dmaBuf fd.
 * @param workloadId The workload Id for this codec session to which the VASurface belong.
 * @param dmaBufFd The dmaBuf Fd of this VASurface, the fd can be got by calling vaExportSurfaceHandle.
 * @return true if successfully.
 */
HDDLUNITE_API HddlStatusCode releaseVASurfaceDmaBufFd(WorkloadID workloadId, int32_t dmaBufFd);

/** APIs for VAAPI internal use only */
typedef struct {
    MediaDecodeType mediaDecodeType;
    float mediaBitrate;
    uint32_t ResolutionWidth;
    uint32_t ResolutionHeight;
    float mediaFps;
    uint32_t internalWorkload;
} ContextHint;

/**
 * @brief create workloadContext and set contextHint for VAAPI shim only.
 * @param workloadId The workload Id for this codec session to which the VASurface belong.
 * @param contextHint The structure of input parameter to set contextHint in this workloadContext.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode createWorkloadContext(WorkloadID* workloadId, ContextHint* contextHint);

/**
 * @brief destroy the workloadContext for VAAPI test
 * @param workloadId The workload Id for this codec session to which the VASurface belong.
 */
HDDLUNITE_API void destroyWorkloadContext(WorkloadID workloadId);

#ifdef __cplusplus
}
#endif

#endif //HDDLUNITE_WORKLOADCONTEXTC_H
