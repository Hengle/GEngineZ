#pragma once

#include <direct.h>


class COMException {
public:
	COMException(HRESULT hr) : error_(hr) {}
	HRESULT Error() const {
		return error_;
	}
private:
	const HRESULT error_;
};

#define DX12_CHECK(hr) if (FAILED(hr)){ throw COMException(hr); }