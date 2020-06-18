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
#ifndef HDDL_UNITE_API_HDDL_QUERY_H
#define HDDL_UNITE_API_HDDL_QUERY_H

#include "HddlUnite.h"
#include "Parameter.h"
#include "QueryType.h"

namespace HddlUnite {

/**
 * @brief Query the status of service.
 * @param types The info type to query.
 * @param results The object that contains the info data.
 * @return Status code of the operation. HDDL_OK if all succeeded.
 */
HDDLUNITE_API HddlStatusCode query(const std::vector<QueryType>& types, std::vector<Parameter>& results);

/**
 * @brief Query the status of service.
 * @param types The info type to query.
 * @return The object that contains the info data.
 */
HDDLUNITE_API std::vector<Parameter> query(const std::vector<QueryType>& types);

} // namespace hddl

#endif // HDDL_UNITE_API_HDDL_QUERY_H
