#include <iostream>
#include "FFTfilter.h"

FFTfilter::FFTfilter()
{
}

FFTfilter::FFTfilter(const double* coeff_real, const double* coeff_imag, const int filter_order, const int FFT_size)
{
	//copy and setup filter parameters
	real_coeff = coeff_real;
	imag_coeff = coeff_imag;
	P = filter_order;
	N = FFT_size;
	//calculate the size of the frames to process to avoid time domain aliasing
	L = N - P + 1;
	//allocate space for the buffers used
	overlap = new double[N];
	in_fwd =  new double[N];
	out_bwd = new double[N];
	out_fwd = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N/2 + 1));
	in_bwd = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * (N/2 + 1));
	//setup fftw plans (see fftw documentation for more info)
	forward = fftw_plan_dft_r2c_1d(N, in_fwd, out_fwd, FFTW_MEASURE);
	backward = fftw_plan_dft_c2r_1d(N, in_bwd, out_bwd, FFTW_MEASURE);

	//clear buffers
	int i;
	for(i = 0; i < N/2 + 1; ++i)
	{
		overlap[i] = 0.0;
		in_fwd[i] = 0.0;
		out_bwd[i] = 0.0;
		in_bwd[i][0] = 0.0;
		in_bwd[i][1] = 0.0;
		out_fwd[i][0] = 0.0;
		out_fwd[i][1] = 0.0;
	}
	for(; i < N; ++i)
	{
		overlap[i] = 0.0;
		in_fwd[i] = 0.0;
		out_bwd[i] = 0.0;
	}
}

FFTfilter::~FFTfilter()
{
	delete[] overlap;
	delete[] in_fwd;
	delete[] out_bwd;
	fftw_free(out_fwd);
	fftw_free(in_bwd);
	fftw_destroy_plan(forward);
	fftw_destroy_plan(backward);
}

