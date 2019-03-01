#include "DeckLinkInput.h"
#include "settings.h"

#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <comutil.h>

DeckLinkInputDevice::DeckLinkInputDevice(IDeckLink* device, VideoProcessor* videoprocessor)
	: m_deckLink(device), m_deckLinkInput(NULL), processor(videoprocessor)
{
	m_deckLink->AddRef();
	this->init();
}

HRESULT DeckLinkInputDevice::init()
{
	HRESULT							result;
	IDeckLinkDisplayModeIterator*	displayModeIterator = NULL;
	IDeckLinkDisplayMode*			displayMode = NULL;

	result = m_deckLink->QueryInterface(IID_IDeckLinkInput, (void**)&m_deckLinkInput);
	if (result != S_OK)
	{
		fprintf(stderr, "Unable to get IDeckLinkInput interface\n");
		goto bail;
	}

	// Retrieve and cache mode list
	result = m_deckLinkInput->GetDisplayModeIterator(&displayModeIterator);
	if (result != S_OK)
	{
		fprintf(stderr, "Unable to get IDeckLinkDisplayModeIterator interface\n");
		goto bail;
	}


bail:
	if (displayModeIterator != NULL)
	{
		displayModeIterator->Release();
		displayModeIterator = NULL;
	}

	return result;
}

HRESULT DeckLinkInputDevice::StartCapture()
{
	HRESULT result;
	BMDVideoInputFlags inputFlags = bmdVideoInputFlagDefault;

	inputFlags |= bmdVideoInputEnableFormatDetection;

	// Set capture callback
	m_deckLinkInput->SetCallback(this);

	// Set the video input mode
	result = m_deckLinkInput->EnableVideoInput(DISPLAY_MODE, PIXEL_MODE, inputFlags);
	if (result != S_OK)
	{
		fprintf(stderr, "Unable to enable video input. Perhaps, the selected device is currently in-use.\n");
		goto bail;
	}

	// Start the capture
	result = m_deckLinkInput->StartStreams();
	if (result != S_OK)
	{
		fprintf(stderr, "Unable to start input streams\n");
		goto bail;
	}

bail:
	return result;
}

void DeckLinkInputDevice::StopCapture()
{
	if (m_deckLinkInput != NULL)
	{
		// Unregister capture callback
		m_deckLinkInput->SetCallback(NULL);

		// Stop the capture
		m_deckLinkInput->StopStreams();

		// Disable video input
		m_deckLinkInput->DisableVideoInput();
	}
}

HRESULT DeckLinkInputDevice::VideoInputFormatChanged(/* in */ BMDVideoInputFormatChangedEvents notificationEvents, /* in */ IDeckLinkDisplayMode *newMode, /* in */ BMDDetectedVideoInputFormatFlags detectedSignalFlags)
{
	printf("Video Format changed.\n");

	BSTR nameBSTR;
	newMode->GetName(&nameBSTR);
	_bstr_t		modeName(nameBSTR, false);

	printf("Mode: %s\n", (char*) modeName);
	
	printf("Pixel format: ");
	if (detectedSignalFlags & bmdDetectedVideoInputRGB444) printf("10-bit RGB\n");
	else printf("10-bit YUV\n");
	
	return 0;
}

HRESULT DeckLinkInputDevice::VideoInputFrameArrived(/* in */ IDeckLinkVideoInputFrame* videoFrame, /* in */ IDeckLinkAudioInputPacket* audioPacket)
{
	if (videoFrame)
	{
		bool inputFrameValid = ((videoFrame->GetFlags() & bmdFrameHasNoInputSource) == 0);


		if (inputFrameValid)
		{
			videoFrame->AddRef();
			processor->publishFrame(videoFrame);
		}

	}

	return S_OK;
}

HRESULT	STDMETHODCALLTYPE DeckLinkInputDevice::QueryInterface(REFIID iid, LPVOID *ppv)
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
	else if (iid == IID_IDeckLinkInputCallback)
	{
		*ppv = (IDeckLinkInputCallback*)this;
		AddRef();
		result = S_OK;
	}

	return result;
}

ULONG STDMETHODCALLTYPE DeckLinkInputDevice::AddRef(void)
{
	return m_refCount.fetch_add(1);
}

ULONG STDMETHODCALLTYPE DeckLinkInputDevice::Release(void)
{
	int		newRefValue;

	newRefValue = m_refCount.fetch_sub(1);
	if (newRefValue == 0)
	{
		delete this;
		return 0;
	}

	return newRefValue;
}