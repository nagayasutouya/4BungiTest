#ifndef INSTANCE_MESH_H_
#define INSTANCE_MESH_H_

#include <string>
#include <vector>
#include "GTLib/Math/Vector2.h"
#include "GTLib/Math/Vector3.h"
#include "GTLib/Math/Color.h"

// 前方宣言
class ConstantBuffer;
class Texture;
class VertexBuffer;
class IndexBuffer;
class GraphicsDevice;

// インスタンス描画用メッシュクラス
class InstanceMesh {
public:
    // マテリアル構造体
    struct InsMaterial {
        // コンスタントバッファ
        ConstantBuffer* constant_buffer{ nullptr };
        // テクスチャ
        Texture* texture{ nullptr };
    };
    // サブセット構造体
    struct InsSubset {
        int             material;           // マテリアル番号
        int             start;              // インデックスの開始位置
        int             count;              // インデックス数
    };
    // 頂点データ構造体
    struct InsVertex {
        Vector3         position;           // 座標
        Vector3         normal;             // 法線ベクトル
        Vector2         texcoord;           // テクスチャ座標
        unsigned char   bone_indices[4];    // ボーンインデックス
        float           bone_weights[4];    // ボーンウェイト
    };

public:
    // コンストラクタ
    InstanceMesh(GraphicsDevice* device);
    // デストラクタ
    ~InstanceMesh();
    // ファイルの読み込み
    void load(const std::string& file_name);
    // 描画
    void draw(unsigned int instance_count, unsigned int start_vartex=0) const;
    // 解放
    void clear();

    // コピー禁止
    InstanceMesh(const InstanceMesh&) = delete;
    InstanceMesh& operator = (const InstanceMesh&) = delete;

private:
    // グラフィックスデバイス
    GraphicsDevice* device_{ nullptr };
    // マテリアル配列
    std::vector<InsMaterial>   materials_;
    // サブセット配列
    std::vector<InsSubset>     subsets_;
    // インデックスバッファ
    IndexBuffer* indices_{ nullptr };
    // 頂点バッファ
    VertexBuffer* vertices_{ nullptr };
};

#endif