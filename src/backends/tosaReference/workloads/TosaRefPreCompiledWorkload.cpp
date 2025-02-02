//
// Copyright © 2022 Arm Ltd and Contributors. All rights reserved.
// SPDX-License-Identifier: MIT
//

#include "TosaRefPreCompiledWorkload.hpp"

namespace armnn
{

TosaRefPreCompiledWorkload::TosaRefPreCompiledWorkload(const PreCompiledQueueDescriptor& descriptor,
                                                       const WorkloadInfo& info)
    : BaseWorkload<PreCompiledQueueDescriptor>(descriptor, info)
    , m_workloadInfo(info)
{
    // Check that the workload is holding a pointer to a valid pre-compiled object
    if (m_Data.m_PreCompiledObject == nullptr)
    {
        throw InvalidArgumentException(
                "TosaRefPreCompiledWorkload requires a valid pre-compiled object (TosaSerializationHandler).");
    }
}

void TosaRefPreCompiledWorkload::Execute() const
{
    uint32_t numInputBuffers  = static_cast<uint32_t>(m_Data.m_Inputs.size());
    uint32_t numOutputBuffers = static_cast<uint32_t>(m_Data.m_Outputs.size());

    tosa::TosaSerializationHandler* handler = static_cast<tosa::TosaSerializationHandler*>(m_Data.m_PreCompiledObject);

    std::vector<std::string> input_names = handler->GetInputs();
    std::vector<std::string> output_names = handler->GetOutputs();

    TosaReference::IModelRunner runner;
    GraphStatus status;

    // Initialise the model runner with the TosaSerializationHandler
    status = runner.initialize(*handler);
    if(status != GraphStatus::TOSA_VALID)
    {
        throw armnn::Exception("An error has occurred while initialising the TOSA Reference Model.");
    }

    // Set the inputs
    for (uint32_t inputSlotIdx = 0; inputSlotIdx < numInputBuffers; ++inputSlotIdx)
    {
        DataType dataType = m_workloadInfo.m_InputTensorInfos[inputSlotIdx].GetDataType();
        switch (dataType)
        {
            case DataType::Float32:
                SetInput<float>(runner, input_names[inputSlotIdx], inputSlotIdx);
                break;
            default:
                throw armnn::Exception("Input data type is unsupported in TOSA Reference Backend.");
        }
    }

    // Run the TOSA Reference Model
    status = runner.run();
    if(status != GraphStatus::TOSA_VALID)
    {
        throw armnn::Exception("An error has occurred while running the TOSA Reference Model.");
    }

    // Gets the outputs
    for (uint32_t outputSlotIdx = 0; outputSlotIdx < numOutputBuffers; ++outputSlotIdx)
    {
        DataType dataType = m_workloadInfo.m_OutputTensorInfos[outputSlotIdx].GetDataType();
        switch (dataType)
        {
            case DataType::Float32:
                GetOutput<float>(runner, output_names[outputSlotIdx], outputSlotIdx);
                break;
            default:
                throw armnn::Exception("Output data type is unsupported in TOSA Reference Backend.");
        }
    }
}

template <typename T>
void TosaRefPreCompiledWorkload::SetInput(TosaReference::IModelRunner& runner,
                                          std::string inputName,
                                          uint32_t inputIndex) const
{
    std::vector<T> inputData(m_Data.m_Inputs[inputIndex]->GetShape().GetNumElements());
    m_Data.m_Inputs[inputIndex]->CopyOutTo(inputData.data());

    runner.setInput<T>(inputName, inputData);
}

template <typename T>
void TosaRefPreCompiledWorkload::GetOutput(TosaReference::IModelRunner& runner,
                                           std::string outputName,
                                           uint32_t outputIndex) const
{
    std::vector<T> actualOutputs = runner.getOutput<T>(outputName);

    m_Data.m_Outputs[outputIndex]->CopyInFrom(actualOutputs.data());
}

bool TosaRefPreCompiledWorkloadValidate(std::string*)
{
    return true;
}

}    //namespace armnn
