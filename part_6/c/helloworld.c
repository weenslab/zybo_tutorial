#include <stdio.h>
#include "xparameters.h"
#include "xllfifo.h"
#include "xstatus.h"

#define WORD_SIZE 		4 // Size of words in bytes
#define DATA_LEN 		8 // Number of data

int Init_XLlFifo(XLlFifo *InstancePtr, u16 DeviceId);
int TxSend(XLlFifo *InstancePtr, u32 *SourceAddr);
int RxReceive(XLlFifo *InstancePtr, u32 *DestinationAddr);

XLlFifo FifoInstance;
u32 SourceBuffer[DATA_LEN];
u32 DestinationBuffer[DATA_LEN];

int main()
{
	// Initialize AXI Stream FIFO IP
	Init_XLlFifo(&FifoInstance, XPAR_AXI_FIFO_0_DEVICE_ID);

    printf("Initialization success\n");

	printf("Input:\n");
    for (int i = 0; i <= 7; i++)
    {
    	uint8_t a = i + 1;
    	uint8_t b = 8 - i;
    	uint8_t y = i + 1;
    	SourceBuffer[i] = (y << 16) | (b << 8) | a;
    	printf(" a=%d, b=%d, y=%d\n", a, b, y);
    }

    // Send to NN core
    TxSend(&FifoInstance, SourceBuffer);

    // Read from NN core
    RxReceive(&FifoInstance, DestinationBuffer);

    // Read input
    printf("Output:\n");
    for (int i = 0; i <= 7; i++)
    	printf(" %ld\n", DestinationBuffer[i]);

    return 0;
}

int Init_XLlFifo(XLlFifo *InstancePtr, u16 DeviceId)
{
	XLlFifo_Config *Config;
	int Status;

	Config = XLlFfio_LookupConfig(DeviceId);
	if (!Config)
	{
		printf("No config found for %d\n", DeviceId);
		return XST_FAILURE;
	}

	Status = XLlFifo_CfgInitialize(InstancePtr, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS)
	{
		printf("Initialization failed\n");
		return XST_FAILURE;
	}

	XLlFifo_IntClear(InstancePtr, 0xffffffff);
	Status = XLlFifo_Status(InstancePtr);
	if (Status != 0x0)
	{
		printf("Reset failed\n");
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int TxSend(XLlFifo *InstancePtr, u32 *SourceAddr)
{
	// Writing into the FIFO transmit buffer
	for(int i = 0; i < DATA_LEN; i++)
		if (XLlFifo_iTxVacancy(InstancePtr))
			Xil_Out32(InstancePtr->Axi4BaseAddress + XLLF_TDFD_OFFSET, *(SourceAddr+i));

	// Start transmission by writing transmission length into the TLR
	XLlFifo_iTxSetLen(InstancePtr, (DATA_LEN * WORD_SIZE));

	// Check for transmission completion
	while (!(XLlFifo_IsTxDone(InstancePtr)));

	return XST_SUCCESS;
}

int RxReceive(XLlFifo *InstancePtr, u32* DestinationAddr)
{
	static u32 ReceiveLength;
	u32 RxWord;
	int Status;

	while (XLlFifo_iRxOccupancy(InstancePtr))
	{
		// Read receive length
		ReceiveLength = XLlFifo_iRxGetLen(InstancePtr) / WORD_SIZE;
		// Reading from the FIFO receive buffer
		for (int i = 0; i < ReceiveLength; i++)
		{
			RxWord = Xil_In32(InstancePtr->Axi4BaseAddress + XLLF_RDFD_OFFSET);
			*(DestinationAddr+i) = RxWord;
		}
	}

	// Check for receive completion
	Status = XLlFifo_IsRxDone(InstancePtr);
	if (Status != TRUE)
	{
		printf("Failing in receive complete\n");
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}
