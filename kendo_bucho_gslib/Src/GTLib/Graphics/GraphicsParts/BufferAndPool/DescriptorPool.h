#ifndef DESCRIPTOR_POOL_H_
#define DESCRIPTOR_POOL_H_

#include <d3d12.h>
#include <vector>

// ディスクリプタハンドルクラス
class DescriptorHandle {
public:
    // コンストラクタ
    DescriptorHandle(const D3D12_CPU_DESCRIPTOR_HANDLE& cpu_descriptor_handle, const D3D12_GPU_DESCRIPTOR_HANDLE& gpu_descriptor_handle);
    // 確保する
    void allocate();
    // 解放する
    void release();
    // 使用中か？
    bool used() const;
    // CPUディスクリプタハンドルを返す
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_handle() const;
    // GPUディスクプリタハンドルを返す
    D3D12_GPU_DESCRIPTOR_HANDLE gpu_handle() const;

private:
    // CPUディスクリプタハンドル
    D3D12_CPU_DESCRIPTOR_HANDLE cpu_descriptor_handle_;
    // GPUディスクプリタハンドル
    D3D12_GPU_DESCRIPTOR_HANDLE gpu_descriptor_handle_;
    // 使用中か？
    bool allocated_{ false };
};

// ディスクリプタプールクラス
class DescriptorPool {
public:
    // コンストラクタ
    DescriptorPool(ID3D12Device* device, const D3D12_DESCRIPTOR_HEAP_DESC& heap_desc);
    // デストラクタ
    ~DescriptorPool();
    // ディスクプリタの確保
    DescriptorHandle* allocate();
    // ディスクプリタヒープの取得
    ID3D12DescriptorHeap* heap() const;

    // コピー禁止
    DescriptorPool(const DescriptorPool&) = delete;
    DescriptorPool& operator = (const DescriptorPool&) = delete;

private:
    // ディスクプリタヒープディスクリプタ
    D3D12_DESCRIPTOR_HEAP_DESC heap_desc_;
    // ディスクプリタヒープ
    ID3D12DescriptorHeap* heap_{};
    // ディスクプリタプール
    std::vector<DescriptorHandle> pool_;
    // 現在のディスクリプタの位置
    int current_position_{ 0 };
};

#endif

