#
# Copyright (C) 2019 Intel Corporation.
#
# This software and the related documents are Intel copyrighted materials,
# and your use of them is governed by the express license under which they
# were provided to you (normally a Nondisclosure Agreement). Unless the
# License provides, otherwise, you may not use, modify, copy, publish,
# distribute, disclose or transmit this software or the related documents
# without Intel's prior written permission.
#
# This software and the related documents are provided as is, with no
# express or implied warranties, other than those that are expressly
# stated in the License.
#

####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was package.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)

####################################################################################

list(APPEND CMAKE_MODULE_PATH, "${PACKAGE_PREFIX_DIR}/cmake")

include("${CMAKE_CURRENT_LIST_DIR}/HddlUnite.cmake")
