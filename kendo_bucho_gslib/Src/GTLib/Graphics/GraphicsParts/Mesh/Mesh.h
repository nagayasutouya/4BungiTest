#ifndef MESH_H_
#define MESH_H_

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

// メッシュクラス
class Mesh {
public:
    // マテリアル構造体
    struct Material {
        // コンスタントバッファ
        ConstantBuffer* constant_buffer{ nullptr };
        // テクスチャ
        Texture* texture{ nullptr };
    };
    // サブセット構造体
    struct Subset {
        int             material;           // マテリアル番号
        int             start;              // インデックスの開始位置
        int             count;              // インデックス数
    };
    // 頂点データ構造体
    struct Vertex {
        Vector3         position;           // 座標
        Vector3         normal;             // 法線ベクトル
        Vector2         texcoord;           // テクスチャ座標
        unsigned char   bone_indices[4];    // ボーンインデックス
        float           bone_weights[4];    // ボーンウェイト
    };

public:
    // コンストラクタ
    Mesh(GraphicsDevice* device);
    // デストラクタ
    ~Mesh();
    // ファイルの読み込み
    void load(const std::string& file_name);
    // 描画
    void draw() const;
    // 解放
    void clear();

    // コピー禁止
    Mesh(const Mesh&) = delete;
    Mesh& operator = (const Mesh&) = delete;

private:
    // グラフィックスデバイス
    GraphicsDevice* device_{ nullptr };
    // マテリアル配列
    std::vector<Material>   materials_;
    // サブセット配列
    std::vector<Subset>     subsets_;
    // インデックスバッファ
    IndexBuffer* indices_{ nullptr };
    // 頂点バッファ
    VertexBuffer* vertices_{ nullptr };
};

#endif