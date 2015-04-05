//
//  AbstractBridge.h
//  moka
//
//  Created by Firas Abuzaid on 1/22/15.
//  Copyright (c) 2015 Hazy Research. All rights reserved.
//

#ifndef moka_Abstract_Bridge_h
#define moka_Abstract_Bridge_h

#include "../LogicalCube.h"
#include "../Connector.h"
#include "../Kernel.h"
#include "../Report.h"
#include "../Scanner.h"
#include "PhysicalOperator.h"
#include "../Layer.h"
#include "../parser/cnn.pb.h"
#include "../algorithms/GradientUpdater.h"

#ifdef _GPU_TARGET
#include "../sched/DeviceDriver_GPU.h"
#endif
#include "../sched/DeviceDriver_CPU.h"

template
<typename InputLayerDataType, LayoutType InputLayerLayout,
  typename OutputLayerDataType, LayoutType OutputLayerLayout, typename DriverClass>
class AbstractBridge : public PhysicalOperator {
  protected:
    // the size of the current training batch
    // always <= iB
    size_t curr_B;
    LogicalCube<InputLayerDataType, InputLayerLayout> * input_d_cube;
    LogicalCube<InputLayerDataType, InputLayerLayout> * input_g_cube;
    LogicalCube<OutputLayerDataType, OutputLayerLayout> * output_d_cube;
    LogicalCube<OutputLayerDataType, OutputLayerLayout> * output_g_cube;

  public:
    std::string name; // lets give Bridge a name

    typedef Layer<InputLayerDataType, InputLayerLayout> InputLayerType;
    typedef Layer<OutputLayerDataType, OutputLayerLayout> OutputLayerType;

    const size_t iR, iC, iD, iB; // Size of the input data, LogicalCube 1
    const size_t oR, oC, oD, oB; // Size of the output data, LogicalCube 2

    InputLayerType * const p_input_layer;
    OutputLayerType * const p_output_layer;

    const cnn::LayerParameter * const layer_param;
    const cnn::SolverParameter * const solver_param;

    DriverClass * const p_driver;

    bool needs_to_calc_backward_grad;

    Report report_constructor;
    Report report_last_lowering;
    Report report_history;

    bool bias_term;

    void report_forward() {
        std::cout << std::endl;
        std::cout << "## FOWARD REPORT OF LAYER " << name << " ##" << std::endl;
        report_forward_last_transfer.print();
    }

    void report_backward() {
        std::cout << std::endl;
        std::cout << "## BACKWARD REPORT OF LAYER " << name << " ##" << std::endl;
        report_backward_updateweight_last_transfer.print();
    }

    // Bridges which subclass AbstractBridge may override these four methods later
    // (e.g. ConvolutionBridge). Most, however, won't, since only ConvolutionBridge
    // and FullyConnected Bridge have weights that need to be updated
    virtual void set_model_cube(LogicalCube<InputLayerDataType, InputLayerLayout> * model) {}

    virtual LogicalCube<InputLayerDataType, InputLayerLayout> * const get_model_cube() {
        return NULL;
    }

    virtual void set_bias_cube(LogicalCube<InputLayerDataType, InputLayerLayout> * bias) {}

    virtual LogicalCube<InputLayerDataType, InputLayerLayout> * const get_bias_cube() {
        return NULL;
    }

    virtual LogicalCube<InputLayerDataType, InputLayerLayout> * const get_model_grad_cube() {
        return NULL;
    }

    LogicalCube<InputLayerDataType, InputLayerLayout> * const get_bias_grad_cube() {
        return NULL;
    }

    // Need these for snapshot tests
    virtual GradientUpdater<InputLayerDataType, DriverClass> * const get_model_updater() {
        return NULL;
    }

    virtual GradientUpdater<InputLayerDataType, DriverClass> * const get_bias_updater() {
        return NULL;
    }

    void set_curr_batch_size(const size_t _curr_B) {
      curr_B = _curr_B;
    }

    // First constructor, which takes in a cnn::LayerParameter as a third argument. This will
    // be used when initializing from a *.prototxt file
    AbstractBridge<InputLayerDataType, InputLayerLayout, OutputLayerDataType,
      OutputLayerLayout, DriverClass>(InputLayerType * const _p_input_layer,
          OutputLayerType * const _p_output_layer, const cnn::LayerParameter * const _layer_param,
          const cnn::SolverParameter * const _solver_param, DriverClass * const _p_driver) :
        curr_B(_p_input_layer->p_data_cube->B), iR(_p_input_layer->p_data_cube->R),
        iC(_p_input_layer->p_data_cube->C), iD(_p_input_layer->p_data_cube->D),
        iB(_p_input_layer->p_data_cube->B), oR(_p_output_layer->p_data_cube->R),
        oC(_p_output_layer->p_data_cube->C), oD(_p_output_layer->p_data_cube->D),
        oB(_p_output_layer->p_data_cube->B), p_input_layer(_p_input_layer),
        p_output_layer(_p_output_layer), layer_param(_layer_param),
        solver_param(_solver_param), p_driver(_p_driver), bias_term(false) {

          input_d_cube = new LogicalCube<InputLayerDataType, InputLayerLayout>(iR, iC, iD, iB, p_driver);
          input_g_cube = new LogicalCube<InputLayerDataType, InputLayerLayout>(iR, iC, iD, iB, p_driver);
          output_d_cube = new LogicalCube<OutputLayerDataType, OutputLayerLayout>(oR, oC, oD, oB, p_driver);
          output_g_cube = new LogicalCube<OutputLayerDataType, OutputLayerLayout>(oR, oC, oD, oB, p_driver);
        }

    // Second constructor, which does NOT take in a cnn::LayerParameter as a third argument.
    // (Used only for Softmax)
    AbstractBridge<InputLayerDataType, InputLayerLayout, OutputLayerDataType,
      OutputLayerLayout, DriverClass>(InputLayerType * const _p_input_layer,
          OutputLayerType * const _p_output_layer, DriverClass * const _p_driver) :
        curr_B(_p_input_layer->p_data_cube->B),
        iR(_p_input_layer->p_data_cube->R), iC(_p_input_layer->p_data_cube->C),
        iD(_p_input_layer->p_data_cube->D), iB(_p_input_layer->p_data_cube->B),
        oR(_p_output_layer->p_data_cube->R), oC(_p_output_layer->p_data_cube->C),
        oD(_p_output_layer->p_data_cube->D), oB(_p_output_layer->p_data_cube->B),
        p_input_layer(_p_input_layer), p_output_layer(_p_output_layer),
        layer_param(NULL), solver_param(NULL), p_driver(_p_driver),
        bias_term(false) {

          input_d_cube = new LogicalCube<InputLayerDataType, InputLayerLayout>(iR, iC, iD, iB, p_driver);
          input_g_cube = new LogicalCube<InputLayerDataType, InputLayerLayout>(iR, iC, iD, iB, p_driver);
          output_d_cube = new LogicalCube<OutputLayerDataType, OutputLayerLayout>(oR, oC, oD, oB, p_driver);
          output_g_cube = new LogicalCube<OutputLayerDataType, OutputLayerLayout>(oR, oC, oD, oB, p_driver);
        }

    // This needs to be virtual, so we can delete the subclass bridges
    virtual ~AbstractBridge() {
      delete input_d_cube;
      delete input_g_cube;
      delete output_d_cube;
      delete output_g_cube;
    }
};

#endif
