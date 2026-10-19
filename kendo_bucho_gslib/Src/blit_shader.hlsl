// 転送元のテクスチャ
Texture2D<float4> SrcTexture : register(t0);
SamplerState TextureSampler : register(s0);

// 出力頂点構造体
struct VS_OUTPUT
{
    float2 texcoord : TEXCOORD0;
    float4 position : SV_POSITION;
};

// 頂点シェーダー：頂点バッファなしで全画面を覆う三角形を生成
VS_OUTPUT VS(uint VertexID : SV_VertexID)
{
    VS_OUTPUT output;
    // 頂点番号 (0, 1, 2) から UV座標 (0.0~2.0, 0.0~2.0) を計算
    // 0番: (0, 0), 1番: (2, 0), 2番: (0, 2) となり、画面（0~1の範囲）を完全に覆う三角形となる
    output.texcoord = float2((VertexID << 1) & 2, VertexID & 2);
    // UV座標を正規化デバイス座標系（-1.0 ～ 1.0）の座標に変換
    output.position = float4(output.texcoord * float2(2.0f, -2.0f) + float2(-1.0f, 1.0f), 0.0f, 1.0f);
    return output;
}

// ピクセルシェーダー：レンダーターゲットテクスチャをサンプリングして描画
float4 PS(VS_OUTPUT input) : SV_Target
{
    return SrcTexture.Sample(TextureSampler, input.texcoord);
}