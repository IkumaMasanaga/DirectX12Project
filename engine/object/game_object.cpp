#include "../engine.h"


namespace eng {
	
	bool GameObject::constructor() {
		Engine::getInstance().getNowScene()->game_objects_.emplace_back(shared_from_this<GameObject>());
		return true;
	}

	void GameObject::onDestroy() {

		for (const auto& com : components_) {
			com->onDestroy();
		}

	}

	void GameObject::onEnable() {

		for (const auto& com : components_) {
			if (!com->isActive()) continue;
			com->onEnable();
		}

	}
	
	void GameObject::onDisable() {

		for (const auto& com : components_) {
			if (!com->isActive()) continue;
			com->onDisable();
		}

	}

	void GameObject::update() {

		std::list<std::shared_ptr<Component>>::iterator it = components_.begin();
		while (it != components_.end()) {
			if (!(*it)->isAlive()) {
				(*it)->onDestroy();
				it = components_.erase(it);
				continue;
			}
			if ((*it)->isActive()) {
				if (!(*it)->is_enable_) {
					(*it)->onEnable();
					(*it)->is_enable_ = true;
				}
				if ((*it)->is_start_) {
					(*it)->start();
					(*it)->is_start_ = false;
				}
				(*it)->update();
			}
			else {
				if ((*it)->is_enable_) {
					(*it)->onDisable();
					(*it)->is_enable_ = false;
				}
			}
			++it;
		}

	}

	void GameObject::render(std::shared_ptr<Camera> camera) {

		std::list<std::shared_ptr<Renderer>>::iterator it = renderers_.begin();
		while (it != renderers_.end()) {
			if (!(*it)->isAlive()) {
				it = renderers_.erase(it);
				continue;
			}
			if ((*it)->isActive()) (*it)->render(camera);
			++it;
		}

	}

	bool GameObject::isActiveParent() {
		if (!isActive()) return false;
		Transform<GameObject>::s_ptr parent = transform_->getParent();
		if (!parent) return true;
		GameObject::s_ptr parent_obj = parent->getIncluded();
		return parent_obj->isActiveParent();
	}

	void GameObject::destroy() {
		Object::destroy();
		for (const auto& com : components_) {
			com->destroy();
		}
		transform_->roundup([](Transform<GameObject>::s_ptr ptr) {
			GameObject::s_ptr child = ptr->getIncluded();
			child->Object::destroy();
			for (const auto& child_com : child->components_) {
				child_com->destroy();
			}
		});
	}

	GameObject::s_ptr GameObject::createEmpty(const std::string& name) {
		GameObject::s_ptr ptr = GameObject::createShared<GameObject>();
		ptr->setName(name);
		ptr->transform_ = Transform<GameObject>::create(ptr);
		return ptr;
	}

	GameObject::s_ptr GameObject::createPlaneXY(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(1);
		renderer->meshs_[0] = Mesh::createFromShape(Shape::createPlaneXY(desc), texture_file_path);
		return ptr;
	}

	GameObject::s_ptr GameObject::createPlaneYZ(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(1);
		renderer->meshs_[0] = Mesh::createFromShape(Shape::createPlaneYZ(desc), texture_file_path);
		return ptr;
	}

	GameObject::s_ptr GameObject::createPlaneZX(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(1);
		renderer->meshs_[0] = Mesh::createFromShape(Shape::createPlaneZX(desc), texture_file_path);
		return ptr;
	}
	
	GameObject::s_ptr GameObject::createCube(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(6);

		std::function<Shape::s_ptr(const Shape::CreateDesc&)> create_func[6] = {
			Shape::createPlaneXY,
			Shape::createPlaneXY,
			Shape::createPlaneYZ,
			Shape::createPlaneYZ,
			Shape::createPlaneZX,
			Shape::createPlaneZX,
		};

		for (int i = 0; i < 6; ++i) {
			Shape::CreateDesc d = desc;
			if (!desc.regist_name.empty()) d.regist_name += "_" + std::to_string(i);
			renderer->meshs_[i] = Mesh::createFromShape(create_func[i](d), texture_file_path);
		}

		float w2 = (desc.width / 2) - FLT_EPSILON;
		float h2 = (desc.height / 2) - FLT_EPSILON;
		float d2 = (desc.depth / 2) - FLT_EPSILON;
		lib::Vector3 pos_ofs[6] = {
			{   0, 0, d2 }, { 0,  0, -d2 }, { w2,   0, 0 },
			{ -w2, 0,  0 }, { 0, h2,   0 }, {  0, -h2, 0 }
		};
		lib::Vector3 rot_axis[6] = {
			{ 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 },
			{ 0, 1, 0 }, { 1, 0, 0 }, { 1, 0, 0 }
		};
		float rot_angles[6] = {
			  0, 180,   0,
			180,   0, 180
		};

		for (int i = 0; i < 6; ++i) {
			renderer->meshs_[i]->transform_->local_position_ = pos_ofs[i];
			renderer->meshs_[i]->transform_->local_rotation_ = lib::Quaternion::createRotationAxis(rot_axis[i], lib::Math::toRadian(rot_angles[i]));
		}

		return ptr;
	}

