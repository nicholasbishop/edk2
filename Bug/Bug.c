typedef VOID(EFIAPI *EFI_AP_PROCEDURE)(IN OUT VOID *Buffer);

#include <Library/UefiApplicationEntryPoint.h>
#include <Library/UefiLib.h>
#include <Protocol/MpService.h>
#include <Uefi.h>

EFI_STATUS
EFIAPI
UefiMain(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE *SystemTable) {
  EFI_STATUS status = EFI_SUCCESS;
  EFI_MP_SERVICES_PROTOCOL *MpProtocol = NULL;
  EFI_HANDLE *handles = NULL;
  UINTN num_handles = 0;
  UINTN num_processors = 0;
  UINTN num_enabled_processors = 0;

  Print(L"Start MP test\n");

  status = SystemTable->BootServices->LocateHandleBuffer(
      ByProtocol, &gEfiMpServiceProtocolGuid,
      /*SearchKey=*/NULL, &num_handles, &handles);
  if (status != EFI_SUCCESS)
    goto done;

  Print(L"num_handles=%d\n", num_handles);
  if (num_handles < 1) {
    goto done;
  }

  status = SystemTable->BootServices->OpenProtocol(
      handles[0], &gEfiMpServiceProtocolGuid, (VOID **)&MpProtocol,
      /*AgentHandle=*/ImageHandle,
      /*ControllerHandle=*/NULL,
      /*Attributes=*/EFI_OPEN_PROTOCOL_EXCLUSIVE);
  if (status != EFI_SUCCESS)
    goto done;

  status = MpProtocol->GetNumberOfProcessors(MpProtocol, &num_processors,
                                             &num_enabled_processors);
  if (status != EFI_SUCCESS)
    goto done;
  Print(L"num_processors=%d, num_enabled_processors=%d\n", num_processors,
        num_enabled_processors);

  status = EFI_SUCCESS;

done:
  Print(L"status: %d", status);
  return status;
}
