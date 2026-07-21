// 3Dスプライト用パラメータ (b0)
cbuffer Sprite3DParam : register(b0) {
    float3 position;            // スプライトの描画位置
    float  angle;               // スプライトの回転角度
    float2 scale;               // スプライトの拡大率
    float2 size;                // スプライトのサイズ
    float2 center;              // スプライトの中心
    float2 padding;             // アライメント用パディング
    float4 color;               // スプライトの色
    float2 texture_position;    // テクスチャの切り出し開始位置
    float2 texture_size;        // テクスチャの切り出しサイズ
};

// カメラパラメータ (b1)
cbuffer CameraBuffer : register(b1) {
    matrix view_matrix;
    matrix projection_matrix;
    float3 view_position;
};

// 頂点シェーダーの出力
struct VS_OUTPUT {
    float2 texcoord : TEXCOORD0;
    float4 color    : COLOR;
    float4 position : SV_POSITION;
};

// バーテックスシェーダー
VS_OUTPUT VS(uint vertexID : SV_VertexID) {
    VS_OUTPUT output;
   
    // 頂点バッファなしで四角形を描画するための頂点座標 (Triangle Strip)
    static const float2 pos[4] =
    {
        float2(0.0, 1.0), // 左上
        float2(1.0, 1.0), // 右上
        float2(0.0, 0.0), // 左下
        float2(1.0, 0.0)  // 右下
    };
    output.position = float4(pos[vertexID], 0.0, 1.0);

    // スプライトサイズ分拡大する
    output.position.xy *= size;
    // スプライトの中心座標の補正
    output.position.xy -= center;
    // 拡大縮小
    output.position.xy *= scale;
    // 回転
    float s, c;
    sincos(angle, s, c);
    float2 rotate;
    rotate.x = output.position.x * c - output.position.y * s;
    rotate.y = output.position.x * s + output.position.y * c;
    output.position.xy = rotate;
    // 視野変換の逆変換 (カメラとは逆の方向を向かせておく）
    output.position.xyz = mul(output.position.xyz, (float3x3)view_matrix); // ベクトル x 行列

    // 平行移動
    output.position.xyz += position;
    // 視野変換
    output.position = mul(view_matrix, output.position);
    // 透視変換
    output.position = mul(projection_matrix, output.position);
    // テクスチャ座標の出力
    output.texcoord.xy = pos[vertexID] * texture_size + texture_position;
    // カラーの出力
    output.color = color;
   
    return output;
}

// テクスチャ
Texture2D Texture : register(t0);
SamplerState BaseMapSampler : register(s0);

// ピクセルシェーダー
float4 PS(VS_OUTPUT input) : SV_Target {
    float4 texColor = Texture.Sample(BaseMapSampler, input.texcoord);
    float4 color = texColor * input.color;
    // アルファ値が0.0の場合はピクセルを出力しない
    if (color.a <= 0.0) discard;
    return color;
}