	GameObject::s_ptr GameObject::createInnerCube(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(6);

		std::function<Shape::s_ptr(const Shape::CreateDesc&)> create_func[6] = {
			Shape::createPlaneXY,
			Shape::createPlaneXY,
			Shape::createPlaneYZ,
			Shape::createPlaneYZ,
			Shape::createPlaneZX,
			Shape::createPlaneZX,
		};

		for (int i = 0; i < 6; ++i) {
			Shape::CreateDesc d = desc;
			if (!desc.regist_name.empty()) d.regist_name += "_" + std::to_string(i);
			renderer->meshs_[i] = Mesh::createFromShape(create_func[i](d), texture_file_path);
		}

		float w2 = (desc.width / 2) - FLT_EPSILON;
		float h2 = (desc.height / 2) - FLT_EPSILON;
		float d2 = (desc.depth / 2) - FLT_EPSILON;
		lib::Vector3 pos_ofs[6] = {
			{   0, 0, -d2 }, { 0,  0, d2 }, { -w2,   0, 0 },
			{ w2, 0,  0 }, { 0, -h2,   0 }, {  0, h2, 0 }
		};
		lib::Vector3 rot_axis[6] = {
			{ 0, 1, 0 }, { 0, 1, 0 }, { 0, 1, 0 },
			{ 0, 1, 0 }, { 1, 0, 0 }, { 1, 0, 0 }
		};
		float rot_angles[6] = {
			  0, 180,   0,
			180,   0, 180
		};

		for (int i = 0; i < 6; ++i) {
			renderer->meshs_[i]->transform_->local_position_ = pos_ofs[i];
			renderer->meshs_[i]->transform_->local_rotation_ = lib::Quaternion::createRotationAxis(rot_axis[i], lib::Math::toRadian(rot_angles[i]));
		}

		return ptr;
	}

	GameObject::s_ptr GameObject::createTriangleIsosceles(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(1);
		renderer->meshs_[0] = Mesh::createFromShape(Shape::createTriangleIsosceles(desc), texture_file_path);
		return ptr;
	}

	GameObject::s_ptr GameObject::createTriangleRight(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(1);
		renderer->meshs_[0] = Mesh::createFromShape(Shape::createTriangleRight(desc), texture_file_path);
		return ptr;
	}

	GameObject::s_ptr GameObject::createTriangleLeft(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(1);
		renderer->meshs_[0] = Mesh::createFromShape(Shape::createTriangleLeft(desc), texture_file_path);
		return ptr;
	}

	GameObject::s_ptr GameObject::createTriangleEquilateral(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(1);
		renderer->meshs_[0] = Mesh::createFromShape(Shape::createTriangleEquilateral(desc), texture_file_path);
		return ptr;
	}

	GameObject::s_ptr GameObject::createSphere(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(1);
		renderer->meshs_[0] = Mesh::createFromShape(Shape::createSphere(desc), texture_file_path);
		return ptr;
	}

	GameObject::s_ptr GameObject::createDome(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(1);
		renderer->meshs_[0] = Mesh::createFromShape(Shape::createDome(desc), texture_file_path);
		return ptr;
	}
	
	GameObject::s_ptr GameObject::createCone(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(1);
		renderer->meshs_[0] = Mesh::createFromShape(Shape::createCone(desc), texture_file_path);
		return ptr;
	}
	
	GameObject::s_ptr GameObject::createDisk(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(1);
		renderer->meshs_[0] = Mesh::createFromShape(Shape::createDisk(desc), texture_file_path);
		return ptr;
	}
	
	GameObject::s_ptr GameObject::createDiskRing(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(1);
		renderer->meshs_[0] = Mesh::createFromShape(Shape::createDiskRing(desc), texture_file_path);
		return ptr;
	}
	
	GameObject::s_ptr GameObject::createCylinder(const std::string& name, const Shape::CreateDesc& desc, const std::string& texture_file_path) {
		GameObject::s_ptr ptr = GameObject::createEmpty(name);
		DefaultMeshRenderer::s_ptr renderer = ptr->addComponent<DefaultMeshRenderer>();
		renderer->meshs_.resize(1);
		renderer->meshs_[0] = Mesh::createFromShape(Shape::createCylinder(desc), texture_file_path);
		return ptr;
	}
	
}
