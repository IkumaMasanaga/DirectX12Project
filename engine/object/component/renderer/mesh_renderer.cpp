#include "../../../graphics_manager.h"
#include "mesh_renderer.h"
#include "../camera.h"
#include "../../mesh.h"
#include "../../pipeline_state.h"
#include "../../shader.h"
#include "../../material.h"
#include "../../texture.h"


using namespace Microsoft::WRL;

namespace eng {

	void MeshRenderer::render(std::shared_ptr<Camera> camera) {

		GraphicsManager& mgr = GraphicsManager::getInstance();

		//カメラの設定
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

			//ルートシグネチャとPSOの設定
			mgr.command_list_->SetGraphicsRootSignature((*it)->pso_->shader_->root_signature_.Get());
			mgr.command_list_->SetPipelineState((*it)->pso_->pso_.Get());

			//テクスチャをシェーダのレジスタにセット
			mgr.command_list_->SetDescriptorHeaps(1, (*it)->material_->tex_diffuse_->descriptor_heap_.GetAddressOf());
			mgr.command_list_->SetGraphicsRootDescriptorTable(1, (*it)->material_->tex_diffuse_->descriptor_heap_->GetGPUDescriptorHandleForHeapStart());


			//--------------------------------------------------

			//ワールド行列
			lib::Matrix4x4 m = (*it)->transform_->getWorldMatrix4x4() * getGameObject()->transform_->getWorldMatrix4x4();

			// 定数バッファに設定
			// 1.定数バッファ
			// 2.ワールド行列
			// 3.ビュープロジェクション行列
			(*it)->pso_->shader_->setting_func_((*it)->cbv_.Get(), &m, &view_projection);

			//--------------------------------------------------

			//インデックスを使用し、トライアングルリストを描画
			D3D12_VERTEX_BUFFER_VIEW vertex_view{};
			vertex_view.BufferLocation = (*it)->shape_->vbo_->GetGPUVirtualAddress();
			vertex_view.StrideInBytes = sizeof(Vertex3D);
			vertex_view.SizeInBytes = sizeof(Vertex3D) * (*it)->shape_->vertex_num_;

			D3D12_INDEX_BUFFER_VIEW index_view{};
			index_view.BufferLocation = (*it)->shape_->ibo_->GetGPUVirtualAddress();
			index_view.SizeInBytes = sizeof(uint32_t) * (*it)->shape_->index_num_;
			index_view.Format = DXGI_FORMAT_R32_UINT;

			mgr.command_list_->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			mgr.command_list_->IASetVertexBuffers(0, 1, &vertex_view);
			mgr.command_list_->IASetIndexBuffer(&index_view);


			//描画
			mgr.command_list_->DrawIndexedInstanced((*it)->shape_->index_num_, 1, 0, 0, 0);

			++it;
		}

	}

}
