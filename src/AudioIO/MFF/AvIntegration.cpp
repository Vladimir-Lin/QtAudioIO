#include <qtaudioio.h>

int N::Acoustics::AvFormat(int sampleFormat)
{
  switch (sampleFormat)                                    {
    case AV_SAMPLE_FMT_NONE : return N::Acoustics::Guess   ;
    case AV_SAMPLE_FMT_U8   : return N::Acoustics::UInt8   ;
    case AV_SAMPLE_FMT_S16  : return N::Acoustics::Int16   ;
    case AV_SAMPLE_FMT_S32  : return N::Acoustics::Int32   ;
    case AV_SAMPLE_FMT_FLT  : return N::Acoustics::Float32 ;
    case AV_SAMPLE_FMT_DBL  : return N::Acoustics::Double  ;
    case AV_SAMPLE_FMT_U8P  : return N::Acoustics::UInt8   ;
    case AV_SAMPLE_FMT_S16P : return N::Acoustics::Int16   ;
    case AV_SAMPLE_FMT_S32P : return N::Acoustics::Int32   ;
    case AV_SAMPLE_FMT_FLTP : return N::Acoustics::Float32 ;
    case AV_SAMPLE_FMT_DBLP : return N::Acoustics::Double  ;
    case AV_SAMPLE_FMT_NB   : return N::Acoustics::Guess   ;
  }                                                        ;
  return N::Acoustics::Guess                               ;
}

int N::Acoustics::AvBits(int sampleFormat)
{
  switch (sampleFormat)                                 {
    case AV_SAMPLE_FMT_NONE : return 0                  ;
    case AV_SAMPLE_FMT_U8   : return 8                  ;
    case AV_SAMPLE_FMT_S16  : return 16                 ;
    case AV_SAMPLE_FMT_S32  : return 32                 ;
    case AV_SAMPLE_FMT_FLT  : return 32                 ;
    case AV_SAMPLE_FMT_DBL  : return sizeof(double) * 8 ;
    case AV_SAMPLE_FMT_U8P  : return 8                  ;
    case AV_SAMPLE_FMT_S16P : return 16                 ;
    case AV_SAMPLE_FMT_S32P : return 32                 ;
    case AV_SAMPLE_FMT_FLTP : return 32                 ;
    case AV_SAMPLE_FMT_DBLP : return sizeof(double) * 8 ;
    case AV_SAMPLE_FMT_NB   : return 0                  ;
  }                                                     ;
  return 0                                              ;
}
