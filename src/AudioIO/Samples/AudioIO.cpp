#include <qtaudioio.h>
#include "nAudioDataPrivate.hpp"

//#define CIOSDEBUG

#ifdef CIOSDEBUG
#include <Foundation>
#define XDEBUG(msg) if (qPlan->Verbose>=50) qPlan -> Debug ( msg )
#else
#define XDEBUG(msg)
#endif

N::AudioIO:: AudioIO            (QObject * parent)
           : QIODevice          (          parent)
           , AudioData          (                )
           , CiosAudio::Conduit (                )
           , method             (Indexing        )
           , accessIndex        (0               )
           , BootTrigger        (false           )
           , autoStop           (true            )
           , totalRead          (0               )
           , totalWritten       (0               )
{
}

N::AudioIO:: AudioIO            (const AudioData & other,QObject * parent)
           : QIODevice          (                                  parent)
           , AudioData          (                  other                 )
           , CiosAudio::Conduit (                                        )
           , method             (Indexing                                )
           , accessIndex        (0                                       )
           , BootTrigger        (false                                   )
           , autoStop           (true                                    )
           , totalRead          (0                                       )
           , totalWritten       (0                                       )
{
}

N::AudioIO:: AudioIO            (const AudioIO & io)
           : QIODevice          (io.parent()       )
           , AudioData          (                  )
           , CiosAudio::Conduit (                  )
           , method             (Indexing          )
           , accessIndex        (0                 )
           , BootTrigger        (false             )
           , autoStop           (true              )
           , totalRead          (0                 )
           , totalWritten       (0                 )
{
}

N::AudioIO::~AudioIO (void)
{
}

N::AudioIO & N::AudioIO::operator = (const AudioIO & io)
{
  nMemberCopy ( io , method      ) ;
  nMemberCopy ( io , accessIndex ) ;
  nMemberCopy ( io , BootTrigger ) ;
  return ME                        ;
}

int N::AudioIO::setIndex(int i)
{
  accessIndex = i    ;
  return accessIndex ;
}

int N::AudioIO::currentIndex (void)
{
  return accessIndex ;
}

TUID N::AudioIO::currentMS(void)
{
  TUID MS = accessIndex  ;
  MS *= 1000             ;
  MS /= BytesPerSecond() ;
  return MS              ;
}

void N::AudioIO::setMethod(ReadMethod m)
{
  method = m ;
}

N::AudioIO::ReadMethod N::AudioIO::Method(void)
{
  return method ;
}

bool N::AudioIO::AutoStop(void)
{
  return autoStop ;
}

bool N::AudioIO::setAutoStop  (bool stopIt)
{
  autoStop = stopIt ;
  return autoStop   ;
}

qint64 N::AudioIO::TotalRead(void) const
{
  return totalRead ;
}

qint64 N::AudioIO::TotalWritten(void) const
{
  return totalWritten ;
}

void N::AudioIO::Error(int error,QString message)
{
}

void N::AudioIO::started(void)
{
}

bool N::AudioIO::proceed(void)
{
  return true ;
}

void N::AudioIO::finished(void)
{
}

void N::AudioIO::progress(int current,int total)
{
}

bool N::AudioIO::Lock(void)
{
  mutex . lock ( ) ;
  return true      ;
}

bool N::AudioIO::Unlock(void)
{
  mutex . unlock ( ) ;
  return true        ;
}

QByteArray & N::AudioIO::add(char * dat,int len)
{
  Lock             (                   ) ;
  AudioData :: add ( (char *)dat , len ) ;
  if (len>0) BootTrigger = true          ;
  Unlock           (                   ) ;
  return ME                              ;
}

qint64 N::AudioIO::take(int pos,int len)
{
  Lock   ( )                                             ;
  RawFormat * rf = (RawFormat *)data()                   ;
  pos += sizeof(RawFormat)                               ;
  remove ( pos , len )                                   ;
  rf->DataSize = AudioData::size ( ) - sizeof(RawFormat) ;
  Unlock ( )                                             ;
  return rf->DataSize                                    ;
}

