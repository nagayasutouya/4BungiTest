#ifndef PIPELINE_STATE_H_
#define PIPELINE_STATE_H_

#include <d3d12.h>

// パイプラインステートクラス
class PipelineState {
public:
    // コンストラクタ
    PipelineState(ID3D12Device* device, const D3D12_GRAPHICS_PIPELINE_STATE_DESC& desc);
    // デストラクタ
    ~PipelineState();
    // パイプラインステートの取得
    ID3D12PipelineState* pipeline_state() const;

    // コピー禁止
    PipelineState(const PipelineState&) = delete;
    PipelineState& operator = (const PipelineState&) = delete;

private:
    // パイプラインステート
    ID3D12PipelineState* pipeline_state_{};
};

#endif