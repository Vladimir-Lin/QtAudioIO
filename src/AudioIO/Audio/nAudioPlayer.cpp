#include <qtaudioio.h>

// #define CIOSDEBUG

#ifdef CIOSDEBUG
#include <Foundation>
#define XDEBUG(msg) if (N::qPlan->Verbose>=50) N::qPlan -> Debug ( msg )
#else
#define XDEBUG(msg)
#endif

//////////////////////////////////////////////////////////////////////////////

N::AudioPlayer:: AudioPlayer     (QObject * Parent)
               : AudioIO         (          Parent)
               , Thread          (0,false         )
               , KeepRunning     (NULL            )
               , Monitoring      (false           )
               , shoot           (false           )
               , completeStopped (true            )
               , core            (NULL            )
               , stream          (NULL            )
               , playing         (false           )
               , BytesPerSecond  (0               )
               , name            (""              )
               , deviceId        (-1              )
               , interval        (100             )
{
  ThreadClass ( "AudioPlayer" )    ;
  core = new CiosAudio :: Core ( ) ;
  core -> Initialize  (    )       ;
}

N::AudioPlayer:: AudioPlayer     (const AudioPlayer & audio)
               : AudioIO         (audio.parent()           )
               , Thread          (0,false                  )
               , KeepRunning     (NULL                     )
               , Monitoring      (false                    )
               , shoot           (false                    )
               , completeStopped (true                     )
               , core            (NULL                     )
               , stream          (NULL                     )
               , playing         (false                    )
               , BytesPerSecond  (0                        )
               , name            (""                       )
               , deviceId        (-1                       )
               , interval        (100                      )
{
  WidgetClass                      ;
  core = new CiosAudio :: Core ( ) ;
  core -> Initialize  (    )       ;
}

N::AudioPlayer::~AudioPlayer(void)
{
  nIfSafe ( core )        {
    core -> Terminate ( ) ;
    delete core           ;
    core   = NULL         ;
  }                       ;
}

void N::AudioPlayer::Stop(void)
{
  AudioQueue . clear ( ) ;
  Monitoring = false     ;
  playing    = false     ;
}

bool N::AudioPlayer::proceed(void)
{
  nKickOut ( !Monitoring         , false ) ;
  nKickOut ( IsNull(KeepRunning) , true  ) ;
  return ( *KeepRunning )                  ;
}

void N::AudioPlayer::progress(int current,int total)
{
  long long ts = total          ;
  long long ms = current        ;
  long long ds = BytesPerSecond ;
  ts *= 1000; ts /= ds          ;
  ms *= 1000; ms /= ds          ;
  emit AudioTime(ms,ts)         ;
}

void N::AudioPlayer::Append(QString name,AudioData & RawWave)
{
  AudioData * nad = new AudioData ( )         ;
  *nad = RawWave                              ;
  if (AudioBank.contains(name))               {
    AudioData * oad = AudioBank[name]         ;
    AudioBank[name] = nad                     ;
    if (!AudioQueue.contains(oad)) delete oad ;
  } else AudioBank[name] = nad                ;
}

void N::AudioPlayer::Append(AudioData & RawWave)
{
  AudioData * nad = new AudioData() ;
  *nad = RawWave                    ;
  AudioQueue << nad                 ;
  if ( ! Monitoring ) Monitor ( )   ;
}

void N::AudioPlayer::Play(QString name)
{
  if (!AudioBank.contains(name)) return ;
  AudioQueue << AudioBank[name]         ;
  if ( ! Monitoring ) Monitor ( )       ;
}

void N::AudioPlayer::Play(SUID uuid)
{
  if (!SoundMaps.contains(uuid)) return ;
  Play ( SoundMaps [ uuid ] )           ;
}

bool N::AudioPlayer::contains(AudioData * data)
{
  QStringList keys   = AudioBank.keys()          ;
  bool        exists = false                     ;
  for (int ii=0;!exists && ii<keys.count();ii++) {
    if (AudioBank[keys[ii]]==data) exists = true ;
  }                                              ;
  return exists                                  ;
}

void N::AudioPlayer::operator <= (AudioData & RawWave)
{
  if ( playing ) return           ;
  BootTrigger = false             ;
  AudioIO :: assign ( RawWave )   ;
  setIndex          ( 0       )   ;
  if ( ! Monitoring ) Monitor ( ) ;
}

