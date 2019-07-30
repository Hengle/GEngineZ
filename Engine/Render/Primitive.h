#pragma once
#include <Core/CoreHeader.h>
#include <RHI/RHIResource.h> 
namespace z {

class Primitive {
public:



	std::vector<float>& GetVertexes() {
		return mVertexes;
	}

	std::vector<uint16_t>& GetIndices() {
		return mIndices;
	}

	void Create();

private:
	std::vector<float> mVertexes;
	std::vector<uint16_t> mIndices;


};

}