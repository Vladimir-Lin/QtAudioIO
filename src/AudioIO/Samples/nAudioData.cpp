#include <qtaudioio.h>
#include "nAudioDataPrivate.hpp"

#define MAXDATASIZE ( 1792 * 1024 * 1024 )

N::AudioData:: AudioData  (void)
             : QByteArray (    )
{
  reset ( ) ;
}

N::AudioData:: AudioData  (const AudioData & other)
             : QByteArray (                       )
{
  if (other.size()>sizeof(RawFormat)) {
    resize(other.size())              ;
    char * f = (char *)other.data()   ;
    char * s = (char *)other.data()   ;
    memcpy(s,f,other.size())          ;
  } else reset()                      ;
}

N::AudioData::~AudioData (void)
{
}

void N::AudioData::zero(void)
{
  resize(sizeof(RawFormat))            ;
  RawFormat * rf = (RawFormat *)data() ;
  rf->DataSize   = 0                   ;
}

void N::AudioData::reset(void)
{
  resize(sizeof(RawFormat))            ;
  RawFormat * rf = (RawFormat *)data() ;
  rf->SampleRate = 0                   ;
  rf->Bits       = 0                   ;
  rf->Channels   = 0                   ;
  rf->Format     = 0                   ;
  rf->DataSize   = 0                   ;
}

void N::AudioData :: set                (
       int                     rate     ,
       int                     bits     ,
       int                     channels ,
       Acoustics::SampleFormat format   )
{
  RawFormat * rf = (RawFormat *)data() ;
  rf->SampleRate = rate                ;
  rf->Bits       = bits                ;
  rf->Channels   = channels            ;
  rf->Format     = (int)format         ;
}

int N::AudioData::Bits(void) const
{
  RawFormat * rf = (RawFormat *)data() ;
  return rf->Bits                      ;
}

int N::AudioData::Channels(void) const
{
  RawFormat * rf = (RawFormat *)data() ;
  return rf->Channels                  ;
}

int N::AudioData::Rate(void) const
{
  RawFormat * rf = (RawFormat *)data() ;
  return rf->SampleRate                ;
}

int N::AudioData::Format(void) const
{
  RawFormat * rf = (RawFormat *)data() ;
  return rf->Format                    ;
}

int N::AudioData::AudioSize(void) const
{
  RawFormat * rf = (RawFormat *)data() ;
  return rf->DataSize                  ;
}

QTime N::AudioData::Duration(void)
{
  QTime T(0,0,0,0)               ;
  T = T.addMSecs(Milliseconds()) ;
  return T                       ;
}

int N::AudioData::Seconds(void)
{
  int oas = AudioSize()                ;
  int bps = BytesPerSecond()           ;
  if (bps<=0) return 0                 ;
  return (oas/bps)                     ;
}

int N::AudioData::Milliseconds(void)
{
  RawFormat * rf = (RawFormat *)data() ;
  long long oas = rf->DataSize         ;
  int bpf = BytesPerFrame()            ;
  if (rf->SampleRate<=0) return 0      ;
  if (bpf           <=0) return 0      ;
  oas *= 1000                          ;
  oas /= bpf                           ;
  oas /= rf->SampleRate                ;
  return (int)oas                      ;
}

int N::AudioData::Frames(void)
{
  RawFormat * rf = (RawFormat *)data() ;
  int bpf = rf->Bits                   ;
  bpf    *= rf->Channels               ;
  bpf    /= 8                          ;
  return (rf->DataSize / bpf)          ;
}

int N::AudioData::BytesPerFrame(void)
{
  RawFormat * rf = (RawFormat *)data() ;
  int bpf = rf->Bits                   ;
  bpf    *= rf->Channels               ;
  bpf    /= 8                          ;
  return bpf                           ;
}

int N::AudioData::BytesPerSecond(void)
{
  RawFormat * rf = (RawFormat *)data() ;
  int bpf = rf->Bits                   ;
  bpf    *= rf->Channels               ;
  bpf    /= 8                          ;
  bpf    *= rf->SampleRate             ;
  return bpf                           ;
}

