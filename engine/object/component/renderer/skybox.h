#pragma once
#include "renderer.h"


namespace eng {

	class Texture;
	class Mesh;

	class Skybox final : public Renderer {
	public:
		using s_ptr = std::shared_ptr<Skybox>;
		using w_ptr = std::weak_ptr<Skybox>;

		struct SetDesc {
			std::shared_ptr<Camera> camera = nullptr;
			std::shared_ptr<Texture> texture_right = nullptr;
			std::shared_ptr<Texture> texture_left = nullptr;
			std::shared_ptr<Texture> texture_up = nullptr;
			std::shared_ptr<Texture> texture_down = nullptr;
			std::shared_ptr<Texture> texture_forward = nullptr;
			std::shared_ptr<Texture> texture_back = nullptr;
		};

	private:
		std::weak_ptr<Camera> camera_;
		std::vector<std::shared_ptr<Mesh>> meshs_;
		float scale_ = 1.0f;

		bool onCreated() final override;

		void update() final override;

		void render(const std::shared_ptr<Camera>& camera) final override;

		void calcScale();

		inline std::shared_ptr<Camera> getCamera() { return (camera_.expired()) ? nullptr : camera_.lock(); }

	public:
		Skybox() {}
		~Skybox() {}

		void set(const SetDesc& desc);

	};

}
