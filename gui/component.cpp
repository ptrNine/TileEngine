#include "component.hpp"

void CComponent::setAligment(U32 align) {
	if (_parent == nullptr) {
		_align = align;
		return;
	}

	// Flags : resizeBottom resizeRight bottom right
	U32 testFlagsEnable = _parent->_align & 3U;
	U32 testFlagsResize = ~((_parent->_align & 12U) >> 2U) & 3U;
	_align |= testFlagsEnable & testFlagsResize;
}