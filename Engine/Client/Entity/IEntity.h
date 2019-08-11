#pragma once
#include <Core/CoreHeader.h>
#include "IComponent.h"

namespace z {

class IEntity {
public:
	void AddComponent(IComponent* comp) {

	}

	std::vector<RefCountPtr<IComponent>> GetComponents(EComponentType) {

	}


	IComponent* GetComponent(EComponentType) {

	}

private:
	std::unordered_map<EComponentType, std::vector<RefCountPtr<IComponent>>> mComps;
};

}