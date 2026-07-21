// オブジェクト用パラメータ (b0)
cbuffer ObjectBuffer : register(b0)
{
    matrix WorldMatrix;
};

// カメラ用パラメータ (b1)
cbuffer CameraBuffer : register(b1)
{
    matrix ViewMatrix;
    matrix ProjectionMatrix;
    float3 CameraPosition;
    float CameraPadding;
};

// ライト用パラメータ (b2)
cbuffer LightBuffer : register(b2)
{
    float4 LightAmbient;
    float4 LightDiffuse;
    float4 LightSpecular;
    float3 LightPosition;
    float LightPadding;
};

// マテリアル用パラメータ (b3)
cbuffer MaterialBuffer : register(b3)
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
    float4 MaterialEmission;
    float MaterialShininess;
    float3 MaterialPadding;
};

// ボーン行列 (b4)
cbuffer BoneBuffer : register(b4)
{
    matrix BoneMatrices[256]; // 最大256ボーン
}

// 頂点シェーダーの入力
struct VS_INPUT
{
    float4 Position : POSITION; // 座標
    float3 Normal : NORMAL; // 法線
    float2 TexCoord : TEXCOORD; // テクスチャ座標
    uint4 BlendIndices : BLENDINDICES; // ボーンの番号
    float4 BlendWeight : BLENDWEIGHT; // ボーンのウェイト
};

// 頂点シェーダーの出力
struct VS_OUTPUT
{
    float4 Position : SV_POSITION; // 座標( プロジェクション空間 )
    float2 TexCoord : TEXCOORD0; // テクスチャ座標
    float4 WorldPosition : TEXCOORD1; // ワールド座標系の頂点座標
    float3 WorldNormal : TEXCOORD2; // ワールド座標系の法線ベクトル
};

// 頂点シェーダ
VS_OUTPUT VS(VS_INPUT Input)
{
    VS_OUTPUT Output;
    // スキニング用の変換行列を作成
    matrix LocalMatrix = BoneMatrices[Input.BlendIndices.x] * Input.BlendWeight.x
                       + BoneMatrices[Input.BlendIndices.y] * Input.BlendWeight.y
                       + BoneMatrices[Input.BlendIndices.z] * Input.BlendWeight.z
                       + BoneMatrices[Input.BlendIndices.w] * Input.BlendWeight.w;
    // ローカル変換
    float4 LocalPosition = mul(LocalMatrix, Input.Position);
    // ワールド変換
    Output.WorldPosition = mul(WorldMatrix, LocalPosition);
    // ビュー変換
    float4 ViewPosition = mul(ViewMatrix, Output.WorldPosition);
    // 透視変換
    Output.Position = mul(ProjectionMatrix, ViewPosition);
    // スキニング後の法線ベクトルを求める
    float3 LocalNormal = mul((float3x3) LocalMatrix, Input.Normal);
    // ワールド座標系の法線ベクトルを求める
    Output.WorldNormal = mul((float3x3) WorldMatrix, LocalNormal);
    // テクスチャ座標の出力
    Output.TexCoord = Input.TexCoord;
    return Output;
}

// テクスチャ
SamplerState g_BaseMapSampler : register(s0);
Texture2D g_BaseMapTexture : register(t0);

// ピクセルシェーダ
float4 PS(VS_OUTPUT Input) : SV_Target
{
    // 法線ベクトルを正規化
    float3 N = normalize(Input.WorldNormal);
    // ライト方向のベクトルを求める
    float3 L = normalize(LightPosition.xyz - Input.WorldPosition.xyz);
    // 視点方向のベクトル
    float3 V = normalize(CameraPosition.xyz - Input.WorldPosition.xyz);
    // 2等分ベクトル
    float3 H = normalize(L + V);

    // 拡散反射光の計算
    float diffuse = max(dot(N, L), 0.0);
    // 鏡面反射の計算
    float specular = pow(max(dot(N, H), 0.0), MaterialShininess);
    // ベースカラーのテクスチャカラーの取得
    float4 baseColor = g_BaseMapTexture.Sample(g_BaseMapSampler, Input.TexCoord);

    // 最終的なカラーの計算
    float4 color = MaterialAmbient * LightAmbient * baseColor
                 + MaterialDiffuse * LightDiffuse * diffuse * baseColor
                 + MaterialSpecular * LightSpecular * specular
                 + MaterialEmission * baseColor;
    // アルファ値の設定
    color.a = baseColor.a * MaterialDiffuse.a;
    // 完全に透過色になっているピクセルは描画しない
    if (color.a <= 0.0f)
        discard;

    // 最終的なカラーの出力
    return color;
}