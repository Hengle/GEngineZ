#pragma once
#include <Core/CoreHeader.h>

namespace z {

class IEntity;
class SceneCollection;



enum EComponentType {
	COMP_INVALID,
	COMP_PRIMITIVE,
	COMP_ENV,
	COMP_CAMERA,
};

template<typename T>
constexpr EComponentType GetComponentType() {
	T t;
	return t.GetType();
}

class IComponent : public RefCounter{
public:
	virtual EComponentType GetType() = 0;

	void Attach(IEntity* owner);
	void Detach();


	// collect render items
	virtual void CollectRender(SceneCollection*) {};

protected:
	IEntity* mOwner{ nullptr };
};

}