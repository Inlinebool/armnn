#
# Copyright © 2022 Arm Ltd and Contributors. All rights reserved.
# SPDX-License-Identifier: MIT
#

include_directories(SYSTEM ${FLATBUFFERS_INCLUDE_PATH})
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/third-party/half)
include_directories(SYSTEM ${TOSA_SERIALIZATION_LIB_INCLUDE})

list(APPEND armnnTosaBackend_sources
        TosaMappings.hpp
        TosaMappings.cpp
        TosaLayerSupportRules.hpp
    )

add_subdirectory(operatorMappings)

if(BUILD_UNIT_TESTS)
    add_subdirectory(test)
endif()

add_library(armnnTosaBackend OBJECT ${armnnTosaBackend_sources})
target_include_directories(armnnTosaBackend PRIVATE ${PROJECT_SOURCE_DIR}/src/armnn)
target_include_directories(armnnTosaBackend PRIVATE ${PROJECT_SOURCE_DIR}/src/armnnUtils)
target_include_directories(armnnTosaBackend PRIVATE ${PROJECT_SOURCE_DIR}/src/backends)
target_include_directories(armnnTosaBackend PRIVATE ${PROJECT_SOURCE_DIR}/src/profiling)
target_include_directories(armnnTosaBackend PRIVATE ${PROJECT_SOURCE_DIR}/profiling/common/include)
target_include_directories(armnnTosaBackend PRIVATE ${PROJECT_SOURCE_DIR}/profiling/client/include)
