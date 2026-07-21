#include "GraphicsCommand.h"

// コンストラクタ
GraphicsCommand::GraphicsCommand(ID3D12Device* device, ID3D12CommandQueue* queue) : queue_{ queue } {
    // コマンドアロケーターの作成
    device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator_));
    // コマンドリストの作成
    device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator_, nullptr, IID_PPV_ARGS(&list_));
    // コマンドリストをクローズ
    list_->Close();
    // フェンスの作成
    device->CreateFence(fence_value_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_));
    // イベントの作成
    event_ = CreateEvent(nullptr, false, false, nullptr);
}

// デストラクタ
GraphicsCommand::~GraphicsCommand() {
    // GPUの実行を待機
    wait();
    CloseHandle(event_);
    fence_->Release();
    list_->Release();
    allocator_->Release();
}

// コマンドキューを返す
ID3D12CommandQueue* GraphicsCommand::queue() const {
    return queue_;
}

// コマンドリストを返す
ID3D12GraphicsCommandList* GraphicsCommand::list() const {
    return list_;
}

// コマンドリストのリセット
GraphicsCommand& GraphicsCommand::reset() {
    // コマンドアロケータのリセット
    allocator_->Reset();
    // コマンドリストのリセット
    list_->Reset(allocator_, nullptr);
    return *this;
}

// コマンドリストの実行
GraphicsCommand& GraphicsCommand::execute() {
    // コマンドリストの命令をクローズ
    list_->Close();
    // コマンドリストの実行
    ID3D12CommandList* lists[]{ list_ };
    queue_->ExecuteCommandLists(1, lists);
    // フェンスにシグナルを送る
    queue_->Signal(fence_, ++fence_value_);
    return *this;
}

// コマンドリストの実行を待つ
GraphicsCommand& GraphicsCommand::wait() {
    // コマンドリストの実行の終了を待つ
    //queue_->Signal(fence_, ++fence_value_);
    if (fence_->GetCompletedValue() < fence_value_) {
        fence_->SetEventOnCompletion(fence_value_, event_);
        WaitForSingleObject(event_, INFINITE);
    }
    return *this;
}

// 全てのコマンドの実行完了を待つ
GraphicsCommand& GraphicsCommand::wait_idle() {
    queue_->Signal(fence_, ++fence_value_);
    return wait();
}