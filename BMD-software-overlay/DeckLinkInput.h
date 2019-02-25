#pragma once

#include "DeckLinkAPI_h.h"
#include "VideoProcessor.h"
#include <atomic>

class DeckLinkInputDevice : public IDeckLinkInputCallback
{
private:
	IDeckLink*							m_deckLink;
	IDeckLinkInput*						m_deckLinkInput;
	std::atomic<uint32_t>				m_refCount;
	VideoProcessor*						processor;
	
	HRESULT init();
public:
	DeckLinkInputDevice(IDeckLink* device, VideoProcessor* videoprocessor);

	HRESULT StartCapture();
	void StopCapture();

	// IDeckLinkInputCallback interface
	virtual HRESULT STDMETHODCALLTYPE	VideoInputFormatChanged(BMDVideoInputFormatChangedEvents notificationEvents, IDeckLinkDisplayMode *newDisplayMode, BMDDetectedVideoInputFormatFlags detectedSignalFlags);
	virtual HRESULT STDMETHODCALLTYPE	VideoInputFrameArrived(IDeckLinkVideoInputFrame* videoFrame, IDeckLinkAudioInputPacket* audioPacket);

	// IUnknown interface
	virtual HRESULT	STDMETHODCALLTYPE	QueryInterface(REFIID iid, LPVOID *ppv);
	virtual ULONG	STDMETHODCALLTYPE	AddRef();
	virtual ULONG	STDMETHODCALLTYPE	Release();
};