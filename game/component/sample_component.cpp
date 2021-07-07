#include "sample_component.h"



void SampleComponent::update() {

	//--------------------------------------------------
	// ‰ñ“]

	getTransform()->rotate(0.0f, lib::Math::toRadian(180.0f * lib::Time::getDeltaTime()), 0.0f);
	//getTransform()->translate(0.0f, 0.0f, 20.0f * lib::Time::getDeltaTime());

	//--------------------------------------------------

}