double N::AudioData::upperBound(void)
{
  switch (Format())         {
    case Acoustics::UInt8   :
    return 255.0            ;
    case Acoustics::Int8    :
    return 127.0            ;
    case Acoustics::Int16   :
    return 32767.0          ;
    case Acoustics::Int32   :
    return 2147483647.0     ;
    case Acoustics::Float32 :
    return 1.0              ;
    case Acoustics::Int24   :
    case Acoustics::Guess   :
    case Acoustics::Double  :
    case Acoustics::Custom  :
    break                   ;
  }                         ;
  return 0                  ;
}

double N::AudioData::lowerBound(void)
{
  switch (Format())         {
    case Acoustics::UInt8   :
    return 0.0              ;
    case Acoustics::Int8    :
    return -128.0           ;
    case Acoustics::Int16   :
    return -32768.0         ;
    case Acoustics::Int32   :
    return -2147483648.0    ;
    case Acoustics::Float32 :
    return -1.0             ;
    case Acoustics::Int24   :
    case Acoustics::Guess   :
    case Acoustics::Double  :
    case Acoustics::Custom  :
    break                   ;
  }                         ;
  return 0                  ;
}

bool N::AudioData::operator == (const AudioData & other)
{
  return equal(other) ;
}

bool N::AudioData::equal(const AudioData & other)
{
  nKickOut (Bits    ()!=other.Bits    () , false ) ;
  nKickOut (Rate    ()!=other.Rate    () , false ) ;
  nKickOut (Channels()!=other.Channels() , false ) ;
  nKickOut (Format  ()!=other.Format  () , false ) ;
  return true                                      ;
}

bool N::AudioData::load(QIODevice & file)
{
  clear ( )                           ;
  if (file.open(QIODevice::ReadOnly)) {
    QByteArray F = file.readAll()     ;
    file.close()                      ;
    if (F.size()>0)                   {
      append(F)                       ;
      return true                     ;
    }                                 ;
  }                                   ;
  return false                        ;
}

bool N::AudioData::save(QIODevice & file)
{
  bool success = false                            ;
  if (file.open(QIODevice::WriteOnly))            {
    if (file.write(*this)==size()) success = true ;
    file.close()                                  ;
  }                                               ;
  return success                                  ;
}

bool N::AudioData::fresh(void)
{
  return true ;
}

bool N::AudioData::demolish(void)
{
  return true ;
}

QByteArray & N::AudioData::assign(const AudioData & other)
{
  if (other.size()>0)               {
    resize(other.size())            ;
    char * f = (char *)data()       ;
    char * s = (char *)other.data() ;
    memcpy(f,s,other.size())        ;
  }                                 ;
  return *this                      ;
}

QByteArray & N::AudioData::operator = (const AudioData & other)
{
  if (other.size()>0)               {
    resize(other.size())            ;
    char * f = (char *)data()       ;
    char * s = (char *)other.data() ;
    memcpy(f,s,other.size())        ;
  }                                 ;
  return *this                      ;
}

QByteArray & N::AudioData::operator += (AudioData & other)
{
  return add(other) ;
}

QByteArray & N::AudioData::add(AudioData & other)
{
  if (equal(other))                      {
    int oas = other.AudioSize()          ;
    RawFormat * rf = (RawFormat *)data() ;
    int di = rf->DataSize                ;
    resize(sizeof(RawFormat)+di+oas)     ;
    char * f = (char *)data()            ;
    char * s = (char *)other.data()      ;
    rf = (RawFormat *)data()             ;
    s += sizeof(RawFormat)               ;
    f += sizeof(RawFormat)+rf->DataSize  ;
    memcpy(f,s,oas)                      ;
    rf->DataSize += oas                  ;
  }                                      ;
  return ME                              ;
}

QByteArray & N::AudioData::add(char * s,int len)
{
  RawFormat * rf = (RawFormat *)data() ;
  int di = rf->DataSize                ;
  if (nGreater(di,MAXDATASIZE))        {
    return ME                          ;
  }                                    ;
  //////////////////////////////////////
  resize(sizeof(RawFormat)+di+len)     ;
  rf = (RawFormat *)data()             ;
  char * f = (char *)data()            ;
  rf = (RawFormat *)data()             ;
  f += sizeof(RawFormat)+rf->DataSize  ;
  memcpy(f,s,len)                      ;
  rf->DataSize += len                  ;
  return ME                            ;
}

