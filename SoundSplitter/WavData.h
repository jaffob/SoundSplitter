#pragma once

#define _CRT_SECURE_NO_WARNINGS

/**
 * WAV file header struct. See details at http://soundfile.sapp.org/doc/WaveFormat/
 */
typedef struct
{
	char riffChunkID[4];
	unsigned int riffChunkSize;
	char riffFormat[4];

	char fmtSubchunk1ID[4];
	unsigned int fmtSubchunk1Size;
	unsigned short fmtAudioFormat;
	unsigned short fmtNumChannels;
	unsigned int fmtSampleRate;
	unsigned int fmtByteRate;
	unsigned short fmtBlockAlign;
	unsigned short fmtBitsPerSample;

	char dataSubchunk2ID[4];
	unsigned int dataSubchunk2Size;
} WavHeader;

/**
 * WavData class created by Jack Boffa.
 * Represents a .WAV file in memory. Must be 16-bit PCM wave,
 * but supports both mono and stereo.
 */
class WavData
{

public:

	short *samples;
	
private:

	unsigned int nSamples;
	unsigned int sampleRate;
	unsigned short nChannels;

public:

	WavData(unsigned int numSamples, unsigned int sampleRate, unsigned short numChannels);
	WavData(const char *filename);

	bool loadFile(const char *filename);
	bool saveFile(char *filename);

	unsigned int getNumSamples();
	unsigned int getSampleRate();
	unsigned short getNumChannels();

	bool checkHeaderValid(WavHeader hdr);
};