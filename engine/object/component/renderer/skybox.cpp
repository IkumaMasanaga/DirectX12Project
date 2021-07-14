#include "../../../graphics/graphics_manager.h"
#include "../../../graphics/pipeline_state.h"
#include "../../../graphics/shader.h"
#include "../../../graphics/descriptor_manager.h"
#include "../../../graphics/texture.h"
#include "skybox.h"
#include "../camera.h"
#include "../../mesh.h"
#include "../../material.h"


using namespace Microsoft::WRL;

namespace eng {

	bool Skybox::onCreated() {

		Shape::CreateDesc desc;
		desc.slices = 1;
		desc.stacks = 1;

		meshs_.resize(6);

		const float SIZE_H = 5.0f - 0.01f;

		const std::string REGIST_NAME[6] = {
			"engine_skybox_yz",
			"engine_skybox_yz",
			"engine_skybox_zx",
			"engine_skybox_zx",
			"engine_skybox_xy",
			"engine_skybox_xy",
		};

		const std::function<Shape::s_ptr(const Shape::CreateDesc&)> SHAPE_CREATE_FUNC[6] = {
			Shape::createPlaneYZ,
			Shape::createPlaneYZ,
			Shape::createPlaneZX,
			Shape::createPlaneZX,
			Shape::createPlaneXY,
			Shape::createPlaneXY,
		};

		const lib::Vector3 OFFSET_POSITION[6] = {
			lib::Vector3(SIZE_H, 0.0f, 0.0f),
			lib::Vector3(-SIZE_H, 0.0f, 0.0f),
			lib::Vector3(0.0f, SIZE_H, 0.0f),
			lib::Vector3(0.0f, -SIZE_H, 0.0f),
			lib::Vector3(0.0f, 0.0f, SIZE_H),
			lib::Vector3(0.0f, 0.0f, -SIZE_H),
		};

		const lib::Vector3 ROTATION_AXIS[6] = {
			lib::Vector3::UP,
			lib::Vector3::UP,
			lib::Vector3::RIGHT,
			lib::Vector3::RIGHT,
			lib::Vector3::UP,
			lib::Vector3::UP,
		};

		const float ROTATION_ANGLE[6] = {
			180.0f,
			0.0f,
			180.0f,
			0.0f,
			180.0f,
			0.0f,
		};

		for (int i = 0; i < 6; ++i) {
			desc.regist_name = REGIST_NAME[i];
			meshs_[i] = Mesh::createFromShape(SHAPE_CREATE_FUNC[i](desc));
			meshs_[i]->transform_->local_position_ = OFFSET_POSITION[i];
			meshs_[i]->transform_->local_rotation_ = lib::Quaternion::createRotationAxis(ROTATION_AXIS[i], lib::Math::toRadian(ROTATION_ANGLE[i]));
		}

		return true;
	}

	void Skybox::render(const std::shared_ptr<Camera>& camera) {

		GraphicsManager& mgr = GraphicsManager::getInstance();
		ComPtr<ID3D12GraphicsCommandList> com_list = mgr.getCommandList();

		// カメラの設定
		lib::Matrix4x4 view_projection = camera->getViewMatrix4x4() * camera->getProjectionMatrix4x4();

		// 中心から頂点までの距離がカメラのファー以内に収まるように調節
		// far / Boxのサイズ / ルート3 * 2
		float scale = camera->far_ / 10.0f / 1.7320508f * 2.0f;

		lib::Vector3 cp = camera->getTransform()->getPosition();

		// カメラの中心行列
		// 回転はさせない
		lib::Matrix4x4 camera_world = lib::Matrix4x4(
			scale, 0.0f, 0.0f, 0.0f,
			0.0f, scale, 0.0f, 0.0f,
			0.0f, 0.0f, scale, 0.0f,
			cp.x, cp.y, cp.z, 1.0f
		);

		std::vector<Mesh::s_ptr>::iterator it = meshs_.begin();
		while (it != meshs_.end()) {
			if (!(*it)->isAlive()) {
				it = meshs_.erase(it);
				continue;
			}
			if (!(*it)->isActive()) {
				++it;
				continue;
			}

			// ルートシグネチャとPSOの設定
			com_list->SetGraphicsRootSignature((*it)->pso_->getShader()->getRootSignature().Get());
			com_list->SetPipelineState((*it)->pso_->getObject().Get());

			// テクスチャをシェーダのレジスタにセット
			// テクスチャが設定されていない場合はデフォルトを設定するように変更する
			com_list->SetDescriptorHeaps(1, mgr.getSrvHeap()->getHeap().GetAddressOf());
			com_list->SetGraphicsRootDescriptorTable(1, (*it)->material_->tex_diffuse_->getHandle().getGpuHandle());

			//--------------------------------------------------

			// ワールド行列
			lib::Matrix4x4 world = (*it)->transform_->getWorldMatrix4x4() * camera_world;

			//全ての変換行列
			lib::Matrix4x4 mvp = lib::Matrix4x4::createTranspose(world * view_projection);

			// ワールド行列をシェーダの定数バッファにセット
			lib::Matrix4x4* buffer{};
			if (FAILED((*it)->cbv_->Map(0, nullptr, (void**)&buffer))) return;

			//定数バッファをシェーダのレジスタにセット
			com_list->SetGraphicsRootConstantBufferView(0, (*it)->cbv_->GetGPUVirtualAddress());

			// 行列を定数バッファに書き込み
			buffer[0] = mvp;
			buffer[1] = world;

			(*it)->cbv_->Unmap(0, nullptr);

			//--------------------------------------------------

			// インデックスを使用し、トライアングルリストを描画
			D3D12_VERTEX_BUFFER_VIEW vertex_view = (*it)->shape_->getVertexView();
			D3D12_INDEX_BUFFER_VIEW index_view = (*it)->shape_->getIndexView();

			com_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			com_list->IASetVertexBuffers(0, 1, &vertex_view);
			com_list->IASetIndexBuffer(&index_view);

			// 描画
			com_list->DrawIndexedInstanced((*it)->shape_->getIndexNum(), 1, 0, 0, 0);

			++it;
		}

	}

	void Skybox::setTextures(const Textures& textures) {
		meshs_[0]->material_->tex_diffuse_ = textures.right;
		meshs_[1]->material_->tex_diffuse_ = textures.left;
		meshs_[2]->material_->tex_diffuse_ = textures.up;
		meshs_[3]->material_->tex_diffuse_ = textures.down;
		meshs_[4]->material_->tex_diffuse_ = textures.forward;
		meshs_[5]->material_->tex_diffuse_ = textures.back;
	}

}
