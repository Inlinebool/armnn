#
# Copyright © 2022 Arm Ltd and Contributors. All rights reserved.
# SPDX-License-Identifier: MIT
#

include_directories(SYSTEM ${FLATBUFFERS_INCLUDE_PATH})
include_directories(SYSTEM ${PROJECT_SOURCE_DIR}/third-party/half)
include_directories(SYSTEM ${TOSA_SERIALIZATION_LIB_INCLUDE})
include_directories(SYSTEM ${TOSA_REFERENCE_MODEL_INCLUDE})

if(ARMNNTOSAREF)
    list(APPEND armnnTosaRefBackend_sources
        TosaRefBackend.cpp
        TosaRefBackend.hpp
        TosaRefBackendId.hpp
        TosaRefTensorHandle.hpp
        TosaRefTensorHandle.cpp
        TosaRefLayerSupport.cpp
        TosaRefLayerSupport.hpp
        TosaRefMemoryManager.hpp
        TosaRefMemoryManager.cpp
        TosaRefRegistryInitializer.cpp
        TosaRefWorkloadFactory.cpp
        TosaRefWorkloadFactory.hpp
        TosaRefTensorHandleFactory.cpp
        TosaRefTensorHandleFactory.hpp
    )

    add_subdirectory(workloads)

    if(BUILD_UNIT_TESTS)
        add_subdirectory(test)
    endif()

else()
    list(APPEND armnnTosaRefBackend_sources
        TosaRefBackendId.hpp
    )
endif()

add_library(armnnTosaRefBackend OBJECT ${armnnTosaRefBackend_sources})
target_include_directories(armnnTosaRefBackend PRIVATE ${PROJECT_SOURCE_DIR}/src/armnn)
target_include_directories(armnnTosaRefBackend PRIVATE ${PROJECT_SOURCE_DIR}/src/armnnUtils)
target_include_directories(armnnTosaRefBackend PRIVATE ${PROJECT_SOURCE_DIR}/src/backends)
target_include_directories(armnnTosaRefBackend PRIVATE ${PROJECT_SOURCE_DIR}/src/backends/tosaCommon)
