#include <iostream>
#include <windows.h>
#include <vector>
#include <atlimage.h>
#include <fstream>
#include <thread>

using namespace std;

int main()
{
	keybd_event(VK_MENU, 0, 0, 0); //Alt Press
	keybd_event(VK_SNAPSHOT, 0, 0, 0); //PrntScrn Press
	keybd_event(VK_SNAPSHOT, 0, KEYEVENTF_KEYUP, 0); //PrntScrn Release
	keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0); //Alt Release
	HBITMAP hBitmap;
	Sleep(200);
	OpenClipboard(NULL);
	hBitmap = (HBITMAP)GetClipboardData(CF_BITMAP);
	CloseClipboard();
	std::vector<BYTE> buf;
	IStream* stream = NULL;
	HRESULT hr = CreateStreamOnHGlobal(0, TRUE, &stream);
	CImage image;
	ULARGE_INTEGER liSize;
	image.Attach(hBitmap);
	image.Save(stream, Gdiplus::ImageFormatPNG);
	IStream_Size(stream, &liSize);
	DWORD len = liSize.LowPart;
	IStream_Reset(stream);
	buf.resize(len);
	IStream_Read(stream, &buf[16], len);
	stream->Release();
	std::fstream fi;
	fi.open("D:\\Code\\RandomProjects\\Cpp-Screenshot-Taker\\x64\\Debug\\Screenshot.png", std::fstream::binary | std::fstream::out);
	fi.write(reinterpret_cast<const char*>(&buf[16]), buf.size() * sizeof(BYTE));
	fi.close();
	std::this_thread::sleep_for(std::chrono::milliseconds(40));
}

