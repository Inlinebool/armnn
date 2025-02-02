//
// Copyright © 2020 Samsung Electronics Co Ltd and Contributors. All rights reserved.
// SPDX-License-Identifier: MIT
//
#pragma once

#include "LayerWithParameters.hpp"

namespace armnn
{

/// This layer represents a reduction operation.
class ReduceLayer : public LayerWithParameters<ReduceDescriptor>
{
public:
    /// Makes a workload for the Reduce type.
    /// @param [in] graph The graph where this layer can be found.
    /// @param [in] factory The workload factory which will create the workload.
    /// @return A pointer to the created workload, or nullptr if not created.
    virtual std::unique_ptr<IWorkload>CreateWorkload(const IWorkloadFactory& factory) const override;

    /// Creates a dynamically-allocated copy of this layer.
    /// @param [in] graph The graph into which this layer is being cloned.
    ReduceLayer* Clone(Graph& graph) const override;

    /// Check if the input tensor shape(s)
    /// will lead to a valid configuration of @ref ReduceLayer.
    void ValidateTensorShapesFromInputs() override;

    void ExecuteStrategy(IStrategy& strategy) const override;


protected:
    /// Constructor to create a ReduceLayer.
    /// @param [in] param ReduceDescriptor to configure the reduction operation.
    /// @param [in] name Optional name for the layer.
    ReduceLayer(const ReduceDescriptor& param, const char* name);

    /// Default destructor
    ~ReduceLayer() = default;
};

} // namespace armnn
