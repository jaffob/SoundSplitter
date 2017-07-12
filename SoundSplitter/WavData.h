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

	unsigned int dataSubchunk2ID;
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

	size_t nSamples;
	unsigned int sampleRate;
	bool bStereo;

public:

	WavData(size_t sampleCount, unsigned int sampleRate, bool stereo);
	WavData(const char *filename);

	bool loadFile(const char *filename);
	bool saveFile(char *filename);

	size_t getSampleCount();
	bool isStereo();

	bool checkHeaderValid(WavHeader hdr);
};