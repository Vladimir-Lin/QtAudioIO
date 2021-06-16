#include <qtaudioio.h>

static bool codecExists(int codecId,int codecType)
{
  AVCodec * codec = NULL                                    ;
  if (codecType==0)                                         {
    codec = ::avcodec_find_decoder((enum AVCodecID)codecId) ;
  } else
  if (codecType==1)                                         {
    codec = ::avcodec_find_encoder((enum AVCodecID)codecId) ;
  }                                                         ;
  nKickOut ( IsNull(codec)                     , false )    ;
  nKickOut ( codec->type != AVMEDIA_TYPE_AUDIO , false )    ;
  return true                                               ;
}

QStringList N::Acoustics::ExtensionFilters (int type)
{
  QStringList filters                             ;
  #define EF(ID,NAME)                             \
    if (codecExists(ID,type))                   { \
      QString n = NAME                          ; \
      if (n.length()>0)                         { \
        n = "*." + n                            ; \
        if (!filters.contains(n))               { \
          filters << n                          ; \
        }                                       ; \
      }                                         ; \
    }
  EF ( AV_CODEC_ID_FIRST_AUDIO       ,  "fia"   ) ;
  EF ( AV_CODEC_ID_PCM_S16LE         ,  "wav"   ) ;
  EF ( AV_CODEC_ID_PCM_S16BE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_U16LE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_U16BE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_S8            ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_U8            ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_MULAW         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_ALAW          ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_S32LE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_S32BE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_U32LE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_U32BE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_S24LE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_S24BE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_U24LE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_U24BE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_S24DAUD       ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_ZORK          ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_S16LE_PLANAR  ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_DVD           ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_F32BE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_F32LE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_F64BE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_F64LE         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_BLURAY        ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_LXF           ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_S302M             ,  "s302"  ) ;
  EF ( AV_CODEC_ID_PCM_S8_PLANAR     ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_S24LE_PLANAR  ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_S32LE_PLANAR  ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_PCM_S16BE_PLANAR  ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_IMA_QT      ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_IMA_WAV     ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_IMA_DK3     ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_IMA_DK4     ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_IMA_WS      ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_IMA_SMJPEG  ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_MS          ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_4XM         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_XA          ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_ADX         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_EA          ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_G726        ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_CT          ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_SWF         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_YAMAHA      ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_SBPRO_4     ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_SBPRO_3     ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_SBPRO_2     ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_THP         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_IMA_AMV     ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_EA_R1       ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_EA_R3       ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_EA_R2       ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_IMA_EA_SEAD ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_IMA_EA_EACS ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_EA_XAS      ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_EA_MAXIS_XA ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_IMA_ISS     ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_G722        ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_IMA_APC     ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_VIMA              ,  "vima"  ) ;
  EF ( AV_CODEC_ID_ADPCM_AFC         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_IMA_OKI     ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_DTK         ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_IMA_RAD     ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_ADPCM_G726LE      ,  "pcm"   ) ;
  EF ( AV_CODEC_ID_AMR_NB            ,  "amr"   ) ;
  EF ( AV_CODEC_ID_AMR_WB            ,  "amr"   ) ;
  EF ( AV_CODEC_ID_RA_144            ,  "ra"    ) ;
  EF ( AV_CODEC_ID_RA_288            ,  "ra"    ) ;
  EF ( AV_CODEC_ID_ROQ_DPCM          ,  "roq"   ) ;
  EF ( AV_CODEC_ID_INTERPLAY_DPCM    ,  "dpcm"  ) ;
  EF ( AV_CODEC_ID_XAN_DPCM          ,  "dpcm"  ) ;
  EF ( AV_CODEC_ID_SOL_DPCM          ,  "dpcm"  ) ;
  EF ( AV_CODEC_ID_MP2               ,  "mp2"   ) ;
  EF ( AV_CODEC_ID_MP3               ,  "mp3"   ) ;
  EF ( AV_CODEC_ID_AAC               ,  "aac"   ) ;
  EF ( AV_CODEC_ID_AC3               ,  "ac3"   ) ;
  EF ( AV_CODEC_ID_DTS               ,  "dts"   ) ;
  EF ( AV_CODEC_ID_VORBIS            ,  "vor"   ) ;
  EF ( AV_CODEC_ID_DVAUDIO           ,  "dva"   ) ;
  EF ( AV_CODEC_ID_WMAV1             ,  "wma"   ) ;
  EF ( AV_CODEC_ID_WMAV2             ,  "wma"   ) ;
  EF ( AV_CODEC_ID_MACE3             ,  "ace3"  ) ;
  EF ( AV_CODEC_ID_MACE6             ,  "ace6"  ) ;
  EF ( AV_CODEC_ID_VMDAUDIO          ,  "vma"   ) ;
  EF ( AV_CODEC_ID_FLAC              ,  "flac"  ) ;
  EF ( AV_CODEC_ID_MP3ADU            ,  "mp3"   ) ;
  EF ( AV_CODEC_ID_MP3ON4            ,  "mp3"   ) ;
  EF ( AV_CODEC_ID_SHORTEN           ,  "short" ) ;
  EF ( AV_CODEC_ID_ALAC              ,  "alac"  ) ;
  EF ( AV_CODEC_ID_WESTWOOD_SND1     ,  "wsnd"  ) ;
  EF ( AV_CODEC_ID_GSM               ,  "gsm"   ) ;
  EF ( AV_CODEC_ID_QDM2              ,  "qdm2"  ) ;
  EF ( AV_CODEC_ID_COOK              ,  "cook"  ) ;
  EF ( AV_CODEC_ID_TRUESPEECH        ,  "trs"   ) ;
  EF ( AV_CODEC_ID_TTA               ,  "tta"   ) ;
  EF ( AV_CODEC_ID_SMACKAUDIO        ,  "saa"   ) ;
  EF ( AV_CODEC_ID_QCELP             ,  "qcp"   ) ;
  EF ( AV_CODEC_ID_WAVPACK           ,  "wv"    ) ;
  EF ( AV_CODEC_ID_DSICINAUDIO       ,  "dsa"   ) ;
  EF ( AV_CODEC_ID_IMC               ,  "imc"   ) ;
  EF ( AV_CODEC_ID_MUSEPACK7         ,  "mp7"   ) ;
  EF ( AV_CODEC_ID_MLP               ,  "mlp"   ) ;
  EF ( AV_CODEC_ID_GSM_MS            ,  "gsm"   ) ;
  EF ( AV_CODEC_ID_ATRAC3            ,  "ac3"   ) ;
  EF ( AV_CODEC_ID_APE               ,  "ape"   ) ;
  EF ( AV_CODEC_ID_NELLYMOSER        ,  "nmo"   ) ;
  EF ( AV_CODEC_ID_MUSEPACK8         ,  "mp8"   ) ;
  EF ( AV_CODEC_ID_SPEEX             ,  "speex" ) ;
  EF ( AV_CODEC_ID_WMAVOICE          ,  "wma"   ) ;
  EF ( AV_CODEC_ID_WMAPRO            ,  "wma"   ) ;
  EF ( AV_CODEC_ID_WMALOSSLESS       ,  "wma"   ) ;
  EF ( AV_CODEC_ID_ATRAC3P           ,  "ac3"   ) ;
  EF ( AV_CODEC_ID_EAC3              ,  "eac3"  ) ;
  EF ( AV_CODEC_ID_SIPR              ,  "sipr"  ) ;
  EF ( AV_CODEC_ID_MP1               ,  "mp1"   ) ;
  EF ( AV_CODEC_ID_TWINVQ            ,  "vqf"   ) ;
  EF ( AV_CODEC_ID_TRUEHD            ,  "thd"   ) ;
  EF ( AV_CODEC_ID_MP4ALS            ,  "mp4"   ) ;
  EF ( AV_CODEC_ID_ATRAC1            ,  "atrac" ) ;
  EF ( AV_CODEC_ID_BINKAUDIO_RDFT    ,  "rdft"  ) ;
  EF ( AV_CODEC_ID_BINKAUDIO_DCT     ,  "dct"   ) ;
  EF ( AV_CODEC_ID_AAC_LATM          ,  "aac"   ) ;
  EF ( AV_CODEC_ID_QDMC              ,  "qdmc"  ) ;
  EF ( AV_CODEC_ID_CELT              ,  "celt"  ) ;
  EF ( AV_CODEC_ID_G723_1            ,  "g723"  ) ;
  EF ( AV_CODEC_ID_G729              ,  "g729"  ) ;
  EF ( AV_CODEC_ID_8SVX_EXP          ,  "svx"   ) ;
  EF ( AV_CODEC_ID_8SVX_FIB          ,  "svx"   ) ;
  EF ( AV_CODEC_ID_BMV_AUDIO         ,  "bmv"   ) ;
  EF ( AV_CODEC_ID_RALF              ,  "ralf"  ) ;
  EF ( AV_CODEC_ID_IAC               ,  "iac"   ) ;
  EF ( AV_CODEC_ID_ILBC              ,  "ilbc"  ) ;
  EF ( AV_CODEC_ID_METASOUND         ,  "mts"   ) ;
  EF ( AV_CODEC_ID_FFWAVESYNTH       ,  "ffw"   ) ;
  EF ( AV_CODEC_ID_SONIC             ,  "sonic" ) ;
  EF ( AV_CODEC_ID_SONIC_LS          ,  "sonic" ) ;
  EF ( AV_CODEC_ID_PAF_AUDIO         ,  "paf"   ) ;
  EF ( AV_CODEC_ID_OPUS              ,  "opus"  ) ;
  EF ( AV_CODEC_ID_TAK               ,  "tak"   ) ;
  EF ( AV_CODEC_ID_EVRC              ,  "evrc"  ) ;
  #undef  EF
  return      filters                             ;
}
