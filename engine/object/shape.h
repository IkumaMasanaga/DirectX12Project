#pragma once
#include <wrl.h>
#include <d3d12.h>
#include "object.h"


namespace eng {

	class Vertex3D {
	public:
		lib::Vector3 position;
		lib::Vector3 normal;
		lib::Vector2 uv;
	};

	class Shape final : public Object {
		friend class lib::SharedFlyweightMap<std::string, Shape>;
	public:
		using s_ptr = std::shared_ptr<Shape>;
		using w_ptr = std::weak_ptr<Shape>;

		struct CreateDesc {
			std::string regist_name_ = "";
			float width_ = 10.0f;
			float height_ = 10.0f;
			float depth_ = 10.0f;
			int stacks_ = 10;
			int slices_ = 10;
		};

	private:
		//====================================================================================================
		// static�ϐ�

		inline static lib::SharedFlyweightMap<std::string, Shape> regist_map_;	// �o�^�}�b�v

		//====================================================================================================
		// �����o�֐�

		// ���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�̍쐬
		bool createBuffers();

		// ���_�o�b�t�@�A�C���f�b�N�X�o�b�t�@�փR�s�[
		bool copyBuffers(Vertex3D* vertexs, uint32_t* indexs);

		//====================================================================================================
		// static�֐�

		// ���ɐ�������Ă��邩
		static bool isCreated(Shape::s_ptr& ret_obj, const std::string& regist_name);

		// SharedFlyweightMap�ł̐���
		static Shape::s_ptr loadOfFlyweight(void* none);

		//====================================================================================================
	public:
		Shape() {}
		~Shape() {}

		//====================================================================================================
		// �����o�ϐ�

		Microsoft::WRL::ComPtr<ID3D12Resource> vbo_;	// ���_�o�b�t�@
		Microsoft::WRL::ComPtr<ID3D12Resource> ibo_;	// �C���f�b�N�X�o�b�t�@
		int vertex_num_ = 0;	// ���_��
		int index_num_ = 0;		// �C���f�b�N�X��

		//====================================================================================================
		// static�֐�

		// �l�p�`�̐���(X,Y��)
		static Shape::s_ptr createPlaneXY(const CreateDesc& desc);
		// �l�p�`�̐���(Y,Z��)
		static Shape::s_ptr createPlaneYZ(const CreateDesc& desc);
		// �l�p�`�̐���(Z,X��)
		static Shape::s_ptr createPlaneZX(const CreateDesc& desc);

		//====================================================================================================
	};

}
