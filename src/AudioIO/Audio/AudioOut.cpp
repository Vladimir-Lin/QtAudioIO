#include <qtaudioio.h>

N::AudioOut:: AudioOut       ( QObject * parent )
            : AudioIO        (           parent )
            , Thread         (                  )
            , core           ( NULL             )
            , stream         ( NULL             )
            , playing        ( false            )
            , BytesPerSecond ( 0                )
            , name           ( ""               )
            , deviceId       ( -1               )
            , interval       ( 100              )
{
  WidgetClass                      ;
  core = new CiosAudio :: Core ( ) ;
  core -> Initialize  (    )       ;
}

N::AudioOut:: AudioOut       ( const AudioData & other,QObject * parent )
            : AudioIO        (                   other,          parent )
            , Thread         (                                          )
            , core           ( NULL                                     )
            , stream         ( NULL                                     )
            , playing        ( false                                    )
            , BytesPerSecond ( 0                                        )
            , name           ( ""                                       )
            , deviceId       ( -1                                       )
            , interval       ( 100                                      )
{
  WidgetClass                      ;
  core = new CiosAudio :: Core ( ) ;
  core -> Initialize  (    )       ;
}

N::AudioOut::~AudioOut(void)
{
  stop ( )                 ;
  while ( NotNull(stream)) {
    Time :: skip ( 10 )    ;
  }                        ;
  nIfSafe ( core )         {
    core -> Terminate ( )  ;
    delete core            ;
    core   = NULL          ;
  }                        ;
}

QString N::AudioOut::audioDevice(void)
{
  return name ;
}

void N::AudioOut::setDevice(QString device)
{
  name = device ;
}

int N::AudioOut::outputDeviceId(void)
{
  return deviceId ;
}

void N::AudioOut::setDeviceId(int id)
{
  deviceId = id ;
}

int N::AudioOut::outputInterval(void)
{
  return interval ;
}

void N::AudioOut::setInterval(int Interval)
{
  interval = Interval ;
}

double N::AudioOut::currentVolume(int atChannel)
{
  if ( playing && NotNull(stream) && stream->hasVolume() ) {
    Volume [ atChannel ] = stream -> Volume ( atChannel )  ;
  }                                                        ;
  if ( ! Volume . contains ( atChannel ) )                 {
    Volume [ atChannel ] = 10000.0                         ;
  }                                                        ;
  return Volume [ atChannel ]                              ;
}

void N::AudioOut::setVolume(double volume,int atChannel)
{
  if ( playing && NotNull(stream) && stream->hasVolume() ) {
    stream->setVolume(volume,atChannel)                    ;
  }                                                        ;
  Volume [ atChannel ] = volume                            ;
}

void N::AudioOut::Error(int error,QString message)
{
}

bool N::AudioOut::proceed(void)
{
  return true ;
}

void N::AudioOut::progress(int current,int total)
{
}

void N::AudioOut::run(void)
{
  setIndex ( 0 )                                                             ;
  Input . Result = CiosAudio::Conduit::Continue                              ;
  if ( deviceId < 0 ) deviceId = core -> DefaultOutputDevice ( )             ;
  ////////////////////////////////////////////////////////////////////////////
  CiosAudio::CaSampleFormat sampleFormat = Acoustics::toAudioCore(Format())  ;
  int frameCount = ( Rate ( ) * interval ) / 1000                            ;
  BytesPerSecond = AudioData::BytesPerSecond()                               ;
  ////////////////////////////////////////////////////////////////////////////
  CiosAudio::StreamParameters ONSP ( deviceId , Channels ( ) , sampleFormat ) ;
  CiosAudio::CaError          rt                                             ;
  bool                        correct = true                                 ;
  ////////////////////////////////////////////////////////////////////////////
  ONSP . suggestedLatency = core -> GetDeviceInfo(deviceId)->defaultLowOutputLatency ;
  ////////////////////////////////////////////////////////////////////////////
  rt = core -> Open ( &stream                                                ,
                      NULL                                                   ,
                      &ONSP                                                  ,
                      Rate ( )                                               ,
                      frameCount                                             ,
                      0                                                      ,
                      this                                                 ) ;
  if ( ( CiosAudio::NoError != rt ) || ( NULL == stream ) ) correct = false  ;
  ////////////////////////////////////////////////////////////////////////////
  if ( correct )                                                             {
    rt = core -> Start ( stream )                                            ;
    if ( CiosAudio::NoError != rt ) correct = false                          ;
    playing = true                                                           ;
    started ( )                                                              ;
    //////////////////////////////////////////////////////////////////////////
    if ( correct && stream -> hasVolume ( ) )                                {
      if ( Volume . count ( ) <= 0 ) Volume [ -1 ] = 10000.0                 ;
      if ( Volume.count()==1 && Volume.contains(-1) )                        {
        stream -> setVolume ( Volume[-1] , -1 )                              ;
      } else                                                                 {
        CUIDs VID = Volume . keys ( )                                        ;
        int   vid                                                            ;
        foreach ( vid , VID ) if ( vid >= 0 )                                {
          stream -> setVolume ( vid , Volume [ vid ] )                       ;
        }                                                                    ;
      }                                                                      ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    while ( playing && correct && ( 1 == core -> IsActive  ( stream ) )    ) {
      Time :: msleep ( interval )                                            ;
      if ( ! isContinue                ) playing = false                     ;
      if ( ! Thread::Data . isContinue ) playing = false                     ;
    }                                                                        ;
    //////////////////////////////////////////////////////////////////////////
    if ( correct && ( 0 != core -> IsStopped ( stream ) ) )                  {
      core -> Stop ( stream )                                                ;
    }                                                                        ;
    playing = false                                                          ;
    finished ( )                                                             ;
  }                                                                          ;
  core -> Close ( stream )                                                   ;
  stream = NULL                                                              ;
  emit Stopped ( )                                                           ;
}

void N::AudioOut::start(const AudioData & data)
{
  nDropOut        ( playing ) ;
  BootTrigger = false         ;
  assign          ( data    ) ;
  Thread :: start (         ) ;
}

void N::AudioOut::start(void)
{
  nDropOut ( playing ) ;
  Thread :: start ( )  ;
}

void N::AudioOut::stop(void)
{
  playing = false ;
}

void N::SilentPlay(QString filename)
{
  N::initLIBAV ( )                                   ;
  N::AudioData audio                                 ;
  if ( ! N::Acoustics::Load(filename,audio) ) return ;
  ////////////////////////////////////////////////////
  N::AudioOut AIO                                    ;
  AIO.assign(audio)                                  ;
  AIO.start()                                        ;
  while (AIO.currentIndex()<AIO.AudioSize())         {
    N::Time::skip(1)                                 ;
    qApp->processEvents()                            ;
  }                                                  ;
}
