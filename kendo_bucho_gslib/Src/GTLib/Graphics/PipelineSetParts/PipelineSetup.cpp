#include "PipelineSetup.h"
#include "PipelineStateID.h"
#include "GTLib/Graphics/Graphics.h"
#include "GTLib/Graphics/GraphicsParts/BlendState.h"
#include "GTLib/Graphics/GraphicsParts/d3dx12.h"
#include <d3dcompiler.h>

// 全デフォルトパイプラインステートの一括登録
void PipelineSetup::setup_defaults(DXGI_FORMAT rtv_format, DXGI_FORMAT dsv_format) {
    // スプライト用のパイプラインステートを作成
    create_pipeline_sprite2d(rtv_format, dsv_format);
    // ビルボード用のパイプラインステート作成
    create_pipeline_sprite3d(rtv_format, dsv_format);
    // メッシュ用のパイプラインステート作成
    create_pipeline_mesh(rtv_format, dsv_format);
    // スキニングメッシュ用のパイプラインステート作成
    create_pipeline_skinned_mesh(rtv_format, dsv_format);
    // インスタンス描画スキニングメッシュ用のパイプラインステート作成
    create_pipeline_instance_skinned_mesh(rtv_format, dsv_format);
    // スカイドーム用のパイプラインステート作成
    create_pipeline_skydome(rtv_format, dsv_format);
}
// スプライト用のパイプラインステートを作成
void PipelineSetup::create_pipeline_sprite2d(DXGI_FORMAT rtv_format, DXGI_FORMAT dsv_format) {
#ifdef _DEBUG
    DWORD compile_flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    DWORD compile_flag = 0;
#endif
    // スプライト用のパイプラインステートの作成
    ID3D10Blob* pCompileVS{};
    D3DCompileFromFile(L"Assets/Shader/Basic/Sprite2D.hlsl", NULL, NULL, "VS", "vs_5_0", compile_flag, 0, &pCompileVS, NULL);
    ID3D10Blob* pCompilePS{};
    D3DCompileFromFile(L"Assets/Shader/Basic/Sprite2D.hlsl", NULL, NULL, "PS", "ps_5_0", compile_flag, 0, &pCompilePS, NULL);
    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
    desc.InputLayout = { nullptr, 0 };                      // 入力頂点なし
    desc.pRootSignature = Graphics::get_root_signature()->root_signature();
    desc.VS = CD3DX12_SHADER_BYTECODE{ pCompileVS };
    desc.PS = CD3DX12_SHADER_BYTECODE{ pCompilePS };
    desc.RasterizerState = CD3DX12_RASTERIZER_DESC(CD3DX12_DEFAULT());
    desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;   // カリングなし
    desc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT());
    desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(CD3DX12_DEFAULT());
    desc.DepthStencilState.DepthEnable = FALSE;             // デプステストなし
    desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.NumRenderTargets = 1;
    desc.RTVFormats[0] = rtv_format;
    desc.DSVFormat = dsv_format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    PipelineState* pipeline_state{};
    // 不透明
    desc.BlendState.RenderTarget[0] = BlendState::Opaque;
    pipeline_state = Graphics::get_graphics_device()->create_pipeline_state(desc);
    Graphics::add_pipeline_state(PipelineStateID::Sprite2D_Opaque, pipeline_state);
    // 透明
    desc.BlendState.RenderTarget[0] = BlendState::Transparent;
    pipeline_state = Graphics::get_graphics_device()->create_pipeline_state(desc);
    Graphics::add_pipeline_state(PipelineStateID::Sprite2D_Transparent, pipeline_state);
    // 加算
    desc.BlendState.RenderTarget[0] = BlendState::Additive;
    pipeline_state = Graphics::get_graphics_device()->create_pipeline_state(desc);
    Graphics::add_pipeline_state(PipelineStateID::Sprite2D_Additive, pipeline_state);
    // シェーダーの解放
    pCompileVS->Release();
    pCompilePS->Release();
    // スプライト用パイプラインステートの設定（αブレンドをデフォルトにする）
    Graphics::set_sprite2d_pipeline_state(PipelineStateID::Sprite2D_Transparent);
}
// ビルボード用パイプラインステート作成 (不透明、半透明、加算)
void PipelineSetup::create_pipeline_sprite3d(DXGI_FORMAT rtv_format, DXGI_FORMAT dsv_format) {
#ifdef _DEBUG
    DWORD compile_flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    DWORD compile_flag = 0;
#endif
    // シェーダーのコンパイル
    ID3D10Blob* pCompileVS{};
    D3DCompileFromFile(L"Assets/Shader/Basic/Sprite3D.hlsl", NULL, NULL, "VS", "vs_5_0", compile_flag, 0, &pCompileVS, NULL);
    ID3D10Blob* pCompilePS{};
    D3DCompileFromFile(L"Assets/Shader/Basic/Sprite3D.hlsl", NULL, NULL, "PS", "ps_5_0", compile_flag, 0, &pCompilePS, NULL);

    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
    desc.InputLayout = { nullptr, 0 };
    desc.pRootSignature = Graphics::get_root_signature()->root_signature();
    desc.VS = CD3DX12_SHADER_BYTECODE{ pCompileVS };
    desc.PS = CD3DX12_SHADER_BYTECODE{ pCompilePS };

    // ラスタライザ設定 (カリングなし)
    desc.RasterizerState = CD3DX12_RASTERIZER_DESC(CD3DX12_DEFAULT());
    desc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
    // ブレンドステート（デフォルト）
    desc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT());
    // デプス設定 (デフォルト)
    desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(CD3DX12_DEFAULT());


    desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.NumRenderTargets = 1;
    desc.RTVFormats[0] = rtv_format;
    desc.DSVFormat = dsv_format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    PipelineState* pipeline_state{};

    // 1. 不透明 (デプス書き込みあり)
    desc.BlendState.RenderTarget[0] = BlendState::Opaque;
    desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
    pipeline_state = Graphics::get_graphics_device()->create_pipeline_state(desc);
    Graphics::add_pipeline_state(PipelineStateID::Sprite3D_Opaque, pipeline_state);

    // 2. アルファブレンド (デプス書き込みなし)
    desc.BlendState.RenderTarget[0] = BlendState::Transparent;
    desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
    pipeline_state = Graphics::get_graphics_device()->create_pipeline_state(desc);
    Graphics::add_pipeline_state(PipelineStateID::Sprite3D_Transparent, pipeline_state);

    // 3. 加算ブレンド (デプス書き込みなし)
    desc.BlendState.RenderTarget[0] = BlendState::Additive;
    desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
    pipeline_state = Graphics::get_graphics_device()->create_pipeline_state(desc);
    Graphics::add_pipeline_state(PipelineStateID::Sprite3D_Additive, pipeline_state);

    // コンパイルされたシェーダーを解放
    pCompileVS->Release();
    pCompilePS->Release();

    // デフォルトは半透明に設定
    Graphics::set_sprite3d_pipeline_state(PipelineStateID::Sprite3D_Transparent);
}
// メッシュ用のパイプラインステート作成
void PipelineSetup::create_pipeline_mesh(DXGI_FORMAT rtv_format, DXGI_FORMAT dsv_format) {
#ifdef _DEBUG
    DWORD compile_flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    DWORD compile_flag = 0;
#endif
    // メッシュ用のシェーダーを読み込む
    ID3D10Blob* pCompileVS{};
    D3DCompileFromFile(L"Assets/Shader/Basic/Mesh.hlsl", NULL, NULL, "VS", "vs_5_0", compile_flag, 0, &pCompileVS, NULL);
    ID3D10Blob* pCompilePS{};
    D3DCompileFromFile(L"Assets/Shader/Basic/Mesh.hlsl", NULL, NULL, "PS", "ps_5_0", compile_flag, 0, &pCompilePS, NULL);

    // 入力レイアウトの設定
    D3D12_INPUT_ELEMENT_DESC input_elements[] = {
        { "POSITION",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL",    0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",  0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
    desc.InputLayout = { input_elements, _countof(input_elements) };
    desc.pRootSignature = Graphics::get_root_signature()->root_signature();
    desc.VS = CD3DX12_SHADER_BYTECODE{ pCompileVS };
    desc.PS = CD3DX12_SHADER_BYTECODE{ pCompilePS };
    // ラスタライザ設定 (右手座標系に対応)
    desc.RasterizerState = CD3DX12_RASTERIZER_DESC(CD3DX12_DEFAULT());
    desc.RasterizerState.FrontCounterClockwise = TRUE; // 反時計回りを表面とする
    // ブレンドステートの設定（デフォルト）
    desc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT());
    // デプスステンシル設定（デフォルト）
    desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(CD3DX12_DEFAULT());

    desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.NumRenderTargets = 1;
    desc.RTVFormats[0] = rtv_format;
    desc.DSVFormat = dsv_format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    // メッシュ用パイプラインステートの作成 (不透明)
    desc.BlendState.RenderTarget[0] = BlendState::Opaque;
    PipelineState* pipeline_state =
        Graphics::get_graphics_device()->create_pipeline_state(desc);
    Graphics::add_pipeline_state(PipelineStateID::Mesh_Opaque, pipeline_state);

    pCompileVS->Release();
    pCompilePS->Release();

    // メッシュ用のデフォルトパイプラインステートを設定
    Graphics::set_mesh_pipeline_state(PipelineStateID::Mesh_Opaque);
}
// スキニングメッシュ用のパイプラインステート作成
void PipelineSetup::create_pipeline_skinned_mesh(DXGI_FORMAT rtv_format, DXGI_FORMAT dsv_format) {
#ifdef _DEBUG
    DWORD compile_flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    DWORD compile_flag = 0;
#endif
    // スキニングメッシュ用のシェーダー
    ID3D10Blob* pCompileVS{};
    D3DCompileFromFile(L"Assets/Shader/Basic/SkinnedMesh.hlsl", NULL, NULL, "VS", "vs_5_0", compile_flag, 0, &pCompileVS, NULL);
    ID3D10Blob* pCompilePS{};
    D3DCompileFromFile(L"Assets/Shader/Basic/SkinnedMesh.hlsl", NULL, NULL, "PS", "ps_5_0", compile_flag, 0, &pCompilePS, NULL);

    // 入力レイアウトの設定
    D3D12_INPUT_ELEMENT_DESC input_elements[] = {
        { "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "BLENDINDICES",  0, DXGI_FORMAT_R8G8B8A8_UINT,      0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
    desc.InputLayout = { input_elements, _countof(input_elements) };
    desc.pRootSignature = Graphics::get_root_signature()->root_signature();
    desc.VS = CD3DX12_SHADER_BYTECODE{ pCompileVS };
    desc.PS = CD3DX12_SHADER_BYTECODE{ pCompilePS };

    // ラスタライザ設定 (右手系に対応)
    desc.RasterizerState = CD3DX12_RASTERIZER_DESC(CD3DX12_DEFAULT());
    desc.RasterizerState.FrontCounterClockwise = TRUE; // 反時計回りを表面とする
    // ブレンドステートの設定（デフォルト）
    desc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT());
    // デプスステンシル設定（デフォルト）
    desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(CD3DX12_DEFAULT());

    desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.NumRenderTargets = 1;
    desc.RTVFormats[0] = rtv_format;
    desc.DSVFormat = dsv_format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    // スキニングメッシュ用パイプラインステートの作成 (不透明)
    desc.BlendState.RenderTarget[0] = BlendState::Opaque;
    PipelineState* pipeline_state =
        Graphics::get_graphics_device()->create_pipeline_state(desc);
    Graphics::add_pipeline_state(PipelineStateID::SkinnedMesh_Opaque, pipeline_state);

    pCompileVS->Release();
    pCompilePS->Release();

    // スキンメッシュのデフォルトPSO設定
    Graphics::set_skinned_mesh_pipeline_state(PipelineStateID::SkinnedMesh_Opaque);
}
// インスタンス描画スキニングメッシュ用のパイプラインステート作成
void PipelineSetup::create_pipeline_instance_skinned_mesh(DXGI_FORMAT rtv_format, DXGI_FORMAT dsv_format) {
#ifdef _DEBUG
    DWORD compile_flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    DWORD compile_flag = 0;
#endif
    // スキニングメッシュ用のシェーダー
    ID3D10Blob* pCompileVS{};
    D3DCompileFromFile(L"Assets/Shader/Basic/InstanceSkinnedMesh.hlsl", NULL, NULL, "VS", "vs_5_0", compile_flag, 0, &pCompileVS, NULL);
    ID3D10Blob* pCompilePS{};
    D3DCompileFromFile(L"Assets/Shader/Basic/InstanceSkinnedMesh.hlsl", NULL, NULL, "PS", "ps_5_0", compile_flag, 0, &pCompilePS, NULL);

    // 入力レイアウトの設定
    D3D12_INPUT_ELEMENT_DESC input_elements[] = {
        { "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,       0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "BLENDINDICES",  0, DXGI_FORMAT_R8G8B8A8_UINT,      0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
    desc.InputLayout = { input_elements, _countof(input_elements) };
    desc.pRootSignature = Graphics::get_root_signature()->root_signature();
    desc.VS = CD3DX12_SHADER_BYTECODE{ pCompileVS };
    desc.PS = CD3DX12_SHADER_BYTECODE{ pCompilePS };

    // ラスタライザ設定 (右手系に対応)
    desc.RasterizerState = CD3DX12_RASTERIZER_DESC(CD3DX12_DEFAULT());
    desc.RasterizerState.FrontCounterClockwise = TRUE; // 反時計回りを表面とする
    // ブレンドステートの設定（デフォルト）
    desc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT());
    // デプスステンシル設定（デフォルト）
    desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(CD3DX12_DEFAULT());

    desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.NumRenderTargets = 1;
    desc.RTVFormats[0] = rtv_format;
    desc.DSVFormat = dsv_format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    // スキニングメッシュ用パイプラインステートの作成 (不透明)
    desc.BlendState.RenderTarget[0] = BlendState::Opaque;
    PipelineState* pipeline_state =
        Graphics::get_graphics_device()->create_pipeline_state(desc);
    Graphics::add_pipeline_state(PipelineStateID::InstanceSkinnedMesh_Opaque, pipeline_state);

    pCompileVS->Release();
    pCompilePS->Release();

    // スキンメッシュのデフォルトPSO設定
    Graphics::set_instance_skinned_mesh_pipeline_state(PipelineStateID::InstanceSkinnedMesh_Opaque);
}
// スカイドーム用のパイプラインステート作成
void PipelineSetup::create_pipeline_skydome(DXGI_FORMAT rtv_format, DXGI_FORMAT dsv_format) {
#if defined(_DEBUG)
    UINT compile_flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
    UINT compile_flag = 0;
#endif
    // 頂点シェーダーのコンパイル
    ID3DBlob* pCompileVS{};
    D3DCompileFromFile(L"Assets/Shader/Basic/Skydome.hlsl", NULL, NULL, "VS", "vs_5_0", compile_flag, 0, &pCompileVS, NULL);
    // ピクセルシェーダーのコンパイル
    ID3DBlob* pCompilePS{};
    D3DCompileFromFile(L"Assets/Shader/Basic/Skydome.hlsl", NULL, NULL, "PS", "ps_5_0", compile_flag, 0, &pCompilePS, NULL);

    // 入力レイアウト
    D3D12_INPUT_ELEMENT_DESC input_elements[] = {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
    desc.InputLayout = { input_elements, _countof(input_elements) };
    desc.pRootSignature = Graphics::get_root_signature()->root_signature();
    desc.VS = CD3DX12_SHADER_BYTECODE{ pCompileVS };
    desc.PS = CD3DX12_SHADER_BYTECODE{ pCompilePS };
    // ラスタライザ設定
    desc.RasterizerState = CD3DX12_RASTERIZER_DESC(CD3DX12_DEFAULT());
    desc.RasterizerState.FrontCounterClockwise = TRUE; // 反時計回りを表にする
    // ブレンドステート（デフォルト）
    desc.BlendState = CD3DX12_BLEND_DESC(CD3DX12_DEFAULT());
    // 深度ステンシル設定
    desc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(CD3DX12_DEFAULT());
    desc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL; // 等しいか小さければ描画
    desc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO; // 深度書き込みを無効化

    desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
    desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    desc.NumRenderTargets = 1;
    desc.RTVFormats[0] = rtv_format;
    desc.DSVFormat = dsv_format;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;

    // パイプラインステートの作成
    desc.BlendState.RenderTarget[0] = BlendState::Opaque; //不透明 
    PipelineState* pipeline_state =
        Graphics::get_graphics_device()->create_pipeline_state(desc);
    Graphics::add_pipeline_state(PipelineStateID::Mesh_Skydome, pipeline_state);

    pCompileVS->Release();
    pCompilePS->Release();
}