qint64 N::AudioIO::readData(char * dat,qint64 maxSize)
{
  nKickOut ( maxSize <= 0 , 0 )             ;
  nKickOut ( ! proceed()  , 0 )             ;
  Lock   ( )                                ;
  char * p = (char *)AudioData::data()      ;
  int hs = sizeof(RawFormat)                ;
  int ss = AudioData::size ( )              ;
  int as = ss                               ;
  int fs = maxSize                          ;
  int rs = AudioSize()                      ;
  #ifdef Q_OS_IOS
  qint64 resultSize = maxSize               ;
  #endif
  XDEBUG ( QString("N::AudioIO::readData = %1 , %2").arg(ss).arg(maxSize) ) ;
  if (ss>0 && rs>0)                         {
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
            fs           -= maxSize         ;
            rs           -= maxSize         ;
            totalRead    += maxSize         ;
            #ifdef ZERO_SET_EMPTY
            if (fs>0)                       {
              dat += maxSize                ;
              memset ( dat , 0 , fs )       ;
            }                               ;
            #endif
          } else                            {
            maxSize = 0                     ;
            rs      = 0                     ;
          }                                 ;
        break                               ;
        case Indexing                       :
          ss -= accessIndex                 ;
          if (maxSize>ss) maxSize = ss      ;
          if (maxSize>0)                    {
            p           += hs               ;
            p           += accessIndex      ;
            accessIndex += maxSize          ;
            memcpy ( dat , p  , maxSize )   ;
            fs          -= maxSize          ;
            rs          -= accessIndex      ;
            totalRead    = accessIndex      ;
            #ifdef ZERO_SET_EMPTY
            if (fs>0)                       {
              dat += maxSize                ;
              memset ( dat , 0 , fs )       ;
            }                               ;
            #endif
          } else                            {
            maxSize = 0                     ;
            rs      = 0                     ;
          }                                 ;
          progress ( accessIndex , as )     ;
        break                               ;
      }                                     ;
    } else                                  {
      maxSize = 0                           ;
      rs      = 0                           ;
    }                                       ;
  } else                                    {
    maxSize = 0                             ;
    rs      = 0                             ;
  }                                         ;
  Unlock ( )                                ;
  if (autoStop && BootTrigger && rs<=0)     {
    emit Empty ( )                          ;
  }                                         ;
  #ifdef Q_OS_IOS
  if (maxSize<=0) return 0                  ;
  return resultSize                         ;
  #else
  return maxSize                            ;
  #endif
}

qint64 N::AudioIO::writeData(const char * dat,qint64 maxSize)
{
  nKickOut ( ! proceed() , 0 ) ;
  add((char *)dat,maxSize)     ;
  totalWritten += maxSize      ;
  return maxSize               ;
}

int N::AudioIO::obtain(void)
{
  if ( CiosAudio::StreamIO::Stagnated == Output.Situation )   {
    if ( AudioData::AudioSize ( ) <= 0 )                      {
      Output . Result = CiosAudio::Conduit::Continue          ;
      return Output . Result                                  ;
    }                                                         ;
    Output.Situation = CiosAudio::StreamIO::Started           ;
  }                                                           ;
  int length = readData((char *)Output.Buffer,Output.Total()) ;
  if ( length <= 0 )                                          {
    Output . Result = CiosAudio::Conduit::Complete            ;
  } else                                                      {
    Output . Result = CiosAudio::Conduit::Continue            ;
  }                                                           ;
  return Output . Result                                      ;
}

int N::AudioIO::put(void)
{
  writeData ( (const char *)Input.Buffer , Input.Total() ) ;
  return Input . Result                                    ;
}

void N::AudioIO::finish(ConduitDirection direction,FinishCondition condition)
{
  if ( CiosAudio::Conduit::OutputDirection == direction ) {
    Output . Situation = CiosAudio::StreamIO::Stagnated   ;
  }                                                       ;
  if ( CiosAudio::Conduit::InputDirection  == direction ) {
    Input  . Situation = CiosAudio::StreamIO::Stagnated   ;
  }                                                       ;
}

void N::AudioIO::LockConduit(void)
{
  conduitMutex . lock   ( ) ;
}

void N::AudioIO::UnlockConduit(void)
{
  conduitMutex . unlock ( ) ;
}
