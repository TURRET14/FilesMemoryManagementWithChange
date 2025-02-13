#include <windows.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("3 Arguments Were NOT Provided.");
		return 1;
	}
	char* FileName = argv[1];
	HANDLE File = CreateFile(argv[1], GENERIC_READ | GENERIC_WRITE, NULL, NULL, OPEN_EXISTING, NULL, NULL);
	if (File == INVALID_HANDLE_VALUE) {
		printf("Error.");
		return 1;
	}
	HANDLE FileMapping = CreateFileMapping(File, NULL, PAGE_READWRITE, 0, 256, NULL);
	if (FileMapping == INVALID_HANDLE_VALUE || FileMapping == NULL) {
		printf("Error.");
		printf("%d", GetLastError());
		return 1;
	}
	char* MappedMemory = (char*)MapViewOfFile(FileMapping, FILE_MAP_WRITE, 0, 0, 0);
	if (MappedMemory == INVALID_HANDLE_VALUE || MappedMemory == NULL) {
		printf("Error.");
		return 1;
	}
	HANDLE WFile = CreateFile("FileCopy.txt", GENERIC_READ | GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);
	if (WFile == INVALID_HANDLE_VALUE) {
		printf("Error.");
		return 1;
	}
	HANDLE WFileMapping = CreateFileMapping(WFile, NULL, PAGE_READWRITE, 0, 256, NULL);
	if (WFileMapping == INVALID_HANDLE_VALUE || WFileMapping == NULL) {
		printf("Error.");
		return 1;
	}
	wchar_t* WMappedMemory = (wchar_t*)MapViewOfFile(WFileMapping, FILE_MAP_WRITE, 0, 0, 0);
	if (WMappedMemory == INVALID_HANDLE_VALUE || WMappedMemory == NULL) {
		printf("Error.");
		return 1;
	}

	if (strlen(MappedMemory) == 0) {
		printf("File Is empty!");
		return 1;
	}
	int ReplaceNumber = atoi(argv[2]);
	int FirstChar = MappedMemory[0];
	int Count = 0;
	int Replaced = 0;
	char ReadChar = MappedMemory[Count];
	while (ReadChar != '\0') {
		if (ReadChar == ' ' && Replaced < ReplaceNumber) {
			ReadChar = FirstChar;
			Replaced = Replaced + 1;
		}
		wchar_t WChar = (wchar_t)ReadChar;
		WMappedMemory[Count] = WChar;
		Count = Count + 1;
		ReadChar = MappedMemory[Count];
	}
	UnmapViewOfFile(MappedMemory);
	UnmapViewOfFile(WMappedMemory);
	CloseHandle(FileMapping);
	CloseHandle(WFileMapping);
	CloseHandle(File);
	CloseHandle(WFile);
	return Replaced;
}