#include <stdio.h>
#include <tchar.h>
#include <conio.h>
#include <objbase.h>		// Necessary for COM
#include <comutil.h>
#include "DeckLinkAPI_h.h"

#include "settings.h"
#include "DeckLinkInput.h"
#include "DeckLinkOutputDevice.h"

IDeckLinkIterator* initCom() {
	IDeckLinkIterator* deckLinkIterator;

	// Initialize COM on this thread
	HRESULT result = CoInitialize(NULL);
	if (FAILED(result))
	{
		fprintf(stderr, "Initialization of COM failed - result = %08x.\n", result);
		return NULL;
	}

	// Create an IDeckLinkIterator object to enumerate all DeckLink cards in the system
	result = CoCreateInstance(CLSID_CDeckLinkIterator, NULL, CLSCTX_ALL, IID_IDeckLinkIterator, (void**)&deckLinkIterator);
	if (FAILED(result))
	{
		fprintf(stderr, "A DeckLink iterator could not be created.  The DeckLink drivers may not be installed.\n");
		return NULL;
	}
	
	return deckLinkIterator;
}

IDeckLink* findDeckLink(IDeckLinkIterator* deckLinkIterator) {
	IDeckLink*		selectedDecklink = NULL;
	IDeckLink*		deckLink;
	int			numDevices = 0;
	HRESULT		result;

	// Enumerate all cards in this system
	while (deckLinkIterator->Next(&deckLink) == S_OK)
	{
		BSTR		deviceNameBSTR = NULL;

		// Increment the total number of DeckLink cards found
		numDevices++;

		// save pointer to selected device
		if (numDevices == BMD_DEVICE) {
			printf("-> ");
		}

		// *** Print the model name of the DeckLink card
		result = deckLink->GetModelName(&deviceNameBSTR);
		if (result == S_OK)
		{
			_bstr_t		deviceName(deviceNameBSTR, false);

			printf("(%d) %s \n", numDevices, (char*)deviceName);
		}

		if(numDevices == BMD_DEVICE) selectedDecklink = deckLink;
		else deckLink->Release(); // Release the IDeckLink instance when we've finished with it to prevent leaks
	}

	deckLinkIterator->Release();

	// If no DeckLink cards were found in the system, inform the user
	if (numDevices == 0)
		printf("No Blackmagic Design devices were found.\n");

	return selectedDecklink;
}

int	_tmain (int argc, _TCHAR* argv[])
{
	HRESULT						result;
	
	IDeckLinkIterator* deckLinkIterator = initCom();
	if (deckLinkIterator == NULL) return 1;

	IDeckLink*		deckLink = findDeckLink(deckLinkIterator);

	printf("\n");
	if (deckLink == NULL) {
		printf("The selected device was not found!\n");
		return 1;
	}

	VideoProcessor* processor = new VideoProcessor();

	DeckLinkOutputDevice* output = new DeckLinkOutputDevice(deckLink);
	processor->setOutput(output);
	output->StartOutput();

	DeckLinkInputDevice* input = new DeckLinkInputDevice(deckLink,processor);
	result = input->StartCapture();

	// Wait for any key press before exiting
	_getch();

	input->StopCapture();
	input->Release();

	output->Release();
	
	// Uninitalize COM on this thread
	CoUninitialize();
	
	return 0;
}
