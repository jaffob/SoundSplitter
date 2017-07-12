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
	if (nHdrBytesRead < sizeof(WavHeader))
	{
		fclose(wavFile);
		return false;
	}

	// Make sure the header is valid and supported.
	if (!checkHeaderValid(hdr))
	{
		fclose(wavFile);
		return false;
	}

	// Set our instance variables based on the header.
	nSamples = hdr.dataSubchunk2Size / (hdr.fmtNumChannels * hdr.fmtBitsPerSample / 8);
	sampleRate = hdr.fmtSampleRate;
	nChannels = hdr.fmtNumChannels;

	// Read the data into the samples array.
	samples = (short *)malloc(hdr.dataSubchunk2Size);
	size_t nDataBytesRead = fread(samples, 1, hdr.dataSubchunk2Size, wavFile);

	fclose(wavFile);
	return nDataBytesRead == hdr.dataSubchunk2Size;

}

bool WavData::saveFile(char * filename)
{
	FILE *wavFile;
	WavHeader hdr;

	// Set header values.

	strncpy(hdr.riffChunkID, "RIFF", 4);
	hdr.riffChunkSize = 36 + (nSamples * nChannels * 2);
	strncpy(hdr.riffFormat, "WAVE", 4);

	strncpy(hdr.fmtSubchunk1ID, "fmt ", 4);
	hdr.fmtSubchunk1Size = 16;
	hdr.fmtAudioFormat = 1;
	hdr.fmtNumChannels = nChannels;
	hdr.fmtSampleRate = sampleRate;
	hdr.fmtByteRate = sampleRate * nChannels * 2;
	hdr.fmtBlockAlign = nChannels * 2;
	hdr.fmtBitsPerSample = 16;

	strncpy(hdr.dataSubchunk2ID, "data", 4);
	hdr.dataSubchunk2Size = nSamples * nChannels * 2;

	// Create/open file for writing.
	wavFile = fopen(filename, "wb");
	if (!wavFile) return false;

	// Write the header.
	size_t nHdrBytesWritten = fwrite(&hdr, 1, sizeof(WavHeader), wavFile);
	if (nHdrBytesWritten < sizeof(WavHeader))
	{
		fclose(wavFile);
		return false;
	}

	// Write the sample data.
	size_t nDataBytesWritten = fwrite(samples, 1, hdr.dataSubchunk2Size, wavFile);

	fclose(wavFile);
	return nDataBytesWritten == hdr.dataSubchunk2Size;
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
