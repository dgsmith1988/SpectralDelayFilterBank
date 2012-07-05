/*
==============================================================================

This file was auto-generated by the Jucer!

It contains the basic startup code for a Juce application.

==============================================================================
*/

#ifndef __PLUGINPROCESSOR_H_C0FD7140__
#define __PLUGINPROCESSOR_H_C0FD7140__

#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"
#include "CircularBuffer.h"
#include "FFTfilter.h"

//==============================================================================
/**
*/
class SpectralDelayPluginAudioProcessor  : public AudioProcessor
{
public:
	//==============================================================================
	SpectralDelayPluginAudioProcessor();
	~SpectralDelayPluginAudioProcessor();

	//==============================================================================
	void prepareToPlay (double sampleRate, int samplesPerBlock);
	void releaseResources();

	void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages);

	//==============================================================================
	AudioProcessorEditor* createEditor();
	bool hasEditor() const;

	//==============================================================================
	const String getName() const;

	int getNumParameters();

	float getParameter (int index);
	void setParameter (int index, float newValue);

	const String getParameterName (int index);
	const String getParameterText (int index);

	const String getInputChannelName (int channelIndex) const;
	const String getOutputChannelName (int channelIndex) const;
	bool isInputChannelStereoPair (int index) const;
	bool isOutputChannelStereoPair (int index) const;

	bool acceptsMidi() const;
	bool producesMidi() const;

	//==============================================================================
	int getNumPrograms();
	int getCurrentProgram();
	void setCurrentProgram (int index);
	const String getProgramName (int index);
	void changeProgramName (int index, const String& newName);

	//==============================================================================
	void getStateInformation (MemoryBlock& destData);
	void setStateInformation (const void* data, int sizeInBytes);

private:
	//==============================================================================
	int N;
	//int Fs = 44100;
	//delay line length/max delay in samples
	int maxDelay;
	int numFilters;

	//vectors are used here as the goal is to evenutally let users give an arbitrary impulse response to be convolved with
	//vector to hold pointers to filters
	std::vector<ScopedPointer<FFTfilter>> filterVector;
	//vector to hold pointers to delay lines
	std::vector<ScopedPointer<CircularBuffer<double>>> delayLineVector;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SpectralDelayPluginAudioProcessor);
};

#endif  // __PLUGINPROCESSOR_H_C0FD7140__
