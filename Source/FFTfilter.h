#ifndef FFTFILTER_H_INCLUDED
#define FFTFILTER_H_INCLUDED
//header necessary for using the fftw library
#include "fftw3.h"

/*!
* \brief FFTfilter class for preforming filtering on signals
* \details This class uses the overlap-add method to preform convolution on a real-valued signal. It requires the fftw library to work properly. 
* \author Graham
* \date 7/7/2012
*/

class FFTfilter
{
public:

	/*!
	* Default constructor existing mainly to satisfy certain compilers
	* \author Graham
	* \date 7/7/2012
	*/

	FFTfilter();

	/*!
	* Constructor for an FFTfilter object. Make sure the size of the filter and the size of the FFT are such that time-domain aliasing doesn't occur.
	*
	* \author Graham
	* \date 7/7/2012
	*
	* \param coeff_real   Pointer to an array storing the real part of the FFT of the filter's IR.
	* \param coeff_imag   Pointer to an array storing the imaginary part of the FFT of the filter's IR 
	* \param filter_order The order of the filter.	 
	* \param FFT_size	   Size of the FFT.
	*/
	FFTfilter(const double* coeff_real, const double* coeff_imag, const int filter_order, const int FFT_size);

	/*!
	* Destructor
	*
	* \author Graham
	* \date 7/7/2012
	*/
	~FFTfilter();

	/*!
	* This method preforms the filtering (or more generally convolution) of the signal passed as input via the overlap-add algorithm. 
	* The output buffer gets overwritten during the process. It is possible to use the input buffer as the output buffer (the overwriting still applies).
	*
	* \author Graham
	* \date 7/7/2012
	*
	* \param [in,out] input  An array of doubles to be convolved.
	* \param [in,out] output The output of the convolution operation.
	* \param length		  The length of the array to be convolved.
	*/
	void filter(double* input, double* output, const int length);

	/*!
	* Clears the interal buffer used to store the overlap from convolution expansion. 
	* Use this method if you begin processing a new signal with the same FFTfilter.
	* \author Graham
	* \date 7/7/2012
	*/
	void clearBuffer();
private:

	/*!
	* The order of the filter
	*/
	int P;

	/*!
	* The length of the blocks to be processed to make sure time domain aliasing doesn't occur (N = L + P - 1)
	*/
	int L;

	/*!
	* The size of the FFT to use
	*/
	int N;

	/*!
	* An array containing the real part of the FFT of the filter's IR
	*/

	const double *real_coeff;

	/*!
	* An array containing the imaginary part of the FFT of the filter's IR
	*/

	const double *imag_coeff;

	/*!
	* An array where the extension from convolution will be stored for the next call to filter()
	*/

	double *overlap;

	/*!
	* An array to store the input to the fftw plan which computes the FFT. Only real valued signals can be used
	*/

	double *in_fwd;

	/*!
	* An array to store the output of the fftw plan which computes the IFFT. Only real valued signals are returned
	*/

	double *out_bwd;

	/*!
	* An array to store the output of the fftw plan which computes the FFT. Due to symmetry of real valued transforms only N/2 + 1 complex values are returned
	*/

	fftw_complex *out_fwd;

	/*!
	* An array to store the input to the fftw plan which computes the IFFT
	*/

	fftw_complex *in_bwd;

	/*!
	* An fftw plan used for computing the FFT of a signal
	*/

	fftw_plan forward;

	/*!
	* An fftw plan used for computing the IFFT of a signal
	*/

	fftw_plan backward;

	/*!
	* A function which convolves the input with the impulse response associated with the filter.
	* This is meant to be used on smaller chunks and is called as part of the overlap-add filtering
	* technique preformed by filter().
	* \author Graham
	* \date 7/7/2012
	*
	* \param input An array of doubles to be convolved
	* \param length The length of the array to be convolved 
	*/

	void fft_convolve(const double* input, const int length);
};

/*!
* \brief Exeception for fft_convolve()
* \details An exception thrown if the length of the input to fft_convolve is too big to allow expansion from convolution
*
* \author Graham
* \date 7/7/2012
*/

class Bad_length{};
#endif // FFTFILTER_H_INCLUDED