QByteArray & N::AudioData::trim(int fromData,int toData)
{
  RawFormat * rf = (RawFormat *)data()           ;
  int s = rf->DataSize                           ;
  if (fromData <=      0) return ME              ;
  if (fromData >       s) return ME              ;
  if (toData   <=      0) return ME              ;
  if (toData   >       s) return ME              ;
  if (fromData >  toData) return ME              ;
  int d = toData - fromData                      ;
  ////////////////////////////////////////////////
  QByteArray V                                   ;
  V.resize(d)                                    ;
  int    shift = sizeof(RawFormat)               ;
  char * f     = (char *)  data()                ;
  char * z     = (char *)V.data()                ;
  memcpy(z                  ,f+shift+fromData,d) ;
  memcpy(f+shift,z                           ,d) ;
  rf->DataSize = d                               ;
  resize(shift+d)                                ;
  V.clear()                                      ;
  return ME                                      ;
}

int N::AudioData::takeAt(int pos,int length)
{
  RawFormat * rf = (RawFormat *)data()  ;
  int s     = rf->DataSize              ;
  int shift = sizeof(RawFormat)         ;
  if ((pos+length)>s)                   {
    length = s - pos                    ;
  }                                     ;
  if (length<=0) return rf->DataSize    ;
  remove ( shift + pos , length )       ;
  rf->DataSize = s - length             ;
  return rf->DataSize                   ;
}

int N::AudioData::buffer(int seconds)
{
  RawFormat * rf = (RawFormat *)data() ;
  int ds = seconds                     ;
  ds *= BytesPerSecond()               ;
  rf->DataSize = ds                    ;
  resize(sizeof(RawFormat)+ds)         ;
  return size()                        ;
}

char * N::AudioData::index(int shift)
{
  char * s = data()      ;
  s += sizeof(RawFormat) ;
  s += shift             ;
  return s               ;
}

char * N::AudioData::frame(int atFrame)
{
  char * s = data()                ;
  s += sizeof(RawFormat)           ;
  s += (atFrame * BytesPerFrame()) ;
  return s                         ;
}

bool N::AudioData::Lock(void)
{
  return false ;
}

bool N::AudioData::Unlock(void)
{
  return false ;
}

QByteArray N::AudioData::header(void)
{
  QByteArray H                                              ;
  H.resize(sizeof(RawFormat))                               ;
  memcpy((char *)H.data(),(char *)data(),sizeof(RawFormat)) ;
  return H                                                  ;
}

QAudioFormat N::AudioData::AudioFormat(void)
{
  QAudioFormat FMT                                         ;
  FMT . setChannelCount     ( Channels()                 ) ;
  FMT . setCodec            ( "audio/pcm"                ) ;
  FMT . setSampleRate       ( Rate()                     ) ;
  FMT . setSampleSize       ( Bits()                     ) ;
  FMT . setByteOrder        ( QAudioFormat::LittleEndian ) ;
  switch (Format())                                        {
    case Acoustics::UInt8                                  :
      FMT . setSampleType   ( QAudioFormat::UnSignedInt  ) ;
    break                                                  ;
    case Acoustics::Int8                                   :
    case Acoustics::Int16                                  :
    case Acoustics::Int32                                  :
      FMT . setSampleType   ( QAudioFormat::SignedInt    ) ;
    break                                                  ;
    case Acoustics::Float32                                :
      FMT . setSampleType   ( QAudioFormat::Float        ) ;
    break                                                  ;
    case Acoustics::Int24                                  :
    case Acoustics::Guess                                  :
    case Acoustics::Double                                 :
    case Acoustics::Custom                                 :
      FMT . setSampleType   ( QAudioFormat::Unknown      ) ;
    break                                                  ;
  }                                                        ;
  return FMT                                               ;
}

