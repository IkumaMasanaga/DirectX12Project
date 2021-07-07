#include "sample_scene.h"

#include "../component/sample_component.h"


bool SampleScene::initialize() {

	eng::Shape::CreateDesc desc = {};
	eng::GameObject::s_ptr obj1 = nullptr;

	const float OFFSET_1 = 8.0f;
	const float OFFSET_2 = 24.0f;

	obj1 = eng::GameObject::createPlaneXY("plane_xy", desc);
	obj1->transform_->local_position_.set(-OFFSET_2, 0.0f, OFFSET_2);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createPlaneYZ("plane_yz", desc);
	obj1->transform_->local_position_.set(-OFFSET_1, 0.0f, OFFSET_2);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createPlaneZX("plane_zx", desc);
	obj1->transform_->local_position_.set(OFFSET_1, 0.0f, OFFSET_2);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createCube("cube", desc);
	obj1->transform_->local_position_.set(OFFSET_2, 0.0f, OFFSET_2);
	obj1->addComponent<SampleComponent>();


	obj1 = eng::GameObject::createTriangleIsosceles("triangle_isosceles", desc);
	obj1->transform_->local_position_.set(-OFFSET_2, 0.0f, OFFSET_1);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createTriangleRight("triangle_right", desc);
	obj1->transform_->local_position_.set(-OFFSET_1, 0.0f, OFFSET_1);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createTriangleLeft("triangle_left", desc);
	obj1->transform_->local_position_.set(OFFSET_1, 0.0f, OFFSET_1);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createTriangleEquilateral("triangle_equilateral", desc);
	obj1->transform_->local_position_.set(OFFSET_2, 0.0f, OFFSET_1);
	obj1->addComponent<SampleComponent>();


	obj1 = eng::GameObject::createSphere("sphere", desc);
	obj1->transform_->local_position_.set(-OFFSET_2, 0.0f, -OFFSET_1);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createDome("dome", desc);
	obj1->transform_->local_position_.set(-OFFSET_1, -5.0f, -OFFSET_1);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createCone("cone", desc);
	obj1->transform_->local_position_.set(OFFSET_1, -5.0f, -OFFSET_1);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createDisk("disk", desc);
	obj1->transform_->local_position_.set(OFFSET_2, 0.0f, -OFFSET_1);
	obj1->addComponent<SampleComponent>();


	obj1 = eng::GameObject::createDiskRing("disk_ring", desc);
	obj1->transform_->local_position_.set(-OFFSET_2, 0.0f, -OFFSET_2);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createHollowOutDiskPlane("hollow_out_disk_plane", desc);
	obj1->transform_->local_position_.set(-OFFSET_1, 0.0f, -OFFSET_2);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createCylinder("cylinder", desc);
	obj1->transform_->local_position_.set(OFFSET_1, 0.0f, -OFFSET_2);
	obj1->addComponent<SampleComponent>();


	camera_->getTransform()->local_position_.set(0.0f, 30.0f, -40.0f);
	camera_->getTransform()->rotate(lib::Vector3::RIGHT, lib::Math::toRadian(45.0f));

	return true;
}

void SampleScene::lateUpdate() {



}

void SampleScene::finalize() {



}
