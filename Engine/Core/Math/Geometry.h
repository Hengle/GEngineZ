#pragma once
#include "Vector.h"
#include "Number.h"
#include <algorithm>
#include <iostream>

namespace z {
namespace math {

typedef Vector2F Point2D;
typedef Vector3F Point3D;

class Rect {

};

class Box {
public:
    Box() {
        MinP = Point3D(K_MAXFLOAT);
        MaxP = Point3D(-K_MAXFLOAT);
    }

    void Union(const Point3D &p) {
        MinP = Point3D(std::min(p.x, MinP.x), std::min(p.y, MinP.y), std::min(p.z, MinP.z));
        MaxP = Point3D(std::max(p.x, MaxP.x), std::max(p.y, MaxP.y), std::max(p.z, MaxP.z));

    }

    friend std::ostream& operator<<(std::ostream& out, const Box& v) {
        out << "Box" << v.MinP << ", " << v.MaxP;
        return out;
    }

    Point3D MinP;
    Point3D MaxP;
};


}
}