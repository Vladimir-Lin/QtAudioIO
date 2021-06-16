#include <qtaudioio.h>

N::AudioJack:: AudioJack    (QObject * parent)
             : AudioIn      (          parent)
             , AudioChannel (                )
{
  WidgetClass ;
}

N::AudioJack::~AudioJack(void)
{
}

int N::AudioJack::type(void) const
{
  return 1 ;
}

QString N::AudioJack::name(void)
{
  return tr("Qt Audio Input") ;
}

QString N::AudioJack::source(void)
{
  return AudioIn::name ;
}

bool N::AudioJack::hasData(void)
{
  return bytes ( ) > 0 ;
}

TUID N::AudioJack::length(void)
{
  return Milliseconds ( )  ;
}

TUID N::AudioJack::bytes(void)
{
  return AudioSize ( ) ;
}

int N::AudioJack::rate(void)
{
  return Rate ( ) ;
}

int N::AudioJack::channels(void)
{
  return Channels ( ) ;
}

bool N::AudioJack::hasChannel(int id)
{
  nKickOut ( id< 0          , false ) ;
  nKickOut ( id>=Channels() , false ) ;
  return true                         ;
}

QString N::AudioJack::channel(int id)
{
  nKickOut ( !hasChannel(id) , "" )               ;
  return tr("%1 : %2").arg(AudioIn::name).arg(id) ;
}

bool N::AudioJack::canSeek(void)
{
  return false ;
}

bool N::AudioJack::assign(AudioData & data)
{
  data . set                              (
    rate                            ( )   ,
    Bits                            ( )   ,
    channels                        ( )   ,
    (Acoustics::SampleFormat)Format ( ) ) ;
  return true                             ;
}

bool N::AudioJack::obtain(AudioData & data,int bytes)
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

bool N::AudioJack::play(void)
{
  AudioIn :: start ( ) ;
  return true          ;
}

bool N::AudioJack::Pause(void)
{
  Mute = true ;
  return true ;
}

bool N::AudioJack::seek(TUID timestamp)
{
  return false ;
}

bool N::AudioJack::Resume(void)
{
  Mute = false ;
  return true  ;
}

bool N::AudioJack::abort(void)
{
  AudioIn :: stop ( ) ;
  return true         ;
}
