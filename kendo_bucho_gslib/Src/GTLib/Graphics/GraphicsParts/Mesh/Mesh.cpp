#include "Mesh.h"
#include "GTLib/Graphics/GraphicsParts/GraphicsDevice.h"
#include <fstream>
#include <filesystem>
#include <stdexcept>

// マテリアル用コンスタントバッファ構造体
struct MaterialParams {
    Color       ambient;    // 環境光
    Color       diffuse;    // 拡散光
    Color       specular;   // 鏡面反射光
    Color       emission;   // 自己発光
    float       shininess;  // 鏡面反射の鋭さ
    float       padding[3]; // アライメント用パディング
};
// コンストラクタ
Mesh::Mesh(GraphicsDevice* device) : device_{ device } {
}
// デストラクタ
Mesh::~Mesh() {
    clear();
}
// ファイルの読み込み
void Mesh::load(const std::string& file_name) {
    // メッシュファイルの読み込み
    std::ifstream file{ file_name, std::ios::binary };
    // ファイルのオープンに成功？
    if (!file) {
        // 失敗したときは例外を投げる
        throw std::runtime_error("Cannot Open File : " + file_name);
    }
    // 初期化
    clear();

    // メッシュファイルのパス
    std::filesystem::path mesh_path{ file_name };
    // メッシュファイルのフォルダをテクスチャファイルの基準フォルダとする
    std::filesystem::path texture_path = mesh_path.parent_path();

    // マテリアル配列の読み込み
    unsigned int num_material = 0;
    file.read(reinterpret_cast<char*>(&num_material), sizeof(num_material));
    for (unsigned int i = 0; i < num_material; ++i) {
        MaterialParams cb{};
        // マテリアルの読み込み
        file.read(reinterpret_cast<char*>(&cb.ambient), sizeof(Color));
        file.read(reinterpret_cast<char*>(&cb.diffuse), sizeof(Color));
        file.read(reinterpret_cast<char*>(&cb.specular), sizeof(Color));
        file.read(reinterpret_cast<char*>(&cb.emission), sizeof(Color));
        file.read(reinterpret_cast<char*>(&cb.shininess), sizeof(float));

        // テクスチャファイル名の読み込み（128バイト固定バッファ）
        char texture_file_name[128]{};
        file.read(texture_file_name, sizeof(texture_file_name));

        Material mat;
        // マテリアル定数バッファの作成
        mat.constant_buffer.reset(device_->create_constant_buffer(&cb, sizeof(MaterialParams)));
        // テクスチャの作成
        if (texture_file_name[0] != '\0') {
            mat.texture.reset(device_->create_texture((texture_path / texture_file_name).string()));
        }
        // マテリアルの追加
        materials_.push_back(std::move(mat));

    }

    // サブセットの読み込み
    unsigned int num_subset = 0;
    file.read(reinterpret_cast<char*>(&num_subset), sizeof(num_subset));
    subsets_.resize(num_subset);
    file.read(reinterpret_cast<char*>(subsets_.data()), sizeof(Subset) * num_subset);

    // インデックスバッファの読み込み
    // インデックスデータのストライドの読み込み
    unsigned int index_stride = 0;
    file.read(reinterpret_cast<char*>(&index_stride), sizeof(index_stride));
    unsigned int index_buffer_size = 0;
    // インデックスデータのバイトサイズを読み込み
    file.read(reinterpret_cast<char*>(&index_buffer_size), sizeof(index_buffer_size));
    // インデックスデータの読み込み
    std::vector<char> indices(index_buffer_size);
    file.read(indices.data(), index_buffer_size);

    // 頂点データの読み込み
    // 頂点データのストライドの読み込み
    unsigned int vertex_stride = 0;
    file.read(reinterpret_cast<char*>(&vertex_stride), sizeof(vertex_stride));
    // 頂点データのバイトサイズを読み込み
    unsigned int vertex_buffer_size = 0;
    file.read(reinterpret_cast<char*>(&vertex_buffer_size), sizeof(vertex_buffer_size));
    // 頂点データの読み込み
    std::vector<char> vertices(vertex_buffer_size);
    file.read(vertices.data(), vertex_buffer_size);
    // 頂点バッファの作成
    vertices_.reset(device_->create_vertex_buffer(vertices.data(), vertex_buffer_size, vertex_stride));
    // インデックスバッファの作成
    DXGI_FORMAT index_format = (index_stride == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;
    indices_.reset(device_->create_index_buffer(indices.data(), index_buffer_size, index_format));
}
// 描画
void Mesh::draw(unsigned int instance_count, unsigned int instance_num_offset,unsigned int start_vartex) const {
    // 頂点バッファとインデックスバッファの設定
    device_->set_vertex_buffer(0, vertices_.get());
    device_->set_index_buffer(indices_.get());
    // サブセットごとに描画
    for (const auto& subset : subsets_) {
        // マテリアル用コンスタントバッファの設定
        device_->set_constant_buffer(3, materials_[subset.material].constant_buffer.get());
        // マテリアル用テクスチャの設定
        device_->set_texture(0, materials_[subset.material].texture.get());
        // ポリゴンの描画
        device_->draw_indexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, subset.count, subset.start,start_vartex,instance_count,instance_num_offset);
    }
}
// 解放
void Mesh::clear() {
    // マテリアルの削除
    materials_.clear();
    // サブセットの消去
    subsets_.clear();
    // バッファの削除
    vertices_.reset();
    indices_.reset();
}