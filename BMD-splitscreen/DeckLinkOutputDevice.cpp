//
//  DeckLinkOutputDevice.cpp
//  Decklink output device callback
//

#include "DeckLinkOutputDevice.h"
#include "settings.h"
#include "Chronometer.h"

DeckLinkOutputDevice::DeckLinkOutputDevice(IDeckLink* deckLink) 
	: m_deckLink(deckLink), m_refCount(1)
{
	m_deckLink->AddRef();
	this->Init();
}

HRESULT DeckLinkOutputDevice::StartOutput() {
	BMDVideoOutputFlags flags = bmdVideoOutputFlagDefault; // no extra flags necessary
	return m_deckLinkOutput->EnableVideoOutput(DISPLAY_MODE, flags);
}

DeckLinkOutputDevice::~DeckLinkOutputDevice()
{
	if (m_deckLinkOutput)
	{
		m_deckLinkOutput->Release();
		m_deckLinkOutput = NULL;
	}

	if (m_deckLink)
	{
		m_deckLink->Release();
		m_deckLink = NULL;
	}
}

bool DeckLinkOutputDevice::Init()
{
	IDeckLinkDisplayModeIterator*   displayModeIterator = NULL;
	IDeckLinkDisplayMode*           displayMode = NULL;

	// Get output interface
	if (m_deckLink->QueryInterface(IID_IDeckLinkOutput, (void**)&m_deckLinkOutput) != S_OK)
		return false;

	return true;
}

HRESULT	DeckLinkOutputDevice::QueryInterface(REFIID iid, LPVOID *ppv)
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

ULONG DeckLinkOutputDevice::AddRef(void)
{
	return _InterlockedIncrement((volatile long *)&m_refCount);
}

ULONG DeckLinkOutputDevice::Release(void)
{
	ULONG newRefValue = _InterlockedDecrement((volatile long*)&m_refCount);
	if (newRefValue == 0)
		delete this;
	return newRefValue;
}

HRESULT	DeckLinkOutputDevice::ScheduledFrameCompleted(IDeckLinkVideoFrame* completedFrame, BMDOutputFrameCompletionResult result)
{
	// When a scheduled video frame is complete, schedule next frame  
	return S_OK;
}

HRESULT	DeckLinkOutputDevice::ScheduledPlaybackHasStopped()
{
	return S_OK;
}

int DeckLinkOutputDevice::showFrame(IDeckLinkVideoFrame* frame) {
	HRESULT result = m_deckLinkOutput->DisplayVideoFrameSync(frame);
	frame->Release();

	Chronometer::stopAndPrint();

	if (result == S_OK)  return 0;
	else return 1;
}

void DeckLinkOutputDevice::getEmptyFrame(IDeckLinkMutableVideoFrame** frame) {
	const int rowBytes = WIDTH * 2;
	HRESULT result= m_deckLinkOutput->CreateVideoFrame(WIDTH, HEIGHT, rowBytes, PIXEL_MODE, bmdFrameFlagDefault, frame);

	if (result != S_OK) printf("Error allocating output frame\n");
}
