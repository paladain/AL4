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
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; // �@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};

	// �}�e���A��
	struct Material
	{
		std::string name;	// �}�e���A����
		XMFLOAT3 ambient;	// �A���r�G���g�e���x
		XMFLOAT3 diffuse;	// �f�B�t���[�Y�e���x
		XMFLOAT3 specular;	// �X�y�L�����[�e���x
		float alpha;		// �A���t�@
		std::string textureFilename;	// �e�N�X�`���t�@�C����
		// �R���X�g���N�^
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
	// �f�o�C�X
	//static ID3D12Device* device;
	// �}�e���A��
	static Material material;
	// ���_�o�b�t�@
	static ComPtr<ID3D12Resource> vertBuff;
	// ���_�f�[�^�z��
	static std::vector<VertexPosNormalUv> vertices;
	// ���_�C���f�b�N�X�z��
	static std::vector<unsigned short> indices;
};

