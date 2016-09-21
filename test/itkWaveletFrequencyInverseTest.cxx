/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#include <memory>
#include <string>
#include <cmath>
#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkWaveletFrequencyForward.h"
#include "itkWaveletFrequencyInverse.h"
#include "itkWaveletFrequencyFilterBankGenerator.h"
#include "itkHeldIsotropicWavelet.h"
#include "itkVowIsotropicWavelet.h"
#include "itkForwardFFTImageFilter.h"
#include "itkInverseFFTImageFilter.h"
#include <itkComplexToRealImageFilter.h>
#if ITK_VISUALIZE_TESTS != 0
#include "itkViewImage.h"
#endif
using namespace std;
using namespace itk;

template<unsigned int N>
int runWaveletFrequencyInverseTest( const std::string& inputImage,
    const std::string& outputImage,
    const unsigned int& inputLevels,
    const unsigned int& inputBands)
{
  const unsigned int dimension = N;
  typedef float                            PixelType;
  typedef itk::Image<PixelType, dimension> ImageType;
  typedef itk::ImageFileReader<ImageType>  ReaderType;
  typename ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(inputImage);
  reader->Update();
  reader->UpdateLargestPossibleRegion();

  // Perform FFT on input image.
  typedef itk::ForwardFFTImageFilter<ImageType> FFTFilterType;
  typename FFTFilterType::Pointer fftFilter = FFTFilterType::New();
  fftFilter->SetInput(reader->GetOutput());
  typedef typename FFTFilterType::OutputImageType ComplexImageType;

  // Set the WaveletFunctionType and the WaveletFilterBank
  typedef itk::HeldIsotropicWavelet<PixelType> WaveletFunctionType;
  // typedef itk::VowIsotropicWavelet<> WaveletFunctionType;
  typedef itk::WaveletFrequencyFilterBankGenerator< ComplexImageType, WaveletFunctionType> WaveletFilterBankType;
  typedef itk::WaveletFrequencyForward<ComplexImageType, ComplexImageType, WaveletFilterBankType> ForwardWaveletType;
  typename ForwardWaveletType::Pointer forwardWavelet = ForwardWaveletType::New();
  forwardWavelet->SetHighPassSubBands(inputBands);
  forwardWavelet->SetLevels(inputLevels);
  forwardWavelet->SetInput(fftFilter->GetOutput());
  forwardWavelet->Update();
  unsigned int noutputs = forwardWavelet->GetNumberOfOutputs();
  std::cout << "Ninputs: " << noutputs << '\n';
  for (unsigned int i = 0; i < noutputs; ++i)
    {
    std::cout << " Size of input: " << i << '\n';
    std::cout << forwardWavelet->GetOutput(i)->GetLargestPossibleRegion() << '\n';
    std::cout << forwardWavelet->GetOutput(i)->GetSpacing() << '\n';
    }

  // Inverse Wavelet Transform
  typedef itk::WaveletFrequencyInverse<ComplexImageType, ComplexImageType, WaveletFilterBankType> InverseWaveletType;
  typename InverseWaveletType::Pointer inverseWavelet = InverseWaveletType::New();
  inverseWavelet->SetHighPassSubBands(inputBands);
  inverseWavelet->SetLevels(inputLevels);
  inverseWavelet->SetInputs(forwardWavelet->GetOutputs());
  inverseWavelet->Update();
  typedef itk::InverseFFTImageFilter<ComplexImageType, ImageType> InverseFFTFilterType;
  typename InverseFFTFilterType::Pointer inverseFFT = InverseFFTFilterType::New();
  inverseFFT->SetInput(inverseWavelet->GetOutput());
  inverseFFT->Update();

  // Write Output for comparisson
  typedef itk::ImageFileWriter< ImageType > WriterType;
  typename WriterType::Pointer writer = WriterType::New();
  writer->SetFileName( outputImage );
  writer->SetInput( inverseFFT->GetOutput() );
  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error writing the WaveletInverse image: " << std::endl;
    std::cerr << error << std::endl;
    return EXIT_FAILURE;
    }
#if ITK_VISUALIZE_TESTS != 0
  Testing::ViewImage(reader->GetOutput(), "Original");
  Testing::ViewImage(inverseFFT->GetOutput(), "InverseWavelet");
#endif

  return EXIT_SUCCESS;
}

int itkWaveletFrequencyInverseTest(int argc, char *argv[])
{
  if( argc < 5 || argc > 6 )
    {
    std::cerr << "Usage: " << argv[0] <<
      " inputImage outputImage inputLevels inputBands [dimension]" << std::endl;
    return EXIT_FAILURE;
    }
  const string inputImage  = argv[1];
  const string outputImage = argv[2];
  const unsigned int inputLevels = atoi(argv[3]);
  const unsigned int inputBands  = atoi(argv[4]);

  unsigned int dimension = 3;
  if( argc == 6 )
  {
    dimension = atoi( argv[5] );
  }

  if( dimension == 2 )
  {
    return runWaveletFrequencyInverseTest<2>(inputImage, outputImage, inputLevels, inputBands);
  }
  else if( dimension == 3 )
  {
    return runWaveletFrequencyInverseTest<3>(inputImage, outputImage, inputLevels, inputBands);
  }
  else
  {
    std::cerr << "Error: only 2 or 3 dimensions allowed, " << dimension << " selected." << std::endl;
    return EXIT_FAILURE;
  }
}
