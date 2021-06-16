#include <qtaudioio.h>
#include "nAudioDataPrivate.hpp"

N::AudioReader:: AudioReader   (QObject * parent)
               : QIODevice     (          parent)
               , AudioData     (                )
               , AvDecoder     (                )
               , method        (Indexing        )
               , accessIndex   (0               )
               , going         (false           )
               , PostSequences (NULL            )
{
  nConnect ( this , SIGNAL(AlterEmpty   ()) , this , SLOT(RelayEmpty   ())) ;
  nConnect ( this , SIGNAL(AlterStarted ()) , this , SLOT(RelayStarted ())) ;
  nConnect ( this , SIGNAL(AlterFinished()) , this , SLOT(RelayFinished())) ;
}

N::AudioReader::~AudioReader (void)
{
}

QByteArray & N::AudioReader::add(char * dat,int len)
{
  mutex      . lock   (                   ) ;
  AudioData :: add    ( (char *)dat , len ) ;
  mutex      . unlock (                   ) ;
  return ME                                 ;
}

int N::AudioReader::setIndex(int i)
{
  accessIndex = i    ;
  return accessIndex ;
}

int N::AudioReader::currentIndex (void)
{
  return accessIndex ;
}

TUID N::AudioReader::currentMS(void)
{
  TUID MS = accessIndex  ;
  MS *= 1000             ;
  MS /= BytesPerSecond() ;
  return MS              ;
}

void N::AudioReader::setMethod(ReadMethod m)
{
  method = m ;
}

N::AudioReader::ReadMethod N::AudioReader::Method(void)
{
  return method ;
}

void N::AudioReader::Error(int error,QString message)
{
}

void N::AudioReader::started(void)
{
  emit AlterStarted ( ) ;
}

bool N::AudioReader::proceed(void)
{
  return going ;
}

void N::AudioReader::finished(void)
{
  emit AlterFinished ( )            ;
  if (IsNull(PostSequences)) return ;
  VarArgs args                      ;
  args << 40001                     ;
  args << "AudioReader"             ;
  args << "Finished"                ;
  args << VoidVariant ( this )      ;
  (*PostSequences) << args          ;
}

void N::AudioReader::progress(int current,int total)
{
}

bool N::AudioReader::Lock(void)
{
  mutex . lock ( ) ;
  return true      ;
}

bool N::AudioReader::Unlock(void)
{
  mutex . unlock ( ) ;
  return true        ;
}

bool N::AudioReader::fresh(void)
{
  return true ;
}

bool N::AudioReader::demolish(void)
{
  going = false ;
  return true   ;
}

qint64 N::AudioReader::take(int pos,int len)
{
  mutex . lock   ( )                                     ;
  RawFormat * rf = (RawFormat *)data()                   ;
  pos += sizeof(RawFormat)                               ;
  remove ( pos , len )                                   ;
  rf->DataSize = AudioData::size ( ) - sizeof(RawFormat) ;
  mutex . unlock ( )                                     ;
  return rf->DataSize                                    ;
}

qint64 N::AudioReader::readData(char * dat,qint64 maxSize)
{
  nKickOut ( maxSize <= 0 , 0 )             ;
  nKickOut ( ! proceed()  , 0 )             ;
  mutex . lock   ( )                        ;
  char * p = (char *)AudioData::data()      ;
  int hs = sizeof(RawFormat)                ;
  int ss = AudioData::size ( )              ;
  int as = ss                               ;
  int fs = maxSize                          ;
  if (ss>0)                                 {
    ss -= hs                                ;
    if (ss>0)                               {
      switch (method)                       {
        case Removal                        :
          if (maxSize>ss) maxSize = ss      ;
          if (maxSize>0)                    {
            RawFormat * rf = (RawFormat *)p ;
            p += hs                         ;
            memcpy ( dat , p  , maxSize )   ;
            remove ( hs       , maxSize )   ;
            rf->DataSize -= maxSize         ;
            fs    -= maxSize                ;
            if (fs>0)                       {
              dat += maxSize                ;
              memset ( dat , 0 , fs )       ;
            }                               ;
          } else maxSize = 0                ;
        break                               ;
        case Indexing                       :
          ss -= accessIndex                 ;
          if (maxSize>ss) maxSize = ss      ;
          if (maxSize>0)                    {
            p           += hs               ;
            p           += accessIndex      ;
            accessIndex += maxSize          ;
            memcpy ( dat , p  , maxSize )   ;
            fs    -= maxSize                ;
            if (fs>0)                       {
              dat += maxSize                ;
              memset ( dat , 0 , fs )       ;
            }                               ;
          } else maxSize = 0                ;
          progress ( accessIndex , as )     ;
        break                               ;
      }                                     ;
    } else maxSize = 0                      ;
  } else maxSize = 0                        ;
  mutex . unlock ( )                        ;
  if (maxSize<=0) emit AlterEmpty ( )       ;
  return maxSize                            ;
}

