# SpectralDelayFilterBank
A VST plug-in for a "spectral delay filterbank" using the JUCE library

The effect is illustrated on a funk guitar vamp. The effect itself filters the original source into 5 different spectral bands and then delays them all relative to each other to create a 'ripple' in the sound. The filtering is done in a frame based way to simulate a real time implementation. Special thanks goes to Professor James Clark for the original idea and Joachim Thiemann for fielding questions and providing guidance.

11/08/2017
I have recieved reports that the plug-in no longer works but I have yet to recreate it myself. I think this is due to it requiring the use of the FFTW library and in the original compilation I used dynamic linking instead of static. There have also been numerous updates to the JUCE since the original release so some of the other parts might be pretty out of date. When I get chance I'll look into resolving these issues.
