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
#ifndef itkSimoncelliIsotropicWavelet_h
#define itkSimoncelliIsotropicWavelet_h

#include <itkIsotropicWaveletFrequencyFunction.h>

namespace itk
{
/** \class SimoncelliIsotropicWavelet
 *
 * Simoncelli Wavelet
 *
 * Implement function in frequency space.
 *
 \f{equation}
   h(\omega) =
     \begin{cases}
     \begin{aligned}
       &\cos( \frac{\pi}{2} \log_2\frac{2\omega}{\pi}) , &\omega \in ]\frac{\pi}{4} , \pi] \\
       &0, &\text{otherwise}
     \end{aligned}
     \end{cases}
 \f{equation}
 *
 * Based on:
 * J. Portilla and E. P. Simoncelli, “A parametric texture model based on
 * joint statistics of complex wavelet coefficients,”
 * Int. J. Computer Vision vol. 40, no. 1, pp. 49–70, 2000.
 *
 * \ingroup SpatialFunctions
 * \ingroup IsotropicWavelets
 */
template< typename TFunctionValue = double,
  unsigned int VImageDimension    = 3,
  typename TInput = Point< SpacePrecisionType, VImageDimension > >
class SimoncelliIsotropicWavelet:
  public IsotropicWaveletFrequencyFunction< TFunctionValue, VImageDimension, TInput >
{
public:
  /** Standard class typedefs. */
  typedef SimoncelliIsotropicWavelet                                                   Self;
  typedef IsotropicWaveletFrequencyFunction< TFunctionValue, VImageDimension, TInput > Superclass;
  typedef SmartPointer< Self >                                                         Pointer;
  typedef SmartPointer< const Self >                                                   ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(SimoncelliIsotropicWavelet, SpatialFunction);

  /** Input type for the function. */
  typedef typename Superclass::InputType InputType;

  /** FunctionValue type for the function. */
  typedef typename Superclass::FunctionValueType FunctionValueType;

  /** Evaluate the function */
  FunctionValueType EvaluateMagnitude(const FunctionValueType& freq_norm_in_hz) const ITK_OVERRIDE;

protected:
  SimoncelliIsotropicWavelet();
  virtual ~SimoncelliIsotropicWavelet();
  void PrintSelf(std::ostream & os, Indent indent) const ITK_OVERRIDE;

private:
  SimoncelliIsotropicWavelet(const Self &) ITK_DELETE_FUNCTION;
  void operator=(const Self &) ITK_DELETE_FUNCTION;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkSimoncelliIsotropicWavelet.hxx"
#endif

#endif