qint64 N::AudioReader::writeData(const char * dat,qint64 maxSize)
{
  nKickOut ( ! proceed() , 0 ) ;
  mutex . lock   ( )           ;
  add((char *)dat,maxSize)     ;
  mutex . unlock ( )           ;
  return maxSize               ;
}

bool N::AudioReader::Peep(void)
{
  nKickOut ( !StartAudio(Filename) , false )     ;
  int CHANNELS = AvDecoder :: Channels       ( ) ;
  int SAMPLE   = AvDecoder :: AvSampleFormat ( ) ;
  int BITS     = Acoustics :: AvBits   (SAMPLE)  ;
  int FORMAT   = Acoustics :: AvFormat (SAMPLE)  ;
  AvDecoder :: Initialize ( &Packet )            ;
  AudioData :: reset      (         )            ;
  AudioData :: set                               (
    AvDecoder :: AvSampleRate ( )                ,
    BITS                                         ,
    CHANNELS                                     ,
    (N::Acoustics::SampleFormat)FORMAT         ) ;
  AvDecoder :: Close ( )                         ;
  return true                                    ;
}

bool N::AudioReader::prepare(void)
{
  going = false                                  ;
  nKickOut ( !StartAudio(Filename) , false )     ;
  int CHANNELS = AvDecoder :: Channels       ( ) ;
  int SAMPLE   = AvDecoder :: AvSampleFormat ( ) ;
  int BITS     = Acoustics :: AvBits   (SAMPLE)  ;
  int FORMAT   = Acoustics :: AvFormat (SAMPLE)  ;
  AvDecoder :: Initialize ( &Packet )            ;
  AudioData :: reset      (         )            ;
  AudioData :: set                               (
    AvDecoder :: AvSampleRate ( )                ,
    BITS                                         ,
    CHANNELS                                     ,
    (N::Acoustics::SampleFormat)FORMAT         ) ;
  going = true                                   ;
  return true                                    ;
}

bool N::AudioReader::decode(void)
{
  int Ret = ReadPacket(Packet)                                 ;
  switch (Ret)                                                 {
    case AVERROR_BUFFER_TOO_SMALL                              :
    case AVERROR_EXTERNAL                                      :
    case AVERROR_BUG2                                          :
    case AVERROR_BSF_NOT_FOUND                                 :
    case AVERROR_BUG                                           :
    case AVERROR_DECODER_NOT_FOUND                             :
    case AVERROR_DEMUXER_NOT_FOUND                             :
    case AVERROR_ENCODER_NOT_FOUND                             :
    case AVERROR_EXIT                                          :
    case AVERROR_FILTER_NOT_FOUND                              :
    case AVERROR_INVALIDDATA                                   :
    case AVERROR_MUXER_NOT_FOUND                               :
    case AVERROR_OPTION_NOT_FOUND                              :
    case AVERROR_PATCHWELCOME                                  :
    case AVERROR_PROTOCOL_NOT_FOUND                            :
    case AVERROR_STREAM_NOT_FOUND                              :
    case AVERROR_UNKNOWN                                       :
    case AVERROR_EXPERIMENTAL                                  :
    case AVERROR_EOF                                           :
      going = false                                            ;
    break                                                      ;
    default                                                    :
      if (Ret<0) going = false ;                            else
      if ( AvDecoder :: isAudioChannel ( Packet ) )            {
        AvDecoder :: DecodeAudio ( Packet , *this )            ;
      }                                                        ;
    break                                                      ;
  }                                                            ;
  AvDecoder :: Release ( &Packet )                             ;
  return going                                                 ;
}

bool N::AudioReader::cease(void)
{
  AvDecoder :: Close ( ) ;
  return true            ;
}

void N::AudioReader::RelayStarted(void)
{
  emit Started ( ) ;
}

void N::AudioReader::RelayFinished(void)
{
  emit Finished (      ) ;
  emit Finished ( this ) ;
}

void N::AudioReader::RelayEmpty(void)
{
  emit Empty ( ) ;
}
