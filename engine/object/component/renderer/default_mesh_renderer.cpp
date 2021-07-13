#include "../../../graphics/graphics_manager.h"
#include "../../../graphics/descriptor_manager.h"
#include "../../../graphics/pipeline_state.h"
#include "../../../graphics/shader.h"
#include "../../../graphics/texture.h"
#include "default_mesh_renderer.h"
#include "../camera.h"
#include "../../mesh.h"
#include "../../material.h"


using namespace Microsoft::WRL;

namespace eng {

	void DefaultMeshRenderer::render(const std::shared_ptr<Camera>& camera) {

		GraphicsManager& mgr = GraphicsManager::getInstance();
		ComPtr<ID3D12GraphicsCommandList> com_list = mgr.getCommandList();

		// カメラの設定
		lib::Matrix4x4 view_projection = camera->getViewMatrix4x4() * camera->getProjectionMatrix4x4();

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
			lib::Matrix4x4 world = (*it)->transform_->getWorldMatrix4x4() * getGameObject()->transform_->getWorldMatrix4x4();

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

}
