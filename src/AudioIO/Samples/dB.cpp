#include <qtaudioio.h>

short N::Acoustics::AmplitudeDB[88] =                           {
      0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,     0 ,
      0 ,     0 ,     0 ,     0 ,     0 ,     6 ,     7 ,     8 ,
      9 ,    10 ,    11 ,    13 ,    14 ,    16 ,    18 ,    20 ,
     22 ,    25 ,    28 ,    32 ,    35 ,    40 ,    45 ,    51 ,
     57 ,    64 ,    71 ,    80 ,    90 ,   101 ,   114 ,   128 ,
    142 ,   159 ,   179 ,   202 ,   227 ,   256 ,   284 ,   318 ,
    359 ,   405 ,   455 ,   512 ,   568 ,   638 ,   719 ,   881 ,
    911 ,  1024 ,  1137 ,  1276 ,  1438 ,  1622 ,  1823 ,  2048 ,
   2273 ,  2552 ,  2875 ,  3244 ,  3645 ,  4096 ,  4547 ,  5104 ,
   5751 ,  6488 ,  7291 ,  8192 ,  9093 , 10207 , 11502 , 12976 ,
  14582 , 16384 , 18350 , 20644 , 23429 , 26214 , 29491 , 32767
}                                                               ;

/******************************************************************************\
 *                                                                            *
 * Convert from decibels to a linear scale factor                             *
 *                                                                            *
 * Conversion table, db to linear, 87 dB --> 32767                            *
 *                                 86 dB --> 29491 (1 dB down = 0.5**1/6)     *
 *                                 ...                                        *
 *                                 81 dB --> 16384 (6 dB down = 0.5)          *
 *                                 ...                                        *
 *                                  0 dB -->     0                            *
 *                                                                            *
 * The just noticeable difference for a change in intensity of a vowel is     *
 * approximately 1 dB.  Thus all amplitudes are quantized to 1 dB steps.      *
 *                                                                            *
\******************************************************************************/

double N::Acoustics::toLinear(int dB)
{
  nKickOut ( dB <  0 , 0 )                                      ;
  nKickOut ( dB > 87 , 0 )                                      ;
  return ( ( (double) N::Acoustics::AmplitudeDB[dB] ) * 0.001 ) ;
}

double N::Acoustics::toLinear(int dB,double factor)
{
  nKickOut ( dB <  0 , 0 )                                       ;
  nKickOut ( dB > 87 , 0 )                                       ;
  factor *= 0.001                                                ;
  return ( ( (double) N::Acoustics::AmplitudeDB[dB] ) * factor ) ;
}
