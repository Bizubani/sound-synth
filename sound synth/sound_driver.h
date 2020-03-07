#pragma once

///Derived from the following code base on github
/*Copyright (C) 2018  Javidx9
	This program comes with ABSOLUTELY NO WARRANTY.
	This is free software, and you are welcome to redistribute it
	under certain conditions; See license for details. 
	Original works located at:
	https://www.github.com/onelonecoder
	https://www.onelonecoder.com
	https://www.youtube.com/javidx9
	*/

///A class that accesses the computer's sound hardware directly.
///Used with the synth to produce sound.

#pragma comment(lib, "winmm.lib")
#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>
#include <string>
#include <thread>
#include <atomic>
#include <condition_variable>
using namespace std;

#include <Windows.h>

const double PI = 2.0 * acos(0.0);

template<class T>
class SoundDriver
{
public:
	SoundDriver(wstring sOutputDevice, unsigned int nSampleRate = 44100, unsigned int nChannels = 1, unsigned int nBlocks = 8, unsigned int nBlockSamples = 512);
	bool create(wstring sOutputDevice, unsigned int nSamplRate = 44100, unsigned int nChannels = 1, unsigned int nBlocks = 8, unsgned int nBlockSamples = 512);
	bool destroy();
public:
	static vector<wstring> enumerate();
	void setUserFunction(double(*func)(double));
	double clip(double dSample, double dMax);

private:
	double(*m_userFunction)(double);
	//The meta data about the file to be played
	unsigned int m_nSampleRate;
	unsigned int m_nChannels;
	unsigned int m_nBlockCount;
	unsigned int m_nBlockSamples;
	unsigned int m_nBlockCurrent;

	//member variables for the sound to be played
	T* m_pBlockMemory;
	WAVEHDR* m_pWaveHeaders; // a pointer to a waveform audio buffer
	HWAVEOUT m_hwDevice; // the hardware device that is playing the sound?

	thread m_thread;
	atomic<bool> m_bReady;
	atomic<unsigned int> m_nBlockFree;
	condition_variable m_cvBlockNotZero;
	mutex m_muxBlockNotZero;
	
	atomic<double> m_dGlobalTime;

	void mainThread();
	void waveOutProc(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwParam1, DWORD dwParam2);
	static void CALLBACK waveOutProcWrap(HWAVEOUT hWaveOut, UINT uMsg, DWORD dwInstance, DWORD dwParam1, DWORD dwParam2);
};

