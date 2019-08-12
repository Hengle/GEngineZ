#pragma once
#include <Core/CoreHeader.h>
#include "IComponent.h"

namespace z {

class IEntity : public RefCounter {
public:
	friend class IComponent;

	IEntity() {
		mLocalTransform = math::Matrix4F::Identity;
	}

	void SetLocalPostion(math::Vector3F pos) {
		mLocalTransform[0][3] = pos.x;
		mLocalTransform[1][3] = pos.y;
		mLocalTransform[2][3] = pos.z;
	}

	math::Matrix4F& GetWorldTransform() {
		// use local transoform now
		return mLocalTransform;
	}

	template<typename T>
	std::vector<T*> GetComponents() {
		std::vector<T*> ret;

		EComponentType tp = GetComponentType<T>();
		std::vector<RefCountPtr<IComponent>> &container = mComps[tp];

		for (auto &comp : container) {
			ret.push_back(static_cast<T*>(comp.GetRef()));
		}
		return ret;
	}

	template<typename T>
	T* GetComponent() {
		EComponentType tp = GetComponentType<T>();
		std::vector<RefCountPtr<IComponent>> &container = mComps[tp];
		if (container.size() == 0) {
			return nullptr;
		}
		return static_cast<T*>(container[0].GetRef());
	}

private:
	void AddComponent(IComponent* comp) {
		EComponentType tp = comp->GetType();
		mComps[tp].push_back(comp);
	}

	void RemoveComponent(IComponent* comp) {
		EComponentType tp = comp->GetType();
		std::vector<RefCountPtr<IComponent>> &container = mComps[tp];
		bool find = false;
		for (size_t i = 0; i < container.size(); i++) {
			if (container[i] == comp) {
				container[i] = container[container.size() - 1];
				find = true;
				break;
			}
		}
		if (find) {
			container.pop_back();
		}
	}

	math::Matrix4F mLocalTransform;
	std::unordered_map<EComponentType, std::vector<RefCountPtr<IComponent>>> mComps;
};

}