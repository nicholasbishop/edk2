#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Protocol/SerialIo.h>
#include <Uefi.h>

EFI_STATUS
EFIAPI
UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
  EFI_STATUS status = EFI_SUCCESS;
  EFI_SERIAL_IO_PROTOCOL *SerialProtocol = NULL;

  const int ONE_SECOND_IN_MICROSECONDS = 1000000;

  Print(L"Start SerialBug\n");

  status = SystemTable->BootServices->LocateProtocol(
      &gEfiSerialIoProtocolGuid, NULL, (VOID **)&SerialProtocol);
  if (status != EFI_SUCCESS)
    goto done;

  Print(L"Initial timeout: %d microseconds\n", SerialProtocol->Mode->Timeout);

  status = SerialProtocol->SetAttributes(
      SerialProtocol, SerialProtocol->Mode->BaudRate,
      SerialProtocol->Mode->ReceiveFifoDepth,
      /*Timeout=*/ONE_SECOND_IN_MICROSECONDS, SerialProtocol->Mode->Parity,
      SerialProtocol->Mode->DataBits, SerialProtocol->Mode->StopBits);
  if (status != EFI_SUCCESS)
    goto done;

  Print(L"After setting timeout: %d microseconds\n",
        SerialProtocol->Mode->Timeout);

  Print(L"Sleep for a second\n");
  SystemTable->BootServices->Stall(ONE_SECOND_IN_MICROSECONDS);

  Print(L"Timeout is now: %d microseconds\n", SerialProtocol->Mode->Timeout);

  status = EFI_SUCCESS;

done:
  Print(L"status: %d", status);
  SystemTable->BootServices->Stall(10 * ONE_SECOND_IN_MICROSECONDS);
  return status;
}
