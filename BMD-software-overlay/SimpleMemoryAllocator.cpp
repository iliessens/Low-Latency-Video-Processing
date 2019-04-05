#include "SimpleMemoryAllocator.h"
#include "settings.h"
#include <stdio.h>

SimpleMemoryAllocator::SimpleMemoryAllocator() {
}

HRESULT SimpleMemoryAllocator::AllocateBuffer(unsigned int buffersize, void** allocatedbuffer) {
	if (buffersize > size) {
		printf("Buffer too small!\n");
		printf("%d bytes requested, but only %d available\n", buffersize, size);
		return E_OUTOFMEMORY;
	}
	
	*allocatedbuffer = (void*) memory;
	return S_OK;
}

HRESULT SimpleMemoryAllocator::ReleaseBuffer(void* buffer) {

	return S_OK;
}

HRESULT SimpleMemoryAllocator::Commit() {
	//Windows specific, could be changed by C++17 aligned_alloc but seems unsupported

	size = 8524800; // moeilijk te berekenen want ook audio en overhead moeten voorzien zijn
	memory = (uint8_t*)_aligned_malloc(size, 64);

	return S_OK;
}

HRESULT SimpleMemoryAllocator::Decommit() {
	_aligned_free(memory);
	return S_OK;
}

//------------------------------------------
// Complete interface with default functions
HRESULT	SimpleMemoryAllocator::QueryInterface(REFIID iid, LPVOID *ppv)
{
	HRESULT			result = E_NOINTERFACE;

	if (ppv == NULL)
		return E_INVALIDARG;

	// Initialise the return result
	*ppv = NULL;

	// Obtain the IUnknown interface and compare it the provided REFIID
	if (iid == IID_IUnknown)
	{
		*ppv = this;
		AddRef();
		result = S_OK;
	}
	else if (iid == IID_IDeckLinkDeviceNotificationCallback)
	{
		*ppv = (IDeckLinkDeviceNotificationCallback*)this;
		AddRef();
		result = S_OK;
	}

	return result;
}

ULONG SimpleMemoryAllocator::AddRef(void)
{
	return _InterlockedIncrement((volatile long *)&m_refCount);
}

ULONG SimpleMemoryAllocator::Release(void)
{
	ULONG newRefValue = _InterlockedDecrement((volatile long*)&m_refCount);
	if (newRefValue == 0)
		delete this;
	return newRefValue;
}