#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Model
{
public:
	struct VertexPosNormalUv
	{
		XMFLOAT3 pos; // xyz座標
		XMFLOAT3 normal; // 法線ベクトル
		XMFLOAT2 uv;  // uv座標
	};

	// マテリアル
	struct Material
	{
		std::string name;	// マテリアル名
		XMFLOAT3 ambient;	// アンビエント影響度
		XMFLOAT3 diffuse;	// ディフューズ影響度
		XMFLOAT3 specular;	// スペキュラー影響度
		float alpha;		// アルファ
		std::string textureFilename;	// テクスチャファイル名
		// コンストラクタ
		Material() {
			ambient = { 0.3f, 0.3f, 0.3f };
			diffuse = { 0.0f, 0.0f, 0.0f };
			specular = { 0.0f, 0.0f, 0.0f };
			alpha = 1.0f;
		}
	};

	static void Initialize();

	void LoadMaterial(const std::string& directoryPath, const std::string& filename);

	void CreateModel();

private:
	// デバイス
	//static ID3D12Device* device;
	// マテリアル
	static Material material;
	// 頂点バッファ
	static ComPtr<ID3D12Resource> vertBuff;
	// 頂点データ配列
	static std::vector<VertexPosNormalUv> vertices;
	// 頂点インデックス配列
	static std::vector<unsigned short> indices;
};

