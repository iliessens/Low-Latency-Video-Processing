
#pragma once

#include <atlstr.h>
#include "DeckLinkAPI_h.h"

class DeckLinkOutputDevice : public IDeckLinkVideoOutputCallback
{
private:
	IDeckLinkOutput*			m_deckLinkOutput;
	IDeckLink*					m_deckLink;
	int							m_refCount;
	bool						Init();

public:
	DeckLinkOutputDevice(IDeckLink* deckLink);
	int showFrame(IDeckLinkVideoFrame* frame);

	HRESULT StartOutput();
	virtual ~DeckLinkOutputDevice();

	// IUnknown
	virtual HRESULT		STDMETHODCALLTYPE QueryInterface(REFIID iid, LPVOID *ppv);
	virtual ULONG		STDMETHODCALLTYPE AddRef();
	virtual ULONG		STDMETHODCALLTYPE Release();

	// IDeckLinkVideoOutputCallback
	virtual HRESULT		STDMETHODCALLTYPE ScheduledFrameCompleted(IDeckLinkVideoFrame* completedFrame, BMDOutputFrameCompletionResult result);
	virtual HRESULT		STDMETHODCALLTYPE ScheduledPlaybackHasStopped();
};

