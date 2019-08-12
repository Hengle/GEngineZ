#include "IComponent.h"
#include "IEntity.h"

namespace z {

void IComponent::Attach(IEntity* owner) {
	owner->AddComponent(this);
	mOwner = owner;
}

void IComponent::Detach() {
	if (mOwner) {
		mOwner->RemoveComponent(this);
		//mOwner = nullptr;
	}
}

}
