//#ifndef SHADER_RESOURCE_VIEW_H_
//#define SHADER_RESOURCE_VIEW_H_
//
//#include <d3d12.h>
//#include "GTLib/Graphics/GraphicsParts/d3dx12.h"
//#include "GTLib/Math/Mathf.h"
//#include <memory>
//#include <vector>
//
//template <typename T>
//class ShaderResourceView {
//public:
//    // コンストラクタ
//    ShaderResourceView() {}
//    // デストラクタ
//    ~ShaderResourceView() {
//        buffer_->Unmap(0,nullptr);
//        // バッファの削除
//        buffer_->Release();
//    }
//    //初期化
//    void initialize(ID3D12Device* device, const void* data, UINT size, UINT instance_count = 1000){
//        //確保するバッファのサイズ
//        UINT buffer_size = sizeof(T) * instance_count;
//        // ヒーププロパティの設定(D3D12_HEAP_TYPE_UPLOAD:毎フレーム更新する CD3DX12_HEAP_PROPERTIES:更新は初期化時のみ)
//        CD3DX12_HEAP_PROPERTIES heap_prop{ D3D12_HEAP_TYPE_UPLOAD };
//        // リソースの設定(256バイトアライメント）
//        const D3D12_RESOURCE_DESC& res_desc{ CD3DX12_RESOURCE_DESC::Buffer(buffer_size) };
//        //SRVのプロパティ
//        view_.Format = DXGI_FORMAT_UNKNOWN;
//        view_.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
//        view_.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//        view_.Buffer.FirstElement = 0;
//        view_.Buffer.NumElements = instance_count;
//        view_.Buffer.StructureByteStride = sizeof(T);
//        view_.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
//        // SRV用リソースを作成
//        device->CreateCommittedResource(
//            &heap_prop,
//            D3D12_HEAP_FLAG_NONE,
//            &res_desc,
//            D3D12_RESOURCE_STATE_GENERIC_READ,
//            nullptr,
//            IID_PPV_ARGS(&buffer_)
//        );
//        //バッファの戦闘アドレス取得
//        buffer_->Map(0, nullptr, (void**)&buffer_ptr_);
//        if (data != nullptr) {
//            // 初期化データをコピーする
//            std::memcpy(buffer_ptr_, data, size);
//        }
//    }
//    //CPUのデータをGPUへ
//    D3D12_GPU_VIRTUAL_ADDRESS upload(const void* data,int data_size,UINT current_active=1000) {
//        //データ数が現状のアクティブ数を超えないようにする
//        int keep_size = Mathf::Min(data_size,current_active);
//        //描画データの更新
//        std::memcpy(buffer_ptr_,data,sizeof(T)*keep_size);
//        return buffer_->GetGPUVirtualAddress();
//    }
//    // コンスタントバッファビューを取得
//    const D3D12_SHADER_RESOURCE_VIEW_DESC& shader_resource_view() const {
//        return view_;
//    }
//    // コピー禁止
//    ShaderResourceView(const ShaderResourceView&) = delete;
//    ShaderResourceView& operator = (const ShaderResourceView&) = delete;
//private:
//    // バッファ
//    ID3D12Resource* buffer_;
//    //バッファに先頭ポインタ
//    char* buffer_ptr_{ nullptr };
//    // SRV
//    D3D12_SHADER_RESOURCE_VIEW_DESC view_;
//};
//
//#endif