#include "Model.h"
#include"Object3d.h"
#include <wrl.h>
#include <cassert>
#include <DirectXMath.h>
#include <vector>

#include <d3dcompiler.h>
#include <DirectXTex.h>
#include <fstream>
#include <sstream>
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;


Model::Material Model::material;
ComPtr<ID3D12Resource> Model::vertBuff;
std::vector<Model::VertexPosNormalUv> Model::vertices;
std::vector<unsigned short> Model::indices;

void Model::Initialize()
{

}

void Model::LoadMaterial(const std::string& directoryPath, const std::string& filename)
{
	// �t�@�C���X�g���[��
	std::ifstream file;
	// �}�e���A���t�@�C�����J��
	file.open(directoryPath + filename);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	// 1�s���ǂݍ���
	string line;
	while (getline(file, line)) {

		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		// �擪�̃^�u�����͖�������
		if (key[0] == '\t') {
			key.erase(key.begin()); // �擪�̕������폜
		}

		// �擪������newmtl�Ȃ�}�e���A����
		if (key == "newmtl") {
			// �}�e���A�����ǂݍ���
			line_stream >> material.name;
		}
		// �擪������Ka�Ȃ�A���r�G���g�F
		if (key == "Ka") {
			line_stream >> material.ambient.x;
			line_stream >> material.ambient.y;
			line_stream >> material.ambient.z;
		}
		// �擪������Kd�Ȃ�f�B�t���[�Y�F
		if (key == "Kd") {
			line_stream >> material.diffuse.x;
			line_stream >> material.diffuse.y;
			line_stream >> material.diffuse.z;
		}
		// �擪������Ks�Ȃ�X�y�L�����[�F
		if (key == "Ks") {
			line_stream >> material.specular.x;
			line_stream >> material.specular.y;
			line_stream >> material.specular.z;
		}
		// �擪������map_Kd�Ȃ�e�N�X�`���t�@�C����
		if (key == "map_Kd") {
			// �e�N�X�`���̃t�@�C�����ǂݍ���
			line_stream >> material.textureFilename;
			// �e�N�X�`���ǂݍ���
			LoadTexture(directoryPath, material.textureFilename);
		}
	}
	// �t�@�C�������
	file.close();
}

void Model::CreateModel()
{
	const string modelname = "triangle_mat";
	const string filename = modelname + ".obj";
	const string directoryPath = "Resources/" + modelname + "/";

	HRESULT result;
	// �t�@�C���X�g���[��
	std::ifstream file;
	// .obj�t�@�C�����J��
	file.open(directoryPath + filename);
	// �t�@�C���I�[�v�����s���`�F�b�N
	if (file.fail()) {
		assert(0);
	}

	vector<XMFLOAT3> positions;	// ���_���W
	vector<XMFLOAT3> normals;	// �@���x�N�g��
	vector<XMFLOAT2> texcoords;	// �e�N�X�`��UV
	// 1�s���ǂݍ���
	string line;
	while (getline(file, line)) {

		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);

		// ���p�X�y�[�X��؂�ōs�̐擪��������擾
		string key;
		getline(line_stream, key, ' ');

		// �擪������v�Ȃ璸�_���W
		if (key == "v") {
			// X,Y,Z���W�ǂݍ���
			XMFLOAT3 position{};
			line_stream >> position.x;
			line_stream >> position.y;
			line_stream >> position.z;
			positions.emplace_back(position);

			VertexPosNormalUv vertex{};
			vertex.pos = position;
			vertices.emplace_back(vertex);
		}
		// �擪������vt�Ȃ�e�N�X�`��
		if (key == "vt")
		{
			// U,V�����ǂݍ���
			XMFLOAT2 texcoord{};
			line_stream >> texcoord.x;
			line_stream >> texcoord.y;
			// V�������]
			texcoord.y = 1.0f - texcoord.y;
			// �e�N�X�`�����W�f�[�^�ɒǉ�
			texcoords.emplace_back(texcoord);
		}
		// �擪������vn�Ȃ�@���x�N�g��
		if (key == "vn") {
			// X,Y,Z�����ǂݍ���
			XMFLOAT3 normal{};
			line_stream >> normal.x;
			line_stream >> normal.y;
			line_stream >> normal.z;
			// �@���x�N�g���f�[�^�ɒǉ�
			normals.emplace_back(normal);
		}
		// �擪������f�Ȃ�|���S���i�O�p�`�j
		if (key == "f")
		{
			// ���p�X�y�[�X��؂�ōs�̑�����ǂݍ���
			string index_string;
			while (getline(line_stream, index_string, ' ')) {
				// ���_�C���f�b�N�X1���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
				std::istringstream index_stream(index_string);
				unsigned short indexPosition, indexNormal, indexTexcoord;
				index_stream >> indexPosition;
				index_stream.seekg(1, ios_base::cur); // �X���b�V�����΂�
				index_stream >> indexTexcoord;
				index_stream.seekg(1, ios_base::cur); // �X���b�V�����΂�
				index_stream >> indexNormal;
				// ���_�f�[�^�̒ǉ�
				VertexPosNormalUv vertex{};
				vertex.pos = positions[indexPosition - 1];
				vertex.normal = normals[indexNormal - 1];
				vertex.uv = texcoords[indexTexcoord - 1];
				vertices.emplace_back(vertex);
				// �C���f�b�N�X�f�[�^�̒ǉ�
				indices.emplace_back((unsigned short)indices.size());
			}
		}
		//�}�e���A��
		if (key == "mtllib")
		{
			// �}�e���A���̃t�@�C�����ǂݍ���
			string filename;
			line_stream >> filename;
			// �}�e���A���ǂݍ���
			LoadMaterial(directoryPath, filename);
		}
	}
	file.close();

	UINT sizeVB = static_cast<UINT>(sizeof(VertexPosNormalUv) * vertices.size());

	CD3DX12_HEAP_PROPERTIES A1(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC A2(CD3DX12_RESOURCE_DESC::Buffer(sizeVB));

	// ���_�o�b�t�@����
	result = Object3ddevice->CreateCommittedResource(
		&A1,
		D3D12_HEAP_FLAG_NONE,
		&A2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	UINT sizeIB = static_cast<UINT>(sizeof(unsigned short) * indices.size());

	CD3DX12_HEAP_PROPERTIES B1(D3D12_HEAP_TYPE_UPLOAD);
	CD3DX12_RESOURCE_DESC B2 = CD3DX12_RESOURCE_DESC::Buffer(sizeIB);

	// �C���f�b�N�X�o�b�t�@����
	result = device->CreateCommittedResource(
		&B1,
		D3D12_HEAP_FLAG_NONE,
		&B2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));
	if (FAILED(result)) {
		assert(0);
		return;
	}

	// ���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosNormalUv* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	if (SUCCEEDED(result)) {
		std::copy(vertices.begin(), vertices.end(), vertMap);
		vertBuff->Unmap(0, nullptr);
	}

	// �C���f�b�N�X�o�b�t�@�ւ̃f�[�^�]��
	unsigned short* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	if (SUCCEEDED(result)) {
		std::copy(indices.begin(), indices.end(), indexMap);
		indexBuff->Unmap(0, nullptr);
	}

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(vertices[0]);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;
}
