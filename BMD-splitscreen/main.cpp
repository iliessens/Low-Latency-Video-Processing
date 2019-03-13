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

void findDeckLink(IDeckLinkIterator* deckLinkIterator, IDeckLink** decklink1, IDeckLink** decklink2) {
	IDeckLink*		deckLink;
	int			numDevices = 0;
	HRESULT		result;

	// Enumerate all cards in this system
	while (deckLinkIterator->Next(&deckLink) == S_OK)
	{
		BSTR		deviceNameBSTR = NULL;

		// Increment the total number of DeckLink cards found
		numDevices++;

		if (numDevices == BMD_DEVICE_1) {
			printf("1 -> ");
		}
		if (numDevices == BMD_DEVICE_2) {
			printf("2 -> ");
		}

		// *** Print the model name of the DeckLink card
		result = deckLink->GetModelName(&deviceNameBSTR);
		if (result == S_OK)
		{
			_bstr_t		deviceName(deviceNameBSTR, false);

			printf("(%d) %s \n", numDevices, (char*)deviceName);
		}

		if(numDevices == BMD_DEVICE_1) *decklink1 = deckLink;
		else if (numDevices == BMD_DEVICE_2) *decklink2 = deckLink;
		else deckLink->Release(); // Release the IDeckLink instance when we've finished with it to prevent leaks
	}

	deckLinkIterator->Release();

	// If no DeckLink cards were found in the system, inform the user
	if (numDevices == 0)
		printf("No Blackmagic Design devices were found.\n");
}

int	_tmain (int argc, _TCHAR* argv[])
{
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS); // high priority to prevent slowdown on high cpu load
	
	HRESULT						result;
	
	IDeckLinkIterator* deckLinkIterator = initCom();
	if (deckLinkIterator == NULL) return 1;

	IDeckLink* deckLink1;
	IDeckLink* deckLink2;

	findDeckLink(deckLinkIterator, &deckLink1, &deckLink2);

	printf("\n");
	if (deckLink1 == NULL) {
		printf("The selected device 1 was not found!\n");
		return 1;
	}
	if (deckLink2 == NULL) {
		printf("The selected device 2 was not found!\n");
		return 1;
	}

	VideoProcessor* processor = new VideoProcessor();

	DeckLinkOutputDevice* output = new DeckLinkOutputDevice(deckLink1);
	processor->setOutput(output);
	output->StartOutput();

	DeckLinkInputDevice* input1 = new DeckLinkInputDevice(deckLink1,processor,1);
	result = input1->StartCapture();

	DeckLinkInputDevice* input2 = new DeckLinkInputDevice(deckLink2, processor, 2);
	result &= input2->StartCapture();

	// Wait for any key press before exiting
	_getch();

	input1->StopCapture();
	input1->Release();

	input2->StopCapture();
	input2->Release();

	output->Release();
	
	// Uninitalize COM on this thread
	CoUninitialize();
	
	return 0;
}
