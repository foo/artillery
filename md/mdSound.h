#pragma once

#define INITGUID

#include <dmusicc.h>
#include <dmusici.h>

class MDSound
{
private:
	static HWND hwnd;
	IDirectMusicLoader8 *loader;
	IDirectMusicPerformance8 *performance;
	IDirectMusicSegment8 *segment;
public:
	MDSound()
	{
	}
	~MDSound()
	{
	}
	MDSound(char *filename)
	{
		Load(filename);
	}

	static void Initialize(HWND hwnd_p)
	{
		CoInitialize(NULL);
		hwnd = hwnd_p;
	}
	static void Close()
	{
		//performance->Stop(NULL, NULL, 0, 0);
		//performance->CloseDown();
	}
	
	void Play(const bool loop = false)
	{
		if (loop)
		{
			segment->SetRepeats(DMUS_SEG_REPEAT_INFINITE);
		}
		performance->PlaySegmentEx(segment, NULL, NULL, 0, 0, NULL, NULL, NULL);
	}
	void Stop()
	{
		performance->StopEx(segment, 0, 0);
	}
	void Load(char *filename)
	{
		if (FAILED(CoCreateInstance(CLSID_DirectMusicPerformance, NULL, CLSCTX_INPROC,
							IID_IDirectMusicPerformance8, (void**)&performance)))
		{
			MessageBox(hwnd, "Nie mo¿na utworzyæ obiektu IDirectMusicPerformance8!",
					"B³¹d!", MB_OK);
			return;
		}
		performance->InitAudio(NULL, NULL, hwnd, DMUS_APATH_SHARED_STEREOPLUSREVERB, 64,
								DMUS_AUDIOF_ALL, NULL);
		if (FAILED(CoCreateInstance(CLSID_DirectMusicLoader, NULL, CLSCTX_INPROC,
							IID_IDirectMusicLoader8, (void**)&loader)))
		{
			MessageBox(hwnd, "Nie mo¿na utworzyæ obiektu IDirectMusicLoader8!",
					"B³¹d!", MB_OK);
			return;
		}

		char pathStr[MAX_PATH];	
		WCHAR wcharStr[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, pathStr);
		MultiByteToWideChar(CP_ACP, 0, pathStr, -1, wcharStr, MAX_PATH);
		loader->SetSearchDirectory(GUID_DirectMusicAllTypes, wcharStr, FALSE);

		MultiByteToWideChar(CP_ACP, 0, filename, -1, wcharStr, MAX_PATH);
		if (FAILED(loader->LoadObjectFromFile(CLSID_DirectMusicSegment,
											IID_IDirectMusicSegment8,
											wcharStr,
											(void**)&segment)))
		{
			MessageBox(hwnd, "Nie znaleziono pliku audio!",
					"B³¹d!", MB_OK);
		}
		segment->Download(performance);
	}
};
HWND MDSound::hwnd;