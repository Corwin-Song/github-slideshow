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
#ifndef HDDL_UNITE_FILE_HELPER_H
#define HDDL_UNITE_FILE_HELPER_H

#include <string>
#include <vector>

#include "HddlUnite.h"

namespace HddlUnite {
namespace FileUtils {
struct HDDLUNITE_API FileInfo {
    std::string name;
    size_t size;
    uint64_t mode;
};

/**
 * @brief Query a bunch of file information on remote device.
 * @param device The device that performs the query operation.
 * @param fileNames A vector that stores files that to be queried.
 * @param fileInfos The returned file information vector.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode queryFiles(const Device& device, const std::vector<std::string>& fileNames, std::vector<FileInfo>& fileInfos);

/**
 * @brief Query all file information on remote device.
 * @param device The device that performs the query operation.
 * @param fileInfos The returned file information vector.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode queryFiles(const Device& device, std::vector<FileInfo>& fileInfos);

/**
 * @brief copy file from local host to remote device.
 * @param device The device that the file copied to.
 * @param dstFile The name of destination file on remote device.
 * @param srcFile The path of the source file on local host.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode copyToDevice(const Device& device, const std::string& dstFile, const std::string& srcFile);

/**
 * @brief Copy file from remote device to local host.
 * @param device The device that the file copies from.
 * @param dstFile The path of destination file on local host.
 * @param srcFile The name of the source file on remote device.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode copyFromDevice(const Device& device, const std::string& dstFile, const std::string& srcFile);

/**
 * @brief Delete a list of files on remote device.
 * @param device The device that performs the file deletion operation.
 * @param fileNames A list of file name that to be deleted.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode deleteFiles(const Device& device, const std::vector<std::string>& fileNames);

/**
 * @brief Delete all files located in storage area on remote device.
 * @param device The device that performs the file deletion operation.
 * @return Status code of the operation. HDDL_OK if succeeded.
 */
HDDLUNITE_API HddlStatusCode deleteFiles(const Device& device);
}
}
#endif // HDDL_UNITE_FILE_HELPER_H
