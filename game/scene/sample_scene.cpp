#include "sample_scene.h"

#include "../component/sample_component.h"


bool SampleScene::initialize() {

	camera_->getTransform()->local_position_.set(0.0f, 30.0f, -40.0f);
	camera_->getTransform()->rotate(lib::Vector3::RIGHT, lib::Math::toRadian(45.0f));
	//camera_->addComponent<SampleComponent>();

	// スカイボックスの作成
	{
		//eng::Skybox::Textures skybox_textures;
		//skybox_textures.right = eng::Texture::loadFromFile("resources/texture/skybox/right.bmp");		// 右
		//skybox_textures.left = eng::Texture::loadFromFile("resources/texture/skybox/left.bmp");			// 左
		//skybox_textures.up = eng::Texture::loadFromFile("resources/texture/skybox/up.bmp");				// 上
		//skybox_textures.down = eng::Texture::loadFromFile("resources/texture/skybox/down.bmp");			// 下
		//skybox_textures.forward = eng::Texture::loadFromFile("resources/texture/skybox/forward.bmp");	// 前
		//skybox_textures.back = eng::Texture::loadFromFile("resources/texture/skybox/back.bmp");			// 後ろ
		//camera_->addComponent<eng::Skybox>()->setTextures(skybox_textures);
	}

	eng::Shape::CreateDesc desc = {};
	eng::GameObject::s_ptr obj1 = nullptr;

	const float OFFSET_1 = 8.0f;
	const float OFFSET_2 = 24.0f;
	const std::string TEX_PATH = "resources/texture/test.bmp";
	//const std::string TEX_PATH = "";


	obj1 = eng::GameObject::createPlaneXY("plane_xy", desc, TEX_PATH);
	obj1->transform_->local_position_.set(-OFFSET_2, 0.0f, OFFSET_2);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createPlaneYZ("plane_yz", desc, TEX_PATH);
	obj1->transform_->local_position_.set(-OFFSET_1, 0.0f, OFFSET_2);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createPlaneZX("plane_zx", desc, TEX_PATH);
	obj1->transform_->local_position_.set(OFFSET_1, 0.0f, OFFSET_2);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createCube("cube", desc, TEX_PATH);
	obj1->transform_->local_position_.set(OFFSET_2, 0.0f, OFFSET_2);
	obj1->addComponent<SampleComponent>();


	obj1 = eng::GameObject::createTriangleIsosceles("triangle_isosceles", desc, TEX_PATH);
	obj1->transform_->local_position_.set(-OFFSET_2, 0.0f, OFFSET_1);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createTriangleRight("triangle_right", desc, TEX_PATH);
	obj1->transform_->local_position_.set(-OFFSET_1, 0.0f, OFFSET_1);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createTriangleLeft("triangle_left", desc, TEX_PATH);
	obj1->transform_->local_position_.set(OFFSET_1, 0.0f, OFFSET_1);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createTriangleEquilateral("triangle_equilateral", desc, TEX_PATH);
	obj1->transform_->local_position_.set(OFFSET_2, 0.0f, OFFSET_1);
	obj1->addComponent<SampleComponent>();


	obj1 = eng::GameObject::createSphere("sphere", desc, TEX_PATH);
	obj1->transform_->local_position_.set(-OFFSET_2, 0.0f, -OFFSET_1);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createDome("dome", desc, TEX_PATH);
	obj1->transform_->local_position_.set(-OFFSET_1, -5.0f, -OFFSET_1);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createCone("cone", desc, TEX_PATH);
	obj1->transform_->local_position_.set(OFFSET_1, -5.0f, -OFFSET_1);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createDisk("disk", desc, TEX_PATH);
	obj1->transform_->local_position_.set(OFFSET_2, 0.0f, -OFFSET_1);
	obj1->addComponent<SampleComponent>();


	obj1 = eng::GameObject::createDiskRing("disk_ring", desc, TEX_PATH);
	obj1->transform_->local_position_.set(-OFFSET_1, 0.0f, -OFFSET_2);
	obj1->addComponent<SampleComponent>();

	obj1 = eng::GameObject::createCylinder("cylinder", desc, TEX_PATH);
	obj1->transform_->local_position_.set(OFFSET_1, 0.0f, -OFFSET_2);
	obj1->addComponent<SampleComponent>();


	return true;
}

void SampleScene::lateUpdate() {

	

}

void SampleScene::finalize() {



}
