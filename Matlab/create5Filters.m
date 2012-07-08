%David Smith
%05/07/2012

function Hd = create5Filters
%This function creates 5 different filters based on specifications
%determined through experimentation in Max/MSP to be used in a spectral
%delay filter bank focused on guitar processing. The function could easily
%be adapted to different instruments by changing the specifications of the
%filters.

%General variables used by the different filter specifications in order to
%save memory
FS = 44100;     %Sampling Frequency
Fpass1 = 0;     %Passband 1 Frequency
Fpass2 = 0;     %Passband 2 Frequency
Fstop1 = 0;     %Stopband 1 Frequency
Fstop2 = 0;     %Stopband 2 Frequency
Astop1 = 60;    %Stopband 1 Attenuation (dB)
Astop2 = 60;    %Stopband 2 Attenuation (dB)
Apass = 1;      %Passband Ripple (dB)
h = fdesign.lowpass; %object for storing specifcations

%F0 specifications
Fpass1 = 600;   %Passband Frequency
Fstop1 = 1000;   %Stopband Frequency
Apass = 1;      %Passband Ripple (dB)
Astop1 = 60;    %Stopband Attenuation (dB)
h = fdesign.lowpass('fp,fst,ap,ast', Fpass1, Fstop1, Apass, Astop1, FS);
Hd(1) = design(h, 'equiripple', 'FilterStructure', 'fftfir', 'MinOrder', 'any','StopbandShape', 'flat');

%F1 specifications
Fstop1 = 100;    %Stopband 1 Frequency
Fpass1 = 500;   %Passband 1 Frequency
Fpass2 = 900;   %Passband 2 Frequency
Fstop2 = 8000;  %Stopband 2 Frequency
Astop1 = 60;    %Stopband 1 Attenuation (dB)
Astop2 = 60;    %Stopband 2 Attenuation (dB)
Apass = 1;      %Passband Ripple (dB)
h = fdesign.bandpass('Fst1,Fp1,Fp2,Fst2,Ast1,Ap,Ast2',  Fstop1, Fpass1, Fpass2,Fstop2, Astop1, Apass, Astop2, FS);
Hd(2) = design(h, 'equiripple', 'FilterStructure', 'fftfir', 'MinOrder', 'any');

%F2 specifications
Fstop1 = 200;    %Stopband 1 Frequency
Fpass1 = 900;   %Passband 1 Frequency
Fpass2 = 2000;   %Passband 2 Frequency
Fstop2 = 8000;  %Stopband 2 Frequency
Astop1 = 60;    %Stopband 1 Attenuation (dB)
Astop2 = 60;    %Stopband 2 Attenuation (dB)
Apass = 1;      %Passband Ripple (dB)
h = fdesign.bandpass('Fst1,Fp1,Fp2,Fst2,Ast1,Ap,Ast2',  Fstop1, Fpass1, Fpass2,Fstop2, Astop1, Apass, Astop2, FS);
Hd(3) = design(h, 'equiripple', 'FilterStructure', 'fftfir', 'MinOrder', 'any');

%F3 specifications
Fstop1 = 900;    %Stopband 1 Frequency
Fpass1 = 2000;   %Passband 1 Frequency
Fpass2 = 3000;   %Passband 2 Frequency
Fstop2 = 10000;  %Stopband 2 Frequency
Astop1 = 60;    %Stopband 1 Attenuation (dB)
Astop2 = 60;    %Stopband 2 Attenuation (dB)
Apass = 1;      %Passband Ripple (dB)
h = fdesign.bandpass('Fst1,Fp1,Fp2,Fst2,Ast1,Ap,Ast2',  Fstop1, Fpass1, Fpass2,Fstop2, Astop1, Apass, Astop2, FS);
Hd(4) = design(h, 'equiripple', 'FilterStructure', 'fftfir', 'MinOrder', 'even');

%F4 specifications
Fstop1 = 2000;    %Stopband 1 Frequency
Fpass1 = 3000;   %Passband 1 Frequency
Astop1 = 60;    %Stopband 1 Attenuation (dB)
Apass = 1;      %Passband Ripple (dB)
h = fdesign.highpass('Fst,Fp,Ast,Ap',  Fstop1, Fpass1, Astop1, Apass, FS);
Hd(5) = design(h, 'equiripple', 'FilterStructure', 'fftfir', 'MinOrder', 'any');

%hfvt = fvtool(Hd);
%legend(hfvt, 'F0 response', 'F1 response', 'F2 response', 'F3 response', 'F4 response');