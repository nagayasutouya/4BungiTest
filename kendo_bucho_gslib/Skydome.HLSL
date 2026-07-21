// カメラ用パラメータ (b1)
cbuffer CameraBuffer : register(b1)
{
    matrix ViewMatrix;
    matrix ProjectionMatrix;
    float3 CameraPosition;
    float CameraPadding;
};

// 頂点シェーダーの入力
struct VS_INPUT
{
    float4 position : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

// 頂点シェーダーの出力
struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD;
};

// 頂点シェーダー
VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;

    // スカイドームはカメラの位置に追従するため、ビュー行列の平行移動成分を無視する
    // (float3x3)View により回転行列のみを抽出し、頂点を回転させる
    float3 viewPos = mul((float3x3) ViewMatrix, input.position.xyz);
    // プロジェクション変換
    output.position = mul(ProjectionMatrix, float4(viewPos, 1.0));
    // Z=W とすることで、デプスバッファにおいて常に最遠点（1.0）になるようにする
    // これにより、他のオブジェクトがスカイドームの背後に隠れることを防ぐ
    output.position.z = output.position.w;

    // テクスチャ座標を出力
    output.texcoord = input.texcoord;

    return output;
}

// テクスチャとサンプラー
Texture2D BaseMapTexture : register(t0);
SamplerState BaseMapSampler : register(s0);

// ピクセルシェーダー
float4 PS(VS_OUTPUT input) : SV_Target
{
    // スカイドームはライティングの影響を受けないため、テクスチャをそのまま出力する
    float4 texColor = BaseMapTexture.Sample(BaseMapSampler, input.texcoord);
    return texColor;
}

