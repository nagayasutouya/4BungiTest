#include "DescriptorPool.h"

// コンストラクタ
DescriptorHandle::DescriptorHandle(const D3D12_CPU_DESCRIPTOR_HANDLE& cpu_descriptor_handle, const D3D12_GPU_DESCRIPTOR_HANDLE& gpu_descriptor_handle) :
    cpu_descriptor_handle_{ cpu_descriptor_handle },
    gpu_descriptor_handle_{ gpu_descriptor_handle } {
}

// 確保する
void DescriptorHandle::allocate() {
    allocated_ = true;
}

// 解放する
void DescriptorHandle::release() {
    allocated_ = false;
}

// 使用中か？
bool DescriptorHandle::used() const {
    return allocated_;
}

// CPUディスクリプタハンドルを返す
D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle::cpu_handle() const {
    return cpu_descriptor_handle_;
}

// GPUディスクプリタハンドルを返す
D3D12_GPU_DESCRIPTOR_HANDLE DescriptorHandle::gpu_handle() const {
    return gpu_descriptor_handle_;
}

// コンストラクタ
DescriptorPool::DescriptorPool(ID3D12Device* device, const D3D12_DESCRIPTOR_HEAP_DESC& heap_desc) :
    heap_desc_{ heap_desc } {
    // ディスクプリタヒープの作成
    device->CreateDescriptorHeap(&heap_desc_, IID_PPV_ARGS(&heap_));
    // ディスクプリタの先頭アドレスを取得
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle = heap_->GetCPUDescriptorHandleForHeapStart();
    D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle = heap_->GetGPUDescriptorHandleForHeapStart();
    // １つのディスクプリタのサイズを取得
    UINT descriptor_handle_size = device->GetDescriptorHandleIncrementSize(heap_desc_.Type);
    // ディスクプリタプールの初期化
    pool_.reserve(heap_desc_.NumDescriptors);
    for (UINT i = 0; i < heap_desc_.NumDescriptors; ++i) {
        pool_.emplace_back(cpu_handle, gpu_handle);
        cpu_handle.ptr += descriptor_handle_size;
        gpu_handle.ptr += descriptor_handle_size;
    }
    // 現在のディスクリプタの位置を初期化
    current_position_ = 0;
}

// デストラクタ
DescriptorPool::~DescriptorPool() {
    heap_->Release();
}

// ディスクリプタの確保
DescriptorHandle* DescriptorPool::allocate() {
    // 空いているディスクリプタを検索する
    for (UINT i = 0; i < heap_desc_.NumDescriptors; ++i) {
        // 現在の位置のディスクリプタを取得
        DescriptorHandle& descriptor_handle = pool_[current_position_];
        // 次の位置に更新する（末尾まで検索したら先頭に戻る）
        current_position_ = (current_position_ + 1) % heap_desc_.NumDescriptors;
        // ディスクリプタが未使用?
        if (!descriptor_handle.used()) {
            // 使用中にする
            descriptor_handle.allocate();
            // ディスクリプタを返す
            return &descriptor_handle;
        }
    }
    return nullptr; // 未使用のディクリプタハンドルが見つからない
}

// ディスクリプタヒープの取得
ID3D12DescriptorHeap* DescriptorPool::heap() const {
    return heap_;
}