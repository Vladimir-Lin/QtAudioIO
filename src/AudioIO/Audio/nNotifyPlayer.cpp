#include <qtaudioio.h>

N::NotifyPlayer:: NotifyPlayer    ( QObject * Parent )
                : QObject         (           Parent )
                , Thread          ( 0,false          )
                , KeepRunning     ( NULL             )
                , name            ( ""               )
                , deviceId        ( -1               )
                , interval        ( 100              )
{
  WidgetClass ;
}

N::NotifyPlayer:: NotifyPlayer    ( const NotifyPlayer & audio )
                : QObject         ( audio . parent ( )         )
                , Thread          ( 0 , false                  )
                , KeepRunning     ( NULL                       )
                , name            ( ""                         )
                , deviceId        ( -1                         )
                , interval        ( 100                        )
{
  WidgetClass ;
}

N::NotifyPlayer::~NotifyPlayer(void)
{
  Shutdown ( ) ;
}

void N::NotifyPlayer::Stop(void)
{
  for (int i = 0 ; i < AllThreads . count ( ) ; i++ ) {
    ThreadData * d = AllThreads [ i ]                 ;
    d -> isContinue = false                           ;
  }                                                   ;
}

void N::NotifyPlayer::Append(QString name,AudioData & RawWave)
{
  AudioData * nad = new AudioData ( )    ;
  *nad = RawWave                         ;
  if (AudioBank.contains(name))          {
    AudioData * oad = AudioBank [ name ] ;
    AudioBank [ name ] = nad             ;
    delete oad                           ;
  } else AudioBank [ name ] = nad        ;
}

void N::NotifyPlayer::Play(QString n,QString device,int Id)
{
#pragma message("Add a mute here")
  if ( ! AudioBank . contains ( n ) ) return ;
  VarArgs args                               ;
  args << n                                  ;
  args << device                             ;
  args << Id                                 ;
  start ( 10001 , args )                     ;
}

void N::NotifyPlayer::Play(QString n)
{
  Play ( n , name , deviceId ) ;
}

void N::NotifyPlayer::Play(SUID uuid,QString device,int Id)
{
  if ( ! SoundMaps . contains ( uuid ) ) return ;
  Play ( SoundMaps [ uuid ] , device , Id )     ;
}

void N::NotifyPlayer::Play(SUID uuid)
{
  if ( ! SoundMaps . contains ( uuid ) ) return ;
  Play ( SoundMaps [ uuid ] )                   ;
}

void N::NotifyPlayer::Shutdown(void)
{
  isContinue = false ;
}

QString N::NotifyPlayer::audioDevice(void)
{
  return name ;
}

void N::NotifyPlayer::setDevice(QString device)
{
  name = device ;
}

int N::NotifyPlayer::outputDeviceId(void)
{
  return deviceId ;
}

void N::NotifyPlayer::setDeviceId(int id)
{
  deviceId = id ;
}

int N::NotifyPlayer::outputInterval(void)
{
  return interval ;
}

void N::NotifyPlayer::setInterval(int Interval)
{
  interval = Interval ;
}

double N::NotifyPlayer::currentVolume(int atChannel)
{
  if ( ! Volume . contains ( atChannel ) )                 {
    Volume [ atChannel ] = 10000.0                         ;
  }                                                        ;
  return Volume [ atChannel ]                              ;
}

void N::NotifyPlayer::setVolume(double volume,int atChannel)
{
  Volume [ atChannel ] = volume ;
}

void N::NotifyPlayer::Play(ThreadData * d)
{
  if ( NULL == d                      ) return               ;
  if ( d -> Arguments . count ( ) < 3 ) return               ;
  QString raw = d -> Arguments [ 0 ] . toString ( )          ;
  QString dev = d -> Arguments [ 1 ] . toString ( )          ;
  int     did = d -> Arguments [ 2 ] . toInt    ( )          ;
  if ( ! AudioBank . contains ( raw ) ) return               ;
  AudioData * nad = AudioBank [ raw ]                        ;
  if ( NULL == nad                    ) return               ;
  ////////////////////////////////////////////////////////////
  N::AudioOut AIO                                            ;
  ////////////////////////////////////////////////////////////
  AIO . setDevice   ( dev   )                                ;
  AIO . setDeviceId ( did   )                                ;
  ////////////////////////////////////////////////////////////
  if ( Volume . count ( ) > 0 )                              {
    CUIDs KS = Volume . keys ( )                             ;
    int   k                                                  ;
    foreach ( k , KS ) AIO . setVolume ( Volume [ k ] , k )  ;
  }                                                          ;
  ////////////////////////////////////////////////////////////
  AIO . assign      ( *nad  )                                ;
  AIO . start       (       )                                ;
  Time::msleep(150)                                          ;
  while ( IsContinue ( d )                                  &&
          ( AIO . currentIndex ( ) < AIO . AudioSize ( ) ) ) {
    N::Time::msleep ( 2 )                                    ;
  }                                                          ;
  Time::msleep(350)                                          ;
}

void N::NotifyPlayer::run(int T,ThreadData * data)
{
  switch ( T )      {
    case 10001      :
      Play ( data ) ;
    break           ;
  }                 ;
}
