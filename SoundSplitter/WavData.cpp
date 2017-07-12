#include "WavData.h"
#include <cstdio>
#include <string>

WavData::WavData(unsigned int numSamples, unsigned int sampleRate, unsigned short numChannels)
	: nSamples(numSamples), sampleRate(sampleRate), nChannels(numChannels)
{
}

WavData::WavData(const char * filename)
{
	loadFile(filename);
}

bool WavData::loadFile(const char * filename)
{
	FILE *wavFile;
	WavHeader hdr;

	// Open the file.
	wavFile = fopen(filename, "rb");
	if (!wavFile) return false;

	// Read the header data.
	size_t nHdrBytesRead = fread(&hdr, 1, sizeof(WavHeader), wavFile);
	if (nHdrBytesRead < sizeof(WavHeader)) return false;

	// Make sure the header is valid and supported.
	if (!checkHeaderValid(hdr)) return false;

	// Set our instance variables based on the header.
	nSamples = hdr.dataSubchunk2Size / (hdr.fmtNumChannels * hdr.fmtBitsPerSample / 8);
	sampleRate = hdr.fmtSampleRate;
	nChannels = hdr.fmtNumChannels;

	// Read the data into the samples array.
	samples = (short *)malloc(hdr.dataSubchunk2Size);
	size_t nDataBytesRead = fread(samples, 1, hdr.dataSubchunk2Size, wavFile);

	if (nDataBytesRead < hdr.dataSubchunk2Size) return false;

	return true;
}

bool WavData::saveFile(char * filename)
{
	return false;
}

size_t WavData::getNumSamples()
{
	return nSamples;
}

unsigned int WavData::getSampleRate()
{
	return sampleRate;
}

unsigned short WavData::getNumChannels()
{
	return nChannels;
}

bool WavData::checkHeaderValid(WavHeader hdr)
{
	if (strncmp(hdr.riffChunkID, "RIFF", 4) != 0) return false;
	if (strncmp(hdr.riffFormat, "WAVE", 4) != 0) return false;
	if (strncmp(hdr.fmtSubchunk1ID, "fmt ", 4) != 0) return false;
	if (strncmp(hdr.dataSubchunk2ID, "data", 4) != 0) return false;
	if (hdr.fmtSubchunk1Size != 16) return false;
	if (hdr.fmtAudioFormat != 1) return false;
	if (hdr.fmtBitsPerSample != 16) return false;
}
