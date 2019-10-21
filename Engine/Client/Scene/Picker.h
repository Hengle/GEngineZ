#pragma once
#include <Core/CoreHeader.h>

namespace z {

class IPickableItem {
public:
    virtual bool Pickable();
};

class Picker {
public:
    void AddPickableItem(IPickableItem *item);
    void DelPickableItem(IPickableItem *item);

    void Pick(math::Vector3F rayStart, math::Vector3F rayDir);
private:
};

}