int N::AudioData::toVector(Vector & vector,int channel)
{
  switch (Format())                                        {
    case Acoustics::Int24                                  :
    case Acoustics::Guess                                  :
    case Acoustics::Custom                                 :
    return 0                                               ;
  }                                                        ;
  int skip  = (Channels()>1) ? 1 : 0                       ;
  int total = Frames()                                     ;
  int bytes = Bits ( ) / 8                                 ;
  if (channel<0) total *= Channels ( )                     ;
  if (total<=0) return 0                                   ;
  vector . set ( Cpp::Double , total )                     ;
  double * d = (double *)vector.array()                    ;
  switch (Format())                                        {
    case Acoustics::UInt8                                  :
      if (channel<0)                                       {
        Math::toDouble(total,(unsigned char *)index(0),d)  ;
      } else
      if (skip<=0)                                         {
        Math::toDouble(total,(unsigned char *)index(0),d)  ;
      } else                                               {
        Vector V                                           ;
        V . set ( Cpp::Byte , 0 )                          ;
        const char * a = (const char *)index(0)            ;
        a += channel * bytes                               ;
        V . append ( Frames () , Channels () - 1 , a )     ;
        Math::toDouble                                     (
          Frames ( )                                       ,
          (unsigned char *)V.array()                       ,
          d                                              ) ;
      }                                                    ;
    break                                                  ;
    case Acoustics::Int8                                   :
      if (channel<0)                                       {
        Math::toDouble(total,(char *)index(0),d)           ;
      } else
      if (skip<=0)                                         {
        Math::toDouble(total,(char *)index(0),d)           ;
      } else                                               {
        Vector V                                           ;
        V . set ( Cpp::Char , 0 )                          ;
        const char * a = (const char *)index(0)            ;
        a += channel * bytes                               ;
        V . append ( Frames () , Channels () - 1 , a )     ;
        Math::toDouble                                     (
          Frames ( )                                       ,
          (char *)V.array()                                ,
          d                                              ) ;
      }                                                    ;
    break                                                  ;
    case Acoustics::Int16                                  :
      if (channel<0)                                       {
        Math::toDouble(total,(short *)index(0),d)          ;
      } else
      if (skip<=0)                                         {
        Math::toDouble(total,(short *)index(0),d)          ;
      } else                                               {
        Vector V                                           ;
        V . set ( Cpp::Short , 0 )                         ;
        const char * a = (const char *)index(0)            ;
        a += channel * bytes                               ;
        V . append ( Frames () , Channels () - 1 , a )     ;
        Math::toDouble                                     (
          Frames ( )                                       ,
          (short *)V.array()                               ,
          d                                              ) ;
      }                                                    ;
    break                                                  ;
    case Acoustics::Int32                                  :
      if (channel<0)                                       {
        Math::toDouble(total,(int *)index(0),d)            ;
      } else
      if (skip<=0)                                         {
        Math::toDouble(total,(int *)index(0),d)            ;
      } else                                               {
        Vector V                                           ;
        V . set ( Cpp::Integer , 0 )                       ;
        const char * a = (const char *)index(0)            ;
        a += channel * bytes                               ;
        V . append ( Frames () , Channels () - 1 , a )     ;
        Math::toDouble                                     (
          Frames ( )                                       ,
          (int *)V.array()                                 ,
          d                                              ) ;
      }                                                    ;
    break                                                  ;
    case Acoustics::Float32                                :
      if (channel<0)                                       {
        Math::toDouble(total,(float *)index(0),d)          ;
      } else
      if (skip<=0)                                         {
        Math::toDouble(total,(float *)index(0),d)          ;
      } else                                               {
        Vector V                                           ;
        V . set ( Cpp::Float , 0 )                         ;
        const char * a = (const char *)index(0)            ;
        a += channel * bytes                               ;
        V . append ( Frames () , Channels () - 1 , a )     ;
        Math::toDouble                                     (
          Frames ( )                                       ,
          (float *)V.array()                               ,
          d                                              ) ;
      }                                                    ;
    break                                                  ;
    case Acoustics::Double                                 :
      if (channel<0)                                       {
        memcpy ( d , index(0) , total * sizeof(double) )   ;
      } else
      if (skip<=0)                                         {
        memcpy ( d , index(0) , total * sizeof(double) )   ;
      } else                                               {
        Vector V                                           ;
        V . set ( Cpp::Double , 0 )                        ;
        const char * a = (const char *)index(0)            ;
        a += channel * bytes                               ;
        V . append ( Frames () , Channels () - 1 , a )     ;
        memcpy ( d , V . array( ) , total*sizeof(double) ) ;
      }                                                    ;
    break                                                  ;
  }                                                        ;
  return vector . count()                                  ;
}