void N::AudioPlayer::run(void)
{
  WatchQueue ( ) ;
}

void N::AudioPlayer::Monitor(void)
{
  nDropOut ( Monitoring ) ;
  Monitoring = true       ;
  shoot      = false      ;
  Thread :: start ( )     ;
}

void N::AudioPlayer::Shutdown(void)
{
  nDropOut ( !Monitoring )   ;
  Monitoring = false         ;
  shoot      = false         ;
  playing    = false         ;
  while ( !completeStopped ) {
    qApp->processEvents()    ;
    Time::msleep(10)         ;
  }                          ;
  Stop ( )                   ;
}

QString N::AudioPlayer::audioDevice(void)
{
  return name ;
}

void N::AudioPlayer::setDevice(QString device)
{
  name = device ;
}

int N::AudioPlayer::outputDeviceId(void)
{
  return deviceId ;
}

void N::AudioPlayer::setDeviceId(int id)
{
  deviceId = id ;
}

int N::AudioPlayer::outputInterval(void)
{
  return interval ;
}

void N::AudioPlayer::setInterval(int Interval)
{
  interval = Interval ;
}

double N::AudioPlayer::currentVolume(int atChannel)
{
  if ( playing && NotNull(stream) && stream->hasVolume() ) {
    Volume [ atChannel ] = stream -> Volume ( atChannel )  ;
  }                                                        ;
  if ( ! Volume . contains ( atChannel ) )                 {
    Volume [ atChannel ] = 10000.0                         ;
  }                                                        ;
  return Volume [ atChannel ]                              ;
}

void N::AudioPlayer::setVolume(double volume,int atChannel)
{
  if ( playing && NotNull(stream) && stream->hasVolume() ) {
    stream->setVolume(volume,atChannel)                    ;
  }                                                        ;
  Volume [ atChannel ] = volume                            ;
}

void N::AudioPlayer::NextQueue(void)
{
  if ( AudioQueue . count ( ) <= 0 )                                         {
    shoot  = false                                                           ;
    return                                                                   ;
  }                                                                          ;
  AudioData * nad = AudioQueue[0]                                            ;
  assign   ( *nad )                                                          ;
  setIndex ( 0    )                                                          ;
  BootTrigger = ( nad->AudioSize()>0 )                                       ;
  AudioQueue . takeAt(0)                                                     ;
  if ( ! contains ( nad ) ) delete nad                                       ;
  Output . Result = CiosAudio::Conduit::Continue                             ;
  if ( deviceId < 0 ) deviceId = core -> DefaultOutputDevice ( )             ;
  ////////////////////////////////////////////////////////////////////////////
  CiosAudio::CaSampleFormat sampleFormat = Acoustics::toAudioCore(Format())  ;
  int frameCount = ( Rate ( ) * interval ) / 1000                            ;
  BytesPerSecond = AudioData::BytesPerSecond()                               ;
  ////////////////////////////////////////////////////////////////////////////
  CiosAudio::StreamParameters INSP ( deviceId , Channels ( ) , sampleFormat ) ;
  CiosAudio::CaError          rt                                             ;
  bool                        correct = true                                 ;
  ////////////////////////////////////////////////////////////////////////////
  INSP . suggestedLatency = core -> GetDeviceInfo(deviceId)->defaultLowOutputLatency ;
  ////////////////////////////////////////////////////////////////////////////
  rt = core -> Open ( &stream                                                ,
                      NULL                                                   ,
                      &INSP                                                  ,
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
  }                                                                          ;
  core -> Close ( stream )                                                   ;
  stream = NULL                                                              ;
  shoot  = false                                                             ;
}

void N::AudioPlayer::WatchQueue(void)
{
  completeStopped = false                       ;
  shoot           = false                       ;
  while ( proceed ( ) )                         {
    Time :: msleep ( 10 )                       ;
    if ( AudioQueue . count ( ) <= 0 ) continue ;
    if ( shoot                       ) continue ;
    if ( playing                     ) continue ;
    shoot = true                                ;
    NextQueue ( )                               ;
  }                                             ;
  Monitoring      = false                       ;
  shoot           = false                       ;
  completeStopped = true                        ;
}
