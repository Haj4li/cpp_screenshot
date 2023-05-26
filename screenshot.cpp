#include <windows.h>
#include <gdiplus.h>
#include <iostream>
#include <fstream>
#include <string>

#pragma comment(lib, "gdiplus.lib")

// https://github.com/Haj4li

void screenshot(std::string file)
{
    ULONG_PTR	gdiplustoken;
    Gdiplus::GdiplusStartupInput gdiplusstartupinput;
    Gdiplus::GdiplusStartupOutput gdiplusstartupoutput;
    gdiplusstartupinput.SuppressBackgroundThread = true;
    GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, &gdiplusstartupoutput);//start GDI+

    HDC dc = GetDC(GetDesktopWindow());//Get Desktop Context
    HDC dc2 = CreateCompatibleDC(dc);//Copy Context

    RECT rcOkno;
    GetClientRect(GetDesktopWindow(), &rcOkno);//Get desktop size
    int w = rcOkno.right - rcOkno.left;//width
    int h = rcOkno.bottom - rcOkno.top;//height
    HBITMAP hbitmap = CreateCompatibleBitmap(dc, w, h);//Create bitmap
    HBITMAP holdbitmap = (HBITMAP)SelectObject(dc2, hbitmap);
    BitBlt(dc2, 0, 0, w, h, dc, 0, 0, SRCCOPY);//Copy pixels from pulpit to bitmap

    Gdiplus::Bitmap* bm = new Gdiplus::Bitmap(hbitmap, NULL);//Create GDI+ bitmap

    UINT num;
    UINT size;
    Gdiplus::ImageCodecInfo* imagecodecinfo;
    Gdiplus::GetImageEncodersSize(&num, &size);//get count of codecs
    imagecodecinfo = (Gdiplus::ImageCodecInfo*)(malloc(size));
    Gdiplus::GetImageEncoders(num, size, imagecodecinfo);//get codecs

    CLSID clsidEncoder;

    for (unsigned int i = 0; i < num; i++)
    {
        if (wcscmp(imagecodecinfo[i].MimeType, L"image/jpeg") == 0)
            clsidEncoder = imagecodecinfo[i].Clsid;//get jpeg codec id
    }
    free(imagecodecinfo);

    std::wstring ws;
    ws.assign(file.begin(), file.end());//string->wstring
    bm->Save(ws.c_str(), &clsidEncoder);//save in jpeg format
    SelectObject(dc2, holdbitmap);//Release objects
    DeleteObject(dc2);
    DeleteObject(hbitmap);

    ReleaseDC(GetDesktopWindow(), dc);
    Gdiplus::GdiplusShutdown(gdiplustoken);
}

int main() {
    screenshot("screenshot.jpg");
    return 0;
}
