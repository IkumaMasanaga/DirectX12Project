#include "../../../graphics/graphics_manager.h"
#include "../../../graphics/descriptor_manager.h"
#include "../../../graphics/pipeline_state.h"
#include "../../../graphics/shader.h"
#include "../../../graphics/texture.h"
#include "mesh_renderer.h"
#include "../camera.h"
#include "../../mesh.h"
#include "../../material.h"


using namespace Microsoft::WRL;

namespace eng {

	void MeshRenderer::render(std::shared_ptr<Camera> camera) {

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
			com_list->SetDescriptorHeaps(1, mgr.getSrvHeap()->getHeap().GetAddressOf());	// ここじゃなくていい？
			com_list->SetGraphicsRootDescriptorTable(1, (*it)->material_->tex_diffuse_->getHandle().getGpuHandle());


			//--------------------------------------------------

			// ワールド行列
			lib::Matrix4x4 m = (*it)->transform_->getWorldMatrix4x4() * getGameObject()->transform_->getWorldMatrix4x4();

			// 定数バッファに設定
			// 1.定数バッファ
			// 2.ワールド行列
			// 3.ビュープロジェクション行列
			(*it)->pso_->getShader()->setting_func_((*it)->cbv_.Get(), &m, &view_projection);

			//--------------------------------------------------

			// インデックスを使用し、トライアングルリストを描画
			D3D12_VERTEX_BUFFER_VIEW vertex_view{};
			vertex_view.BufferLocation = (*it)->shape_->getVertexBuffer()->GetGPUVirtualAddress();
			vertex_view.StrideInBytes = sizeof(Vertex3D);
			vertex_view.SizeInBytes = sizeof(Vertex3D) * (*it)->shape_->getVertexNum();

			D3D12_INDEX_BUFFER_VIEW index_view{};
			index_view.BufferLocation = (*it)->shape_->getIndexBuffer()->GetGPUVirtualAddress();
			index_view.SizeInBytes = sizeof(uint32_t) * (*it)->shape_->getIndexNum();
			index_view.Format = DXGI_FORMAT_R32_UINT;

			com_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			com_list->IASetVertexBuffers(0, 1, &vertex_view);
			com_list->IASetIndexBuffer(&index_view);


			// 描画
			com_list->DrawIndexedInstanced((*it)->shape_->getIndexNum(), 1, 0, 0, 0);

			++it;
		}

	}

}
