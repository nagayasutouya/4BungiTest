#ifndef GRAPHICS_COMMAND_H_
#define GRAPHICS_COMMAND_H_

#include <d3d12.h>

// グラフィックスコマンドクラス
class GraphicsCommand {
public:
    // コンストラクタ
    GraphicsCommand(ID3D12Device* device, ID3D12CommandQueue* queue);
    // デストラクタ
    ~GraphicsCommand();
    // コマンドキューを返す
    ID3D12CommandQueue* queue() const;
    // コマンドリストを返す
    ID3D12GraphicsCommandList* list() const;
    // コマンドリストのリセット
    GraphicsCommand& reset();
    // コマンドリストの実行
    GraphicsCommand& execute();
    // コマンドリストの実行を待つ
    GraphicsCommand& wait();
    // すべてのコマンドリストの実行を待つ
    GraphicsCommand& wait_idle();
    // コピー禁止
    GraphicsCommand(const GraphicsCommand&) = delete;
    GraphicsCommand& operator=(const GraphicsCommand&) = delete;

private:
    // コマンドキュー
    ID3D12CommandQueue* queue_{};
    // コマンドアロケータ
    ID3D12CommandAllocator* allocator_{};
    // コマンドリスト
    ID3D12GraphicsCommandList* list_{};
    // フェンス
    ID3D12Fence* fence_{};
    // フェンスの値
    UINT64 fence_value_{};
    // イベント
    HANDLE event_{};
};

#endif