void FFTfilter::filter(double* input, double* output, const int length)
{
	//calculate the number of frames to process and the leftover segment
	int frames = length / L;
	int remainder = length % L;
	//j indexes which value from the input buffer should be copied, it should be incremented by the length passes to fft_convolve
	//l indexes which spot in the output buffer the next value is going to be copied (or added too)
	int j = 0, l = 0;
	
	if(frames == 0 || (frames == 1 && remainder == 0))
	{
		//convolve the frame
		fft_convolve(input, length);

		//copy data into output buffer
		int i;
		for(i = 0; i < length; ++i)
		{
			//add data to delay line which increases write pointer by 1
			output[i] = ((out_bwd[i] + overlap[i]) / N);
		}

		//copy overlapping output for next call
		for(int k = 0; k < P - 1; ++i, ++k)
		{
			overlap[k] = out_bwd[i];
		}
	}
	else if(frames == 1 && remainder > 0)
	{
		//filter the only subframe
		fft_convolve(input, L);
		j += L;

		//clear the output buffer as it will be used in calculations
		for(int i = 0; i < length; ++i)
		{
			output[i] = 0.0;
		}

		//copy the entire processed frame into the output buffer if the remainder is large enough
		if(remainder - (P - 1) >= 0)
		{
			for(int k = 0; k < N; ++k, ++l)
			{
				output[l] = (out_bwd[k] + overlap[k]) / N;
				//clear the overlap buffer so last subframe and remainder can use it
				overlap[k] = 0.0;
			}
			l -= (P - 1);
		}
		else //the extension from the frame will overlap with the "overlap" that needs to be stored for the next processed frame
		{
			int m, k;
			for(k = 0; k < L + remainder; ++k, ++l)
			{
				output[l] = (out_bwd[k] + overlap[k] )/ N;
				//clear the overlap buffer so it can be used right after
				overlap[k] = 0.0;
			}

			l -= remainder;

			//store the overlap
			for(m = 0; m < (P - 1) - remainder; ++m, ++k)
			{
				overlap[m] = out_bwd[k];
			}
		}

		//process the remainder
		//filter that sumbitch
		fft_convolve(&(input[j]), remainder);
		j += remainder;

		int i;
		//copy the convolved remainder into the output buffer
		for(i = 0; i < remainder; ++i, ++l)
		{
			output[l] += out_bwd[i] / N;
		}

		//copy the extension into the overlap buffer for the next call to processReplacing
		for(int k = 0; i < remainder + (P - 1); ++i, ++k)
		{
			overlap[k] += out_bwd[i];
		}
	}
	else //frame is greater than 2L and large enough to cause time-domain aliasing so split it into subframes and overlap-add them
	{
		//clear the output buffer as it will be used in calculations
		for(int i = 0; i < length; ++i)
		{
			output[i] = 0.0;
		}

		for(int i = 0; i < frames; ++i)
		{
			fft_convolve(&(input[j]), L);
			j += L;

			//copy data into output buffer taking overlap into account
			if(i == 0)
			{
				for(int k = 0; k < N; ++k, ++l)
				{
					output[l] = (out_bwd[k] + overlap[k]) / N;
					//clear the overlap buffer so last subframe and remainder can use it
					overlap[k] = 0.0;
				}
				l -= (P - 1);
			}
			//check to see if there's any overlap between the remainder's extension and the last subframes extension from convolution
			//if there is overlap then ensure the overlap buffer has the correct data for the next call the filter for continuous signals
			else if(i == frames - 1)
			{
				if(remainder - (P - 1) >= 0)
				{
					for(int k = 0; k < N; ++k, ++l)
					{
						output[l] += out_bwd[k] / N;
					}

					l -= (P - 1);
				}
				else
				{
					int m, k;
					for(k = 0; k < L + remainder; ++k, ++l)
					{
						output[l] += out_bwd[k] / N;
					}
					l -= remainder;

					//store the overlap
					for(m = 0; m < (P - 1) - remainder; ++m, ++k)
					{
						overlap[m] = out_bwd[k];
					}
				}
			}
			else
			{
				for(int k = 0; k < N; ++k, ++l)
				{
					output[l] += out_bwd[k] / N;
				}
				l -= (P - 1);
			}
		}
		//handle the remainder of the subframe if it exists
		if(remainder > 0)
		{
			//copy the data into the input buffer making sure to clear the old values to ensure proper zero padding
			fft_convolve(&(input[j]), remainder);
			j += remainder;

			int i;
			//copy the remainder into the output buffer
			for(i = 0; i < remainder; ++i, ++l)
			{
				output[l] += out_bwd[i] / N;
			}

			//copy the extension into the overlap buffer for the next call to processReplacing
			for(int k = 0; i < remainder + (P - 1); ++i, ++k)
			{
				overlap[k] += out_bwd[i];
			}
		}
	}
}

void FFTfilter::fft_convolve(const double* input, const int length)
{
	if(length > L)
	{
		std::cerr << "Buffer to big for convolution... Aborting" << std::endl;
		throw Bad_length();
	}

	int i;
	//copy the input into the fftw buffer
	for(i = 0; i < length; ++i)
	{
		in_fwd[i] = input[i];
	}
	//ensure the signal is padded with zeros
	for(; i < N; ++i)
	{
		in_fwd[i] = 0.0;
	}
	//FFT the zero-padded signal
	fftw_execute(forward);

	//multiply the signal FFT by the previously computed filter response FFT
	for(int k = 0; k < N/2 + 1; ++k)
	{
		//(a+bi)(c+di) = (ac-bd) + (ad+bc)i
		in_bwd[k][0] = (out_fwd[k][0] * real_coeff[k]) - (out_fwd[k][1] * imag_coeff[k]);
		in_bwd[k][1] = (out_fwd[k][0] * imag_coeff[k]) + (out_fwd[k][1] * real_coeff[k]);
	}

	//IFFT the convolved signal (don't foget it's unnormalized)
	fftw_execute(backward);
}

void FFTfilter::clearBuffer(void)
{
    for(int i = 0; i < N; ++i)
    {
        overlap[i] = 0.0;
    }
}
