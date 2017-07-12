#include "WavData.h"
#include <cstdio>
#include <string>

WavData::WavData(size_t sampleCount, unsigned int sampleRate, bool stereo)
	: nSamples(sampleCount), sampleRate(sampleRate), bStereo(stereo)
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
	wavFile = fopen(filename, "r");
	if (!wavFile) return false;

	// Read the header data.
	size_t nHdrBytes = fread(&hdr, 1, sizeof(WavHeader), wavFile);
	if (nHdrBytes < sizeof(WavHeader)) return false;

	// Make sure the header is valid and supported.
	if (!checkHeaderValid(hdr)) return false;

	return true;
}

bool WavData::saveFile(char * filename)
{
	return false;
}

size_t WavData::getSampleCount()
{
	return nSamples;
}

bool WavData::isStereo()
{
	return bStereo;
}

bool WavData::checkHeaderValid(WavHeader hdr)
{
	if (strncmp(hdr.riffChunkID, "RIFF", 4) != 0) return false;
	if (strncmp(hdr.riffFormat, "WAVE", 4) != 0) return false;
	if (strncmp(hdr.fmtSubchunk1ID, "fmt ", 4) != 0) return false;
}
