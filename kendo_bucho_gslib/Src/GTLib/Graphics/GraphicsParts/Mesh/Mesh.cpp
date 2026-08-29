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
    std::ifstream file(file_name);
    // ファイルのオープンに成功？
    if (!file) {
        throw std::runtime_error("can not open " + file_name);
    }
    // 解放
    clear();

    // メッシュファイルのフォルダ取得
    std::filesystem::path mesh_path{ file_name };
    // テクスチャファイルのフォルダ
    std::string texture_path;
    // メッシュファイルにフォルダの指定があるか？
    if (mesh_path.has_parent_path()) {
        // メッシュファイルのフォルダをテクスチャファイルのフォルダとする
        texture_path = mesh_path.parent_path().string() + "/";
    }

    // マテリアル配列の読み込み
    int num_material = 0;
    file >> num_material;
    for (int i = 0; i < num_material; ++i) {
        MaterialParams cb;
        // マテリアルの読み込み
        file >> cb.ambient.r >> cb.ambient.g >> cb.ambient.b >> cb.ambient.a;
        file >> cb.diffuse.r >> cb.diffuse.g >> cb.diffuse.b >> cb.diffuse.a;
        file >> cb.specular.r >> cb.specular.g >> cb.specular.b >> cb.specular.a;
        file >> cb.emission.r >> cb.emission.g >> cb.emission.b >> cb.emission.a;
        file >> cb.shininess;
        // テクスチャファイルの読み込み
        std::string texture_file_name;
        file >> texture_file_name;

        Material mat;
        // マテリアル定数バッファの作成
        mat.constant_buffer = device_->create_constant_buffer(&cb, sizeof(MaterialParams));
        // テクスチャの作成
        mat.texture = device_->create_texture(texture_path + texture_file_name);
        // マテリアルの追加
        materials_.push_back(mat);
    }

    // サブセットの読み込み
    int num_subset = 0;
    file >> num_subset;
    for (int i = 0; i < num_subset; ++i) {
        Subset subset;
        file >> subset.material >> subset.start >> subset.count;
        subsets_.push_back(subset);
    }

    // インデックスバッファの読み込み
    std::vector<unsigned int> indices;
    UINT num_index;
    file >> num_index;
    for (UINT i = 0; i < num_index; ++i) {
        unsigned int index;
        file >> index;
        indices.push_back(index);
    }

    // 頂点データの読み込み
    std::vector<Vertex> vertices;
    int num_vertex;
    file >> num_vertex;
    for (int i = 0; i < num_vertex; ++i) {
        Vertex vertex;
        file >> vertex.position.x >> vertex.position.y >> vertex.position.z;
        file >> vertex.normal.x >> vertex.normal.y >> vertex.normal.z;
        file >> vertex.texcoord.x >> vertex.texcoord.y;
        for (int j = 0; j < 4; ++j) {
            unsigned int bone_index;
            file >> bone_index;
            vertex.bone_indices[j] = (unsigned char)bone_index;
        }
        for (int j = 0; j < 4; ++j) {
            file >> vertex.bone_weights[j];
        }
        vertices.push_back(vertex);
    }

    // 頂点バッファの作成
    vertices_ = device_->create_vertex_buffer(vertices.data(),
        (UINT)(sizeof(Vertex) * vertices.size()), (UINT)sizeof(Vertex));
    // インデックスバッファの作成
    indices_ = device_->create_index_buffer(indices.data(),
        (UINT)(sizeof(unsigned int) * indices.size()), DXGI_FORMAT_R32_UINT);
}
// 描画
void Mesh::draw(unsigned int instance_count, unsigned int instance_num_offset,unsigned int start_vartex) const {
    // 頂点バッファとインデックスバッファの設定
    device_->set_vertex_buffer(0, vertices_);
    device_->set_index_buffer(indices_);
    // サブセットごとに描画
    for (const auto& subset : subsets_) {
        // マテリアル用コンスタントバッファの設定
        device_->set_constant_buffer(3, materials_[subset.material].constant_buffer);
        // マテリアル用テクスチャの設定
        device_->set_texture(0, materials_[subset.material].texture);
        // ポリゴンの描画
        device_->draw_indexed(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST, subset.count, subset.start,start_vartex,instance_count,instance_num_offset);
    }
}
// 解放
void Mesh::clear() {
    // マテリアルの削除
    for (const auto& mat : materials_) {
        delete mat.constant_buffer;
        delete mat.texture;
    }
    materials_.clear();
    // サブセットの消去
    subsets_.clear();
    // バッファの削除
    delete vertices_;
    vertices_ = nullptr;
    delete indices_;
    indices_ = nullptr;
}