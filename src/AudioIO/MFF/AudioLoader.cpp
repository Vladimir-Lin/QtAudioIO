#include <qtaudioio.h>

TUID N::Acoustics::Load (QString filename)
{
  AvDecoder AV                           ;
  if (!AV.StartAudio(filename)) return 0 ;
  TUID duration = AV.Duration()          ;
  AV.Close()                             ;
  if (duration<0) duration  =    0       ;
  if (duration>0) duration /= 1000       ;
  return duration                        ;
}

bool N::Acoustics::Load(QString filename,AudioData & data)
{
  ////////////////////////////////////////////////////////////////
  QFileInfo FSI(filename)                                        ;
  QString   Suffix = FSI.suffix()                                ;
  Suffix = Suffix . toLower ( )                                  ;
  if (Suffix=="raw" && FSI.exists())                             {
    bool success = false                                         ;
    QFile F(filename)                                            ;
    success = data . load ( F )                                  ;
    if (success) return true ; else data.reset()                 ;
  }                                                              ;
  ////////////////////////////////////////////////////////////////
  AvDecoder AV                                                   ;
  if (!AV.StartAudio(filename)) return false                     ;
  AVPacket Packet                                                ;
  int      Channels = AV . Channels       ( )                    ;
  int      Sample   = AV . AvSampleFormat ( )                    ;
  int      Bits     = N :: Acoustics :: AvBits   (Sample)        ;
  int      Format   = N :: Acoustics :: AvFormat (Sample)        ;
  bool     Go       = true                                       ;
  int      Ret                                                   ;
  AV   . Initialize ( &Packet )                                  ;
  data . reset      (         )                                  ;
  data . set                                                     (
    AV.AvSampleRate()                                            ,
    Bits                                                         ,
    Channels                                                     ,
    (N::Acoustics::SampleFormat)Format                         ) ;
  do                                                             {
    Ret = AV.ReadPacket(Packet)                                  ;
    switch (Ret)                                                 {
      case AVERROR_BUFFER_TOO_SMALL                              :
      case AVERROR_EXTERNAL                                      :
      case AVERROR_BUG2                                          :
      case AVERROR_BSF_NOT_FOUND                                 :
      case AVERROR_BUG                                           :
      case AVERROR_DECODER_NOT_FOUND                             :
      case AVERROR_DEMUXER_NOT_FOUND                             :
      case AVERROR_ENCODER_NOT_FOUND                             :
      case AVERROR_EXIT                                          :
      case AVERROR_FILTER_NOT_FOUND                              :
      case AVERROR_INVALIDDATA                                   :
      case AVERROR_MUXER_NOT_FOUND                               :
      case AVERROR_OPTION_NOT_FOUND                              :
      case AVERROR_PATCHWELCOME                                  :
      case AVERROR_PROTOCOL_NOT_FOUND                            :
      case AVERROR_STREAM_NOT_FOUND                              :
      case AVERROR_UNKNOWN                                       :
      case AVERROR_EXPERIMENTAL                                  :
      case AVERROR_EOF                                           :
        Go = false                                               ;
      break                                                      ;
      default                                                    :
        if (Ret<0) Go = false ;                               else
        if ( AV . isAudioChannel ( Packet ) )                    {
          AV . DecodeAudio ( Packet , data )                     ;
        }                                                        ;
      break                                                      ;
    }                                                            ;
    AV . Release ( &Packet )                                     ;
  } while (Go)                                                   ;
  AV.Close()                                                     ;
  return ( data . AudioSize ( ) > 0 )                            ;
}
