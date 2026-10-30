#ifndef MESH_H_
#define MESH_H_

#include <string>
#include <vector>
#include <memory>
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
        // 定数バッファ
        std::unique_ptr<ConstantBuffer> constant_buffer;
        // テクスチャ
        std::unique_ptr<Texture>        texture;
    };
    // サブセット構造体
    struct Subset {
        int             material;           // マテリアル番号
        int             start;              // インデックスの開始位置
        int             count;              // インデックス数
    };

public:
    // コンストラクタ
    Mesh(GraphicsDevice* device);
    // デストラクタ
    ~Mesh();
    // ファイルの読み込み
    void load(const std::string& file_name);
    // 描画
    void draw(unsigned int instance_count=1,unsigned int instance_num_offset=0, unsigned int start_vartex=0) const;
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
    std::unique_ptr<IndexBuffer> indices_;
    // 頂点バッファ
    std::unique_ptr<VertexBuffer> vertices_;
};

#endif