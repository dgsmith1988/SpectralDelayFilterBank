%David Smith
%05/27/2012

%This script simulates a "spectral delay filter bank". It calls upon a
%function called create5Filters which was specifically written for this.
%The effect is illustrated on a funk guitar vamp. The effect itself filters
%the original source into 5 different spectral bands and then delays them
%all relative to each other to create a 'ripple' in the sound. The
%filtering is done in a frame based way to simulate a real time
%implementation. Special thanks goes to Professor James Clark for the
%original idea and Joachim Thiemann for fielding questions and providing
%guidance.

%read in the test file
funkVamp = wavread('115 Stratosphere 01a A.wav');
Fs = 44100;
%generate filters using a previously written script, all filters are of
%even order to make adding overlapped signal segments easier
Hd = create5Filters;

%delays associated with each "region" in ms
delays = [0 200 250 275 300] * 10^-3;

%delays associtaed with each region but converted to samples
spectral_delays = round(delays*Fs);

%FFT blocksize
N = 4096;

%number of frames to test with
R = 47;

%number of filters to use (for debugging purposes)
Q = 5;

%size of chunks to test with (offset by the largest order filter, the offset correction delays will be less than this)
L = N - max(order(Hd(1:Q))) + 1;

%preallocate for storing time domain zero padded chunks
chunks = zeros(R, N);

%data used to store individual filter outputs
chunks_filtered = zeros(R, N);

%object for holding collective filter outputs
filtered_outputs{Q} = chunks_filtered;
%initialize object
for i = 1:Q
    filtered_outputs{i} = chunks_filtered;
end

%delay factor - based on the differences between the filter with the
%maximum group delay and the rest in order to synchronize the phases
W = zeros(Q, N);
delta = [0, 12, 46, 74, 74];
for i = 1:Q
    W(i, :) = exp(-1i * 2 * pi * delta(i) * [0:floor(N/2)-1 floor(-N/2):-1]' / N); 
    W(i,N/2+1) = real(W(i, N/2+1));
end

%FFT and store the impulse responses of the different filters
%don't forget to zero pad!
H_IMP = zeros(Q, N);
for i = 1:Q
    H_IMP(i, :) = fft([impz(Hd(i))', zeros(1, N - order(Hd(i)) - 1)], N);
end

%divide the original segment into frames
beg = 1;
ending = L;
for i = 1:R
    chunks(i, :) = [funkVamp(beg:ending)', zeros(1, N-L)] ;
    beg = beg + L;
    ending = ending + L;
end

%FFT THEM SUMBITCHES! (and multiply by FFT of filter impulse response and phase delay factor)
%i indexes which frame is being processed while j indexes which filter is being applied
for i = 1:R
   temp1 = fft(chunks(i, :), N);
   for j = 1:Q
    temp2 = temp1 .* H_IMP(j, :) .* W(j, :);
    temp3 = filtered_outputs{j};
    temp3(i, :) = ifft(temp2, N);
    filtered_outputs{j} = temp3;
   end
end

output_overlapped = zeros(Q, R*L+max(order(Hd))-1);

%add the overlapping segments together
for i = 1:R
    for j = 1:Q
        %fetch the corresponding filtered frame output
        temp1 = filtered_outputs{j};
        padded = [zeros(1, (i-1)*L) , temp1(i, :), zeros(1, (R-i)*L)];
        output_overlapped(j, :) = output_overlapped(j, :) + padded;
    end
end

%delay the spectral regions by the amounts selected and then sum them
final_output = zeros(1, size(output_overlapped, 2) + max(spectral_delays));
for i = 1:Q
    delay_diff = max(spectral_delays) - spectral_delays(i);
    final_output = final_output + [zeros(1, spectral_delays(i)), output_overlapped(i, :), zeros(1, delay_diff)];
end

sound(final_output, Fs);