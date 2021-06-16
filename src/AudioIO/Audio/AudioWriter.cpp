#include <qtaudioio.h>
#include "nAudioDataPrivate.hpp"

N::AudioWriter:: AudioWriter (QObject * parent)
               : QIODevice   (          parent)
               , AudioData   (                )
               , AvEncoder   (                )
               , Thread      (0,false         )
               , Output      (NULL            )
               , method      (Indexing        )
               , accessIndex (0               )
               , going       (false           )
               , WriterName  (""              )
               , interval    (250             )
               , packed      (0               )
{
  WidgetClass                                                               ;
  nConnect ( this , SIGNAL(AlterEmpty   ()) , this , SLOT(RelayEmpty   ())) ;
  nConnect ( this , SIGNAL(AlterStarted ()) , this , SLOT(RelayStarted ())) ;
  nConnect ( this , SIGNAL(AlterFinished()) , this , SLOT(RelayFinished())) ;
}

N::AudioWriter::~AudioWriter (void)
{
}

QByteArray & N::AudioWriter::add(char * dat,int len)
{
  mutex      . lock   (                   ) ;
  AudioData :: add    ( (char *)dat , len ) ;
  mutex      . unlock (                   ) ;
  return ME                                 ;
}

int N::AudioWriter::setIndex(int i)
{
  accessIndex = i    ;
  return accessIndex ;
}

int N::AudioWriter::currentIndex (void)
{
  return accessIndex ;
}

TUID N::AudioWriter::currentMS(void)
{
  TUID MS = accessIndex  ;
  MS *= 1000             ;
  MS /= BytesPerSecond() ;
  return MS              ;
}

void N::AudioWriter::setMethod(ReadMethod m)
{
  method = m ;
}

N::AudioWriter::ReadMethod N::AudioWriter::Method(void)
{
  return method ;
}

qint64 N::AudioWriter::take(int pos,int len)
{
  mutex . lock   ( )                                     ;
  RawFormat * rf = (RawFormat *)data()                   ;
  pos += sizeof(RawFormat)                               ;
  remove ( pos , len )                                   ;
  rf->DataSize = AudioData::size ( ) - sizeof(RawFormat) ;
  mutex . unlock ( )                                     ;
  return rf->DataSize                                    ;
}

qint64 N::AudioWriter::readData(char * dat,qint64 maxSize)
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

qint64 N::AudioWriter::writeData(const char * dat,qint64 maxSize)
{
  nKickOut       ( ! proceed() , 0   ) ;
  mutex . lock   (                   ) ;
  AudioData::add ((char *)dat,maxSize) ;
  mutex . unlock (                   ) ;
  return maxSize                       ;
}

bool N::AudioWriter::prepare(void)
{
  nKickOut ( codec == AV_CODEC_ID_NONE                , false )         ;
  nKickOut ( IsNull(Output)                           , false )         ;
  int fmt = -1                                                          ;
  switch (Format())                                                     {
    case Acoustics::Guess   : fmt = AV_SAMPLE_FMT_NONE ; break          ;
    case Acoustics::UInt8   : fmt = AV_SAMPLE_FMT_U8   ; break          ;
    case Acoustics::Int16   : fmt = AV_SAMPLE_FMT_S16  ; break          ;
    case Acoustics::Int32   : fmt = AV_SAMPLE_FMT_S32  ; break          ;
    case Acoustics::Float32 : fmt = AV_SAMPLE_FMT_FLT  ; break          ;
    case Acoustics::Double  : fmt = AV_SAMPLE_FMT_DBL  ; break          ;
  }                                                                     ;
  nKickOut ( fmt <= 0                                 , false )         ;
  nKickOut ( !OpenAudio ( codec , Rate() , Channels() , fmt ) , false ) ;
  return Output -> open ( QIODevice::WriteOnly | QIODevice::Truncate )  ;
}

int N::AudioWriter::encode(QIODevice & IO,bool flush)
{
  int as = AudioSize()                       ;
  int cs = 0                                 ;
  nIfSafe(context)                           {
    if (as>0)                                {
      unsigned char * src                    ;
      unsigned char * cps                    ;
      cps = new unsigned char [as]           ;
      mutex . lock   ( )                     ;
      src = (unsigned char *)index(0)        ;
      memcpy ( cps , src , as )              ;
      mutex . unlock ( )                     ;
      as  = Encode ( IO , as , cps , flush ) ;
      cs  = as                               ;
      delete [] cps                          ;
    }                                        ;
  }                                          ;
  if (as>0 && !flush)                        {
    mutex . lock   ( )                       ;
    takeAt ( 0 , as )                        ;
    mutex . unlock ( )                       ;
  }                                          ;
  return cs                                  ;
}

bool N::AudioWriter::cease(void)
{
  AvEncoder :: Close ( ) ;
  nIfSafe(Output)        {
    Output->close()      ;
  }                      ;
  going = false          ;
  return true            ;
}

bool N::AudioWriter::Lock(void)
{
  mutex . lock ( ) ;
  return true      ;
}

bool N::AudioWriter::Unlock(void)
{
  mutex . unlock ( ) ;
  return true        ;
}

bool N::AudioWriter::fresh(void)
{
  return true ;
}

bool N::AudioWriter::demolish(void)
{
  going = false ;
  return true   ;
}

void N::AudioWriter::setName(QString n)
{
  WriterName = n ;
}

QString N::AudioWriter::name(void) const
{
  return WriterName ;
}

void N::AudioWriter::setInterval(int value)
{
  interval = value ;
}

int N::AudioWriter::Interval(void) const
{
  return interval ;
}

void N::AudioWriter::addLength(int length)
{ Q_UNUSED ( length ) ;
}

void N::AudioWriter::addCompressed(int length)
{
  packed += length ;
}

void N::AudioWriter::run(void)
{
  going = true                                              ;
  if (!prepare())                                           {
    Error(1001,tr("%1 can not establish").arg(WriterName) ) ;
    going = false                                           ;
    return                                                  ;
  }                                                         ;
  started ( )                                               ;
  TUID total = 0                                            ;
  int  fs = context -> frame_size                           ;
  int  fb = AvEncoder::BytesPerFrame * fs                   ;
  while (proceed())                                         {
    if (AudioSize()>=fb)                                    {
      total += encode  ( *Output , false  )                 ;
      progress         ( total   , packed )                 ;
    }                                                       ;
    Time :: msleep     ( interval         )                 ;
  }                                                         ;
  encode   ( *Output , true )                               ;
  Time :: msleep ( 100 )                                    ;
  cease    (                )                               ;
  finished (                )                               ;
  going = false                                             ;
}

void N::AudioWriter::Error(int error,QString message)
{ Q_UNUSED ( error   ) ;
  Q_UNUSED ( message ) ;
}

void N::AudioWriter::started(void)
{
  emit AlterStarted ( ) ;
}

bool N::AudioWriter::proceed(void)
{
  return going ;
}

void N::AudioWriter::finished(void)
{
  emit AlterFinished ( ) ;
}

void N::AudioWriter::progress(TUID current,TUID total)
{ Q_UNUSED ( current ) ;
  Q_UNUSED ( total   ) ;
}

void N::AudioWriter::RelayStarted(void)
{
  emit Started ( ) ;
}

void N::AudioWriter::RelayFinished(void)
{
  emit Finished (      ) ;
  emit Finished ( this ) ;
}

void N::AudioWriter::RelayEmpty(void)
{
  emit Empty ( ) ;
}
