#include <qtaudioio.h>

N::AudioIn:: AudioIn        ( QObject * parent )
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

N::AudioIn:: AudioIn        ( const AudioData & other,QObject * parent )
           : AudioIO        (                   other,          parent )
           , Thread         (                                          )
           , core           (NULL                                      )
           , stream         (NULL                                      )
           , playing        (false                                     )
           , BytesPerSecond (0                                         )
           , name           (""                                        )
           , deviceId       (-1                                        )
           , interval       (100                                       )
{
  ThreadClass ( "AudioIn" )        ;
  core = new CiosAudio :: Core ( ) ;
  core -> Initialize  (    )       ;
}

N::AudioIn::~AudioIn(void)
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

QString N::AudioIn::audioDevice(void)
{
  return name ;
}

void N::AudioIn::setDevice(QString device)
{
  name = device ;
}

int N::AudioIn::inputDeviceId(void)
{
  return deviceId ;
}

void N::AudioIn::setDeviceId(int id)
{
  deviceId = id ;
}

int N::AudioIn::inputInterval(void)
{
  return interval ;
}

void N::AudioIn::setInterval(int Interval)
{
  interval = Interval ;
}

double N::AudioIn::currentVolume(int atChannel)
{
  if ( playing && NotNull(stream) && stream->hasVolume() ) {
    Volume [ atChannel ] = stream -> Volume ( atChannel )  ;
  }                                                        ;
  if ( ! Volume . contains ( atChannel ) )                 {
    Volume [ atChannel ] = 10000.0                         ;
  }                                                        ;
  return Volume [ atChannel ]                              ;
}

void N::AudioIn::setVolume(double volume,int atChannel)
{
  if ( playing && NotNull(stream) && stream->hasVolume() ) {
    stream->setVolume(volume,atChannel)                    ;
  }                                                        ;
  Volume [ atChannel ] = volume                            ;
}

void N::AudioIn::Error(int error,QString message)
{
}

bool N::AudioIn::proceed(void)
{
  return true ;
}

void N::AudioIn::progress(int current,int total)
{
}

void N::AudioIn::run(void)
{
  setIndex ( 0 )                                                             ;
  Input . Result = CiosAudio::Conduit::Continue                              ;
  if ( deviceId < 0 ) deviceId = core -> DefaultInputDevice ( )              ;
  ////////////////////////////////////////////////////////////////////////////
  CiosAudio::CaSampleFormat sampleFormat = Acoustics::toAudioCore(Format())  ;
  int frameCount = ( Rate ( ) * interval ) / 1000                            ;
  BytesPerSecond = AudioData::BytesPerSecond()                               ;
  ////////////////////////////////////////////////////////////////////////////
  CiosAudio::StreamParameters INSP ( deviceId , Channels ( ) , sampleFormat ) ;
  CiosAudio::CaError          rt                                             ;
  bool                        correct = true                                 ;
  ////////////////////////////////////////////////////////////////////////////
  INSP . suggestedLatency = core -> GetDeviceInfo(deviceId)->defaultLowInputLatency ;
  ////////////////////////////////////////////////////////////////////////////
  rt = core -> Open ( &stream                                                ,
                      &INSP                                                  ,
                      NULL                                                   ,
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

void N::AudioIn::start(void)
{
  nDropOut ( playing ) ;
  Thread :: start ( )  ;
}

void N::AudioIn::stop(void)
{
  playing = false ;
}
