#pragma once

#include "DeckLinkAPI_h.h"
#include <stdint.h>

class SimpleMemoryAllocator : public IDeckLinkMemoryAllocator {
private:
	uint8_t* memory;
	int size;
	int	m_refCount;

public:
	SimpleMemoryAllocator();

	//Manual: buffersize of type unsigned long; code differs
	HRESULT AllocateBuffer(unsigned int buffersize, void ** allocatedbuffer);
	HRESULT ReleaseBuffer(void * buffer);
	HRESULT Commit();
	HRESULT Decommit();

	//Mandatory methods without special implementation
	HRESULT QueryInterface(REFIID iid, LPVOID * ppv);
	ULONG AddRef(void);
	ULONG Release(void);
};