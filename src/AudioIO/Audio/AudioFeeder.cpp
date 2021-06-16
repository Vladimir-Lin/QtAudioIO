#include <qtaudioio.h>

N::AudioFeeder:: AudioFeeder    ( void     )
               : AudioReader    (          )
               , AudioChannel   (          )
               , Thread         (          )
               , bufferTime     ( 5 * 1000 )
               , interval       ( 10       )
               , waitCompletion ( true     )
{
  WidgetClass ;
}

N::AudioFeeder::~AudioFeeder(void)
{
}

int N::AudioFeeder::type(void) const
{
  return 2 ;
}

QString N::AudioFeeder::name(void)
{
  return tr("Audio File Feeder") ;
}

QString N::AudioFeeder::source(void)
{
  return Filename ;
}

bool N::AudioFeeder::hasData(void)
{
  return bytes ( ) > 0 ;
}

TUID N::AudioFeeder::length(void)
{
  return AvDecoder :: Duration ( ) / 1000 ;
}

TUID N::AudioFeeder::bytes(void)
{
  return AudioSize ( ) ;
}

int N::AudioFeeder::rate(void)
{
  return Rate ( ) ;
}

int N::AudioFeeder::channels(void)
{
  return AudioData :: Channels ( ) ;
}

bool N::AudioFeeder::hasChannel(int id)
{
  nKickOut ( id< 0                     , false ) ;
  nKickOut ( id>=AudioData::Channels() , false ) ;
  return true                                    ;
}

QString N::AudioFeeder::channel(int id)
{
  nKickOut ( !hasChannel(id) , "" )          ;
  return tr("%1 : %2").arg(Filename).arg(id) ;
}

bool N::AudioFeeder::canSeek(void)
{
  return true ;
}

bool N::AudioFeeder::assign(AudioData & data)
{
  data . set                              (
    rate                            ( )   ,
    Bits                            ( )   ,
    channels                        ( )   ,
    (Acoustics::SampleFormat)Format ( ) ) ;
  return true                             ;
}

bool N::AudioFeeder::obtain(AudioData & data,int bytes)
{
  nKickOut ( AudioSize() <= 0 , false )      ;
  char * src = index(0)                      ;
  nKickOut ( IsNull(src)      , false )      ;
  Lock   ( )                                 ;
  src = index(0)                             ;
  if (AudioSize()<bytes) bytes = AudioSize() ;
  if (!Mute) data . add ( src , bytes )      ;
  takeAt ( 0 , bytes )                       ;
  Unlock ( )                                 ;
  return true                                ;
}

bool N::AudioFeeder::play(void)
{
  start ( )   ;
  return true ;
}

bool N::AudioFeeder::Pause(void)
{
  Mute = true ;
  return true ;
}

bool N::AudioFeeder::seek(TUID msecs)
{
  return RawSeek ( msecs ) ;
}

bool N::AudioFeeder::Resume(void)
{
  Mute = false ;
  return true  ;
}

bool N::AudioFeeder::abort(void)
{
  going = false ;
  return true   ;
}

int N::AudioFeeder::Interval(void)
{
  return interval ;
}

void N::AudioFeeder::setInterval(int value)
{
  interval = value ;
}

int N::AudioFeeder::BufferTime(void)
{
  return bufferTime ;
}

void N::AudioFeeder::setBufferTime(int buft)
{
  bufferTime = buft ;
}

bool N::AudioFeeder::Waiting(void)
{
  return waitCompletion ;
}

void N::AudioFeeder::setCompletion(bool wait)
{
  waitCompletion = wait ;
}

void N::AudioFeeder::Completion(void)
{
  while (waitCompletion && AudioSize()>0 && interval>0) {
    Time::msleep(interval)                              ;
  }                                                     ;
}

void N::AudioFeeder::run(void)
{
  nDropOut ( ! AudioReader :: prepare ( ) )        ;
  started ( )                                      ;
  while ( AudioReader :: proceed ( ) )             {
    bool shouldRead = true                         ;
    if (bufferTime>0 && Milliseconds()>bufferTime) {
      shouldRead = false                           ;
    }                                              ;
    if (shouldRead) AudioReader :: decode ( )      ;
    if (AudioReader :: proceed ( ) && interval>0)  {
      Time::msleep(interval)                       ;
    }                                              ;
  }                                                ;
  cease      ( )                                   ;
  Completion ( )                                   ;
  finished   ( )                                   ;
}
