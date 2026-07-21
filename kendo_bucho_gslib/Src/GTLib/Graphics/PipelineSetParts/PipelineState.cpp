#include "PipelineState.h"

// コンストラクタ
PipelineState::PipelineState(ID3D12Device* device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc) {
    // パイプラインステートの作成
    device->CreateGraphicsPipelineState(&desc, IID_PPV_ARGS(&pipeline_state_));
}

// デストラクタ
PipelineState::~PipelineState() {
    pipeline_state_->Release();
}

// パイプラインステートの取得
ID3D12PipelineState* PipelineState::pipeline_state() const {
    return pipeline_state_;
}