#include <qtaudioio.h>

N::AvDecoder:: AvDecoder       (void)
             : FFmpeg::Decoder (    )
{
}

N::AvDecoder::~AvDecoder (void)
{
}

int N::AvDecoder::DecodeAudio(AVPacket & Packet,AudioData & data)
{
  nKickOut ( !isAudio , 0 )                       ;
  /////////////////////////////////////////////////
  int  audiolen = 0                               ;
  int  len      = 0                               ;
  int  dataSize = 0                               ;
  int  gotFrame = 0                               ;
  /////////////////////////////////////////////////
  av_frame_unref(AudioFrame)                      ;
  while ( nGreater ( Packet.size , 0 ) )          {
    gotFrame = 0                                  ;
    len      = ::avcodec_decode_audio4            (
                 AudioCtx                         ,
                 AudioFrame                       ,
                 &gotFrame                        ,
                 &Packet                        ) ;
    if (len < 0)                                  {
      Error = len                                 ;
      return audiolen                             ;
    }                                             ;
    if ( nGreater ( gotFrame , 0 ) )              {
      dataSize = ::av_samples_get_buffer_size     (
                   NULL                           ,
                   AudioCtx   -> channels         ,
                   AudioFrame -> nb_samples       ,
                   AudioCtx   -> sample_fmt       ,
                   1                            ) ;
      if (dataSize>0)                             {
        char * src = (char *)AudioFrame->data[0]  ;
        if (NotNull(Resample))                    {
         ::swr_convert(Resample                   ,
                       &audioConvert              ,
                       AudioFrame -> nb_samples   ,
     (const uint8_t **)AudioFrame -> data         ,
                       AudioFrame -> nb_samples ) ;
          src = (char *)audioConvert              ;
        }                                         ;
        data . add                                (
          (char *)src                             ,
          dataSize                              ) ;
        audiolen += dataSize                      ;
        return audiolen                           ;
      }                                           ;
    }                                             ;
    Packet.size -= len                            ;
    Packet.data += len                            ;
    Packet.pts   = AV_NOPTS_VALUE                 ;
    Packet.dts   = AV_NOPTS_VALUE                 ;
  }                                               ;
  return audiolen                                 ;
}
