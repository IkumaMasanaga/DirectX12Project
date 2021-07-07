#include "sample_scene.h"

#include "../component/sample_component.h"


bool SampleScene::initialize() {

	eng::GameObject::s_ptr obj1 = eng::GameObject::createCube("obj1", eng::Shape::CreateDesc());
	obj1->transform_->local_position_ = lib::Vector3(0, 0, 0);
	obj1->transform_->local_scale_ = lib::Vector3(1.0f, 1.0f, 1.0f);
	obj1->addComponent<SampleComponent>();

	return true;
}

void SampleScene::lateUpdate() {



}

void SampleScene::finalize() {



}
