#include <stdio.h>
#include "xparameters.h"
#include "xllfifo.h"
#include "xstatus.h"

#define WORD_SIZE 		8 // Size of words in bytes
#define INPUT_LEN 		7 // Weight and input
#define OUTPUT_LEN		2 // Output

int Init_XLlFifo(XLlFifo *InstancePtr, u16 DeviceId);
int TxSend(XLlFifo *InstancePtr, u64 *SourceAddr);
int RxReceive(XLlFifo *InstancePtr, u64 *DestinationAddr);

XLlFifo FifoInstance;
u64 SourceBuffer[INPUT_LEN];
u64 DestinationBuffer[OUTPUT_LEN];
float out0[4], out1[4];

int main()
{
	// Initialize AXI Stream FIFO IP
	Init_XLlFifo(&FifoInstance, XPAR_AXI_FIFO_0_DEVICE_ID);

    printf("Initialization success\n");

    // Weight
    SourceBuffer[0] = 0x0000B07A057A057A;
    SourceBuffer[1] = 0x0000FC6603E10314;
    SourceBuffer[2] = 0x0000FC70028F0433;
    SourceBuffer[3] = 0xF5A30051FAC21C70;
    SourceBuffer[4] = 0x00CC07E10685E399;

    // Input
    SourceBuffer[5] = 0x1400140020002000;
    SourceBuffer[6] = 0x1400200014002000;

    // Check weight and input
    printf("SourceBuffer:\n");
    for (int i = 0; i < INPUT_LEN; i++)
    	printf(" 0x%016llX\n", SourceBuffer[i]);

    // Send to NN core
    TxSend(&FifoInstance, SourceBuffer);

    // Read from NN core
    RxReceive(&FifoInstance, DestinationBuffer);

    // Check output
    printf("DestinationBuffer:\n");
    for (int i = 0; i < OUTPUT_LEN; i++)
    	printf(" 0x%016llX\n", DestinationBuffer[i]);

    // Decode output (We use 10 fraction bits, so we divide by 2^10 = 1024)
    out0[0] = (u16)((DestinationBuffer[0] & 0x000000000000FFFF)) / 1024.0;
    out1[0] = (u16)((DestinationBuffer[1] & 0x000000000000FFFF)) / 1024.0;
    out0[1] = (u16)((DestinationBuffer[0] & 0x00000000FFFF0000) >> 16) / 1024.0;
    out1[1] = (u16)((DestinationBuffer[1] & 0x00000000FFFF0000) >> 16) / 1024.0;
    out0[2] = (u16)((DestinationBuffer[0] & 0x0000FFFF00000000) >> 32) / 1024.0;
    out1[2] = (u16)((DestinationBuffer[1] & 0x0000FFFF00000000) >> 32) / 1024.0;
    out0[3] = (u16)((DestinationBuffer[0] & 0xFFFF000000000000) >> 48) / 1024.0;
    out1[3] = (u16)((DestinationBuffer[1] & 0xFFFF000000000000) >> 48) / 1024.0;

    // Print final output
    printf("Final NN output:\n");
    printf(" [%.3f, %.3f]\n", out0[0], out1[0]);
    printf(" [%.3f, %.3f]\n", out0[1], out1[1]);
    printf(" [%.3f, %.3f]\n", out0[2], out1[2]);
    printf(" [%.3f, %.3f]\n", out0[3], out1[3]);

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

int TxSend(XLlFifo *InstancePtr, u64 *SourceAddr)
{
	// Writing into the FIFO transmit buffer
	for(int i = 0; i < INPUT_LEN; i++)
		if (XLlFifo_iTxVacancy(InstancePtr))
			Xil_Out64(InstancePtr->Axi4BaseAddress + XLLF_AXI4_TDFD_OFFSET, *(SourceAddr+i));

	// Start transmission by writing transmission length into the TLR
	XLlFifo_iTxSetLen(InstancePtr, (INPUT_LEN * WORD_SIZE));

	// Check for transmission completion
	while (!(XLlFifo_IsTxDone(InstancePtr)));

	return XST_SUCCESS;
}

int RxReceive(XLlFifo *InstancePtr, u64* DestinationAddr)
{
	static u32 ReceiveLength;
	u64 RxWord;
	int Status;

	while (XLlFifo_iRxOccupancy(InstancePtr))
	{
		// Read receive length
		ReceiveLength = XLlFifo_iRxGetLen(InstancePtr) / WORD_SIZE;
		// Reading from the FIFO receive buffer
		for (int i = 0; i < ReceiveLength; i++)
		{
			RxWord = Xil_In64(InstancePtr->Axi4BaseAddress + XLLF_AXI4_RDFD_OFFSET);
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
