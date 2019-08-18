#pragma once
#include <Core/CoreHeader.h>
#include "IComponent.h"
#include "Transform.h"

namespace z {

class IEntity : public RefCounter {
public:
	friend class IComponent;

	IEntity() {
	}

	// transform opertion
	void SetLocalTransform(const math::Matrix4F& transform) {
		mLocalTransform.SetTransform(transform);
	}

	void SetLocalPostion(const math::Vector3F& pos) {
		mLocalTransform.SetPostion(pos);
	}

	void SetLocalScale(const math::Vector3F& scale) {
		mLocalTransform.SetScale(scale);
	}

	void SetLocalRotator(const math::Vector3F rotator) {
		mLocalTransform.SetRotator(rotator);
	}

	math::Matrix4F GetLocalTransform() {
		return mLocalTransform.GetTransform();
	}

	math::Vector3F GetLocalPosition() {
		return mLocalTransform.GetPosition();
	}

	math::Vector3F GetLocalRotation() {
		return mLocalTransform.GetRotator();
	}

	math::Vector3F GetLocalScale() {
		return mLocalTransform.GetScale();
	}

	math::Matrix4F GetWorldTransform() {
		// use local transoform now
		return mLocalTransform.GetTransform();

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

	Transform mLocalTransform;
	Transform mWorldTransform;

	std::unordered_map<EComponentType, std::vector<RefCountPtr<IComponent>>> mComps;
};

}