// スプライトのパラメータ
cbuffer Sprite2DParams : register(b0) {
    float2 size;			// スプライトのサイズ
    float2 position;			// スプライトの描画位置
    float2 scale;			// スプライトの拡大率
    float angle;			// スプライトの回転角度
    float padding;			// パディング
    float2 center;			// スプライトの中心
    float2 screen;			// スクリーンのサイズ
    float4 color;			// スプライトの色
    float2 texture_position;	// テクスチャの切り出し開始位置
    float2 texture_size;		// テクスチャの切り出しサイズ
};

// 出力頂点構造体
struct VS_OUTPUT {
    float2 texcoord : TEXCOORD0;
    float4 color    : COLOR;
    float4 position : SV_POSITION;
};

// 頂点シェーダー
VS_OUTPUT VS(uint vertexID : SV_VertexID)
{
    VS_OUTPUT output;
    
    // 頂点バッファなしで四角形（Triangle Strip）を描画するための頂点座標
    static const float2 pos[4] = {
        float2(0.0, 0.0),
        float2(1.0, 0.0),
        float2(0.0, 1.0),
        float2(1.0, 1.0)
    };
    float2 local_pos = pos[vertexID];

    // 頂点座標
    output.position = float4(local_pos, 0.0, 1.0);
    // スプライトサイズ
    output.position.xy *= size;
    // 中心座標の適用
    output.position.xy -= center;
    // 拡大率の適用
    output.position.xy *= scale;
    
    // 回転の計算
    float s, c;
    sincos(angle, s, c);//サイン・コサインを同時に求める
    float2 rotate;
    rotate.x = output.position.x * c - output.position.y * s;
    rotate.y = output.position.x * s + output.position.y * c;
    output.position.xy = rotate;
    // 平行移動の計算
    output.position.xy += position;
    // 正規化デバイス座標系へ変換
    output.position.xy = output.position.xy * float2(2.0 / screen.x, 2.0 / -screen.y) + float2(-1.0, 1.0);
    // テクスチャ座標の計算
    output.texcoord.xy = local_pos * texture_size + texture_position;
    // カラーの出力    
    output.color = color;   
    return output;
}

// テクスチャ
Texture2D Texture : register(t0);
SamplerState BaseMapSampler : register(s0);

// ピクセルシェーダー
float4 PS(VS_OUTPUT input) : SV_Target {
    return Texture.Sample(BaseMapSampler, input.texcoord) * input.color;
}

