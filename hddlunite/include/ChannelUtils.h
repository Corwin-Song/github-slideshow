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
// Client of XLink channel manager.
// It will be used for XLink channel allocation and free.
//
// 1. Each KMB has itself independent channel IDs serials,  So it must specify which KMB will be allocated channel for.
//
// 2.  Channel map table is below( from PRD of Xlink, maybe changed in future)
//-------------------------------------------------------------------------------------------
//     Channel ID range     |   Remote to Local Host Interface  |  Local host to IP Interface|
//-------------------------------------------------------------------------------------------|
//              0x0         |           PCIE                    |            IPC             |
//              0x1         |           PCIE                    |            IPC             |
//           0x2 - 0x9      |           USB VSC                 |            IPC             |
//           0xA - 0x3FD    |           PCIE                    |            IPC             |
//              0x3FE       |           ETHERNET                |            IPC             |
//              0x3FF       |           ETHERNET                |            IPC             |
//              0x400       |           PCIE                    |          unmapped          |
//         0x401 - 0xFFE    |           PCIE                    |          unmapped          |
//              0xFFF       |           ETHERNET                |          unmapped          |
//--------------------------------------------------------------------------------------------
//
//            0x0   - 0x9         high speed channels
//            0xA   - 0x3FF       standard channels
//            0x400 - 0xFFF       host only channels
//      note: The following channel IDs are reserved for initialization, configure, shutdown and general control
//            0xA: IP control channel
//            0x400: VPU control channel(host only)
//
//    We should allocate channel as below:
//            0x3   - 0x9          passthrough channel, high speed
//            0xB  - 0x3FD         passthrough channel, standard speed
//            0x401 - 0xFFE        only Remote host(IA) to/from Local host(ARM)
//
// 3. This component is only charge of allocating an available channels, but not to open this channel.
//     Open and read/write this channel is application or VPUAL's business (If VPUAL need external allocated channel id).
// 4. This is a singleton instance in one process.
//
#ifndef HDDLUNITE_CHANNEL_UTILS_H
#define HDDLUNITE_CHANNEL_UTILS_H

#include <string>

#include "HddlUniteC.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PCIE_CHANNEL, // Channel b/w Host and ARM
    VPU_CHANNEL // Pass Through channel.
} ChannelType;

/**
 * @brief Allocate a valid XLink channel ID from HDDLUnite Device Service, which will call ResourceManager to do it(not implement?)
 *        Each XLink has a XLinkHandler, which is shared by all XLink channels for one KMB device
 *
 *        There will be several scenarios using xlink communication:
 *          Scenarios 1. Remote host APP --> XLink(PCIE) --> HDDLUnite Device Service
 *          Scenarios 2. HDDLUnite Service --> XLink(PCIE) --> HDDLUnite Device Service
 *          Scenarios 3. Remote host APP --> VPUAL --> XLink(PCIE) --> XLink Passthrough --> XLink(IPC) --> VPUIP
 *          Scenarios 4. HDDLUnite Service --> VPUAL --> XLink(PCIE) --> XLink Passthrough --> XLink(IPC) --> VPUIP
 *          Scenarios 5. Local host APP --> VPUAL --> XLink(IPC) --> VPUIP
 *
 *        For scenarios 1 & 2, the channelID range is [0x401, 0xFFE], deviceType == PCIE_DEVICE, XLink channel only between Remote host and Local host;
 *        For scenarios 3 & 4, the channelID range is [0x1, 0x9] & [0xB, 0x3FD], deviceType == VPU_DEVICE, XLink passthrough channel;
 *        For scenarios 5, the channelID range is [0x1, 0x9] & [0xB, 0x3FD].
 *
 *       Note:
 *         1). HDDLUnite Device Service will call Resource Manager(VPUAL need implement it) to allocate a channel
 *         2). allocate() should check the allocated channel if is valid.
 *         3). User can open this channel with proper operation mode, but need be charge of how to share XLinkHandler between multiple processes
 *         4). It should have a better way to pass channel id to the another side, such as name? It is up to the implement of XLink.
 *
 *       This function is synchronous operation, and will be blocked until get response from HDDLUnite Service.
 *
 * @param deviceHandle The device handle that operates the allocation of channelID.
 * @param channelType The type that indicates the usage of the new allocated channelID.
 * @param channelName The *unique* channel name that is used to retrieve the channelID.
 * @param channelIDPtr Containing the allocated channelID value.
 * @return The status code of the allocateChannel operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode allocateChannel(DeviceHandle deviceHandle, ChannelType channelType, const char* channelName, ChannelID* channelIDPtr);

/**
 * @brief Allocate a valid XLink channel ID from HDDLUnite Device Service, which will call ResourceManager to do it(not implement?)
 * @param deviceHandle The device handle that operates the allocation of channelID.
 * @param channelType The type that indicates the usage of the new allocated channelID.
 * @param channelIDPtr Containing the allocated channelID value.
 * @return The status code of the allocateChannel operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode allocateChannelUnnamed(DeviceHandle deviceHandle, ChannelType channelType, ChannelID* channelIDPtr);

/**
 * @brief Return the channelID to ResourceManager, which is a synchronous operation
 * @param deviceHandle The device that operates the release of channelID.
 * @param channelID The channelID value that should be released.
 * @return The status code of the free operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode releaseChannel(DeviceHandle deviceHandle, ChannelID channelID);
/**
 * @brief Check the channelID with a channelName.
 * @param deviceHandle The device that operates the release of channelID.
 * @param channelName The *unique* channel name that is used to retrieve the channelID.
 * @param channelIDPtr Containing the returned channelID for the channelName.
 * @return The status code of the allocateChannel operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode queryChannel(DeviceHandle deviceHandle, const char* channelName, ChannelID* channelIDPtr);

#ifdef __cplusplus
}
#endif

#endif //HDDLUNITE_CHANNEL_UTILS_H
