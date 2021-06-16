#include <qtaudioio.h>

int N::Acoustics::SamplingRates[15] = {
    8000                              ,
    9600                              ,
   11025                              ,
   12000                              ,
   16000                              ,
   22050                              ,
   24000                              ,
   32000                              ,
   44100                              ,
   48000                              ,
   88200                              ,
   96000                              ,
  176400                              ,
  192000                              ,
       0                            } ;

QString GetDeviceName(CiosAudio::Core & core,CiosAudio::DeviceInfo & device)
{
  CiosAudio::HostApi         * hostApi = NULL                 ;
  CiosAudio::HostApi::Encoding e                              ;
  QString           s                                         ;
  hostApi = NULL                                              ;
  core . GetHostApi ( &hostApi ,device.hostType)              ;
  if ( NULL != hostApi )                                      {
    e = hostApi->encoding()                                   ;
    if ( CiosAudio::HostApi::NATIVE == e )                    {
      QTextCodec * codec = QTextCodec::codecForLocale()       ;
      s = codec->toUnicode(device.name)                       ;
    } else
    if ( CiosAudio::HostApi::UTF8   == e )                    {
      QTextCodec * codec = QTextCodec::codecForName("UTF-8" ) ;
      s = codec->toUnicode(device.name)                       ;
    } else
    if ( CiosAudio::HostApi::UTF16  == e )                    {
      QTextCodec * codec = QTextCodec::codecForName("UTF-16") ;
      s = codec->toUnicode(device.name)                       ;
    } else s = QString(device.name)                           ;
  } else s = QString(device.name)                             ;
  return s                                                    ;
}

NAMEs N::Acoustics::Devices(N::Acoustics::Equipment equipment)
{
  NAMEs           devices                                 ;
  CiosAudio::Core core                                    ;
  core . Initialize ( )                                   ;
  for (int i=0;i<core.DeviceCount();i++)                  {
    CiosAudio::DeviceInfo * dev                           ;
    dev = core . GetDeviceInfo ( i )                      ;
    if ( NotNull ( dev ) )                                {
      switch (equipment)                                  {
        case Input                                        :
          if ( dev->maxInputChannels  > 0 )               {
            devices [ i ] = GetDeviceName ( core , *dev ) ;
          }                                               ;
        break                                             ;
        case Output                                       :
          if ( dev->maxOutputChannels > 0 )               {
            devices [ i ] = GetDeviceName ( core , *dev ) ;
          }                                               ;
        break                                             ;
     }                                                    ;
    }                                                     ;
  }                                                       ;
  core . Terminate ( )                                    ;
  return devices                                          ;
}

CiosAudio::CaSampleFormat N::Acoustics::toAudioCore(int format)
{
  CiosAudio::CaSampleFormat sampleFormat = CiosAudio::cafNothing             ;
  switch ( format )                                                          {
    case N::Acoustics::Int8                                                  :
      sampleFormat = CiosAudio::cafInt8                                      ;
    break                                                                    ;
    case N::Acoustics::UInt8                                                 :
      sampleFormat = CiosAudio::cafUint8                                     ;
    break                                                                    ;
    case N::Acoustics::Int16                                                 :
      sampleFormat = CiosAudio::cafInt16                                     ;
    break                                                                    ;
    case N::Acoustics::Int24                                                 :
      sampleFormat = CiosAudio::cafInt24                                     ;
    break                                                                    ;
    case N::Acoustics::Int32                                                 :
      sampleFormat = CiosAudio::cafInt32                                     ;
    break                                                                    ;
    case N::Acoustics::Float32                                               :
      sampleFormat = CiosAudio::cafFloat32                                   ;
    break                                                                    ;
    case N::Acoustics::Double                                                :
      sampleFormat = CiosAudio::cafFloat64                                   ;
    break                                                                    ;
    default                                                                  :
    break                                                                    ;
  }                                                                          ;
  return sampleFormat                                                        ;
}
