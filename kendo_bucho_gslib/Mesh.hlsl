// 3Dオブジェクト用パラメータ (b0)
cbuffer ObjectBuffer : register(b0) {
    matrix World;
};

// カメラ用パラメータ (b1)
cbuffer CameraBuffer : register(b1) {
    matrix View;
    matrix Projection;
    float3 CameraPosition;
    float  CameraPadding;
};

// ライト用パラメータ (b2)
cbuffer LightBuffer : register(b2) {
    float4 LightAmbient;
    float4 LightDiffuse;
    float4 LightSpecular;
    float3 LightPosition;
    float  LightPadding;
};

// マテリアル用パラメータ (b3)
cbuffer MaterialBuffer : register(b3) {
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float4 MaterialEmission;
    float  MaterialShininess;
    float3 MaterialPadding;
};

// 頂点シェーダーの入力
struct VS_INPUT {
    float4 position : POSITION;         // ローカル座標
    float3 normal   : NORMAL;           // 法線
    float2 texcoord : TEXCOORD;         // テクスチャ座標
};

// 頂点シェーダーの出力
struct VS_OUTPUT {
    float4 position      : SV_POSITION; // 座標 (プロジェクション空間)
    float2 texcoord      : TEXCOORD;    // テクスチャ座標
    float3 worldNormal   : NORMAL;      // ワールド座標系の法線
    float4 worldPosition : TEXCOORD1;   // ワールド座標系の頂点座標
};

// 頂点シェーダー
VS_OUTPUT VS(VS_INPUT input) {
    VS_OUTPUT output;

    // ワールド座標変換
    output.worldPosition = mul(World, input.position);
    // ビュー・プロジェクション変換
    float4 viewPos = mul(View, output.worldPosition);
    output.position = mul(Projection, viewPos);

    // 法線ベクトルの行列変換
    output.worldNormal = normalize(mul((float3x3)World, input.normal));
    // テクスチャ座標の出力
    output.texcoord = input.texcoord;

    return output;
}

// テクスチャとサンプラー
Texture2D BaseMapTexture : register(t0);
SamplerState BaseMapSampler : register(s0);

// ピクセルシェーダー
float4 PS(VS_OUTPUT input) : SV_Target {
    // 法線ベクトルの正規化
    float3 N = normalize(input.worldNormal);
    // ライトへのベクトル (ポイントライトとして計算)
    float3 L = normalize(LightPosition.xyz - input.worldPosition.xyz);
    // 視点方向のベクトル
    float3 V = normalize(CameraPosition.xyz - input.worldPosition.xyz);
    // ハーフベクトル (2等分ベクトル)
    float3 H = normalize(L + V);

    // 拡散反射の計算 (Lambert)
    float diffuse = max(dot(N, L), 0.0);
    // 鏡面反射の計算 (Blinn-Phong)
    float specular = pow(max(dot(N, H), 0.0), MaterialShininess);

    // テクスチャカラーのサンプリング
    float4 texColor = BaseMapTexture.Sample(BaseMapSampler, input.texcoord);
    // 最終的なカラーの計算
    float4 color = MaterialAmbient  * LightAmbient  * texColor           // 環境光
                 + MaterialDiffuse  * LightDiffuse  * diffuse * texColor // 拡散反射
                 + MaterialSpecular * LightSpecular * specular           // 鏡面反射
                 + MaterialEmission * texColor;                          // 自己発光
    
    // アルファ値の設定
    color.a = MaterialDiffuse.a * texColor.a;
    // 完全に透過色になっているピクセルは描画しない
    if (color.a <= 0.0f) discard;
    return color;
}

