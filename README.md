# SpectralDelayFilterBank
This project is a VST plug-in for a "spectral delay filterbank" using the JUCE library.

The effect itself filters the original source into 5 different spectral bands and then delays them all relative to each other to create a 'ripple' in the sound. The filtering is done in using the FFTW library to perform fast convolution. An example is included on a funk guitar vamp. Special thanks goes to Professor James Clark for the original idea and Joachim Thiemann for fielding questions and providing guidance.

11/08/2017:
I have recieved reports that the plug-in no longer works but I have yet to recreate the issue myself. My guess is this is due to to the project requiring the use of the FFTW library. In the original compilation I used dynamic linking instead of static (before I knew the difference). There have also been numerous updates to the JUCE library since the original release. Accordingly, some of the other parts might be pretty out of date.
