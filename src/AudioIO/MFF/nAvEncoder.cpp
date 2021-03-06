#include <qtaudioio.h>

N::AvEncoder:: AvEncoder       (void)
             : FFmpeg::Encoder (    )
{
}

N::AvEncoder::~AvEncoder(void)
{
}

void N::AvEncoder::setEncoders(IMAPs & Encoders,int DeviceType)
{
  switch (DeviceType)                       {
    case 0                                  :
      DeviceType = AVMEDIA_TYPE_AUDIO       ;
    break                                   ;
    case 1                                  :
      DeviceType = AVMEDIA_TYPE_VIDEO       ;
    break                                   ;
    case 2                                  :
      DeviceType = AVMEDIA_TYPE_SUBTITLE    ;
    break                                   ;
    default                                 :
    return                                  ;
  }                                         ;
  ///////////////////////////////////////////
  #define EC(ID,CID)                        \
    if (exists(CID,DeviceType))           { \
      Encoders[Encoders::ID] = CID        ; \
    }
  EC(MPEG1VIDEO,AV_CODEC_ID_MPEG1VIDEO);
  EC(MPEG2VIDEO,AV_CODEC_ID_MPEG2VIDEO);
  EC(H261,AV_CODEC_ID_H261);
  EC(H263,AV_CODEC_ID_H263);
  EC(RV10,AV_CODEC_ID_RV10);
  EC(RV20,AV_CODEC_ID_RV20);
  EC(MJPEG,AV_CODEC_ID_MJPEG);
  EC(MJPEGB,AV_CODEC_ID_MJPEGB);
  EC(LJPEG,AV_CODEC_ID_LJPEG);
  EC(SP5X,AV_CODEC_ID_SP5X);
  EC(JPEGLS,AV_CODEC_ID_JPEGLS);
  EC(MPEG4,AV_CODEC_ID_MPEG4);
  EC(RAWVIDEO,AV_CODEC_ID_RAWVIDEO);
  EC(MSMPEG4V1,AV_CODEC_ID_MSMPEG4V1);
  EC(MSMPEG4V2,AV_CODEC_ID_MSMPEG4V2);
  EC(MSMPEG4V3,AV_CODEC_ID_MSMPEG4V3);
  EC(WMV1,AV_CODEC_ID_WMV1);
  EC(WMV2,AV_CODEC_ID_WMV2);
  EC(H263P,AV_CODEC_ID_H263P);
  EC(H263I,AV_CODEC_ID_H263I);
  EC(FLV1,AV_CODEC_ID_FLV1);
  EC(SVQ1,AV_CODEC_ID_SVQ1);
  EC(SVQ3,AV_CODEC_ID_SVQ3);
  EC(DVVIDEO,AV_CODEC_ID_DVVIDEO);
  EC(HUFFYUV,AV_CODEC_ID_HUFFYUV);
  EC(CYUV,AV_CODEC_ID_CYUV);
  EC(H264,AV_CODEC_ID_H264);
  EC(INDEO3,AV_CODEC_ID_INDEO3);
  EC(VP3,AV_CODEC_ID_VP3);
  EC(THEORA,AV_CODEC_ID_THEORA);
  EC(ASV1,AV_CODEC_ID_ASV1);
  EC(ASV2,AV_CODEC_ID_ASV2);
  EC(FFV1,AV_CODEC_ID_FFV1);
  EC(N4XM,AV_CODEC_ID_4XM);
  EC(VCR1,AV_CODEC_ID_VCR1);
  EC(CLJR,AV_CODEC_ID_CLJR);
  EC(MDEC,AV_CODEC_ID_MDEC);
  EC(ROQ,AV_CODEC_ID_ROQ);
  EC(INTERPLAY_VIDEO,AV_CODEC_ID_INTERPLAY_VIDEO);
  EC(XAN_WC3,AV_CODEC_ID_XAN_WC3);
  EC(XAN_WC4,AV_CODEC_ID_XAN_WC4);
  EC(RPZA,AV_CODEC_ID_RPZA);
  EC(CINEPAK,AV_CODEC_ID_CINEPAK);
  EC(WS_VQA,AV_CODEC_ID_WS_VQA);
  EC(MSRLE,AV_CODEC_ID_MSRLE);
  EC(MSVIDEO1,AV_CODEC_ID_MSVIDEO1);
  EC(IDCIN,AV_CODEC_ID_IDCIN);
  EC(N8BPS,AV_CODEC_ID_8BPS);
  EC(SMC,AV_CODEC_ID_SMC);
  EC(FLIC,AV_CODEC_ID_FLIC);
  EC(TRUEMOTION1,AV_CODEC_ID_TRUEMOTION1);
  EC(VMDVIDEO,AV_CODEC_ID_VMDVIDEO);
  EC(MSZH,AV_CODEC_ID_MSZH);
  EC(ZLIB,AV_CODEC_ID_ZLIB);
  EC(QTRLE,AV_CODEC_ID_QTRLE);
  EC(TSCC,AV_CODEC_ID_TSCC);
  EC(ULTI,AV_CODEC_ID_ULTI);
  EC(QDRAW,AV_CODEC_ID_QDRAW);
  EC(VIXL,AV_CODEC_ID_VIXL);
  EC(QPEG,AV_CODEC_ID_QPEG);
  EC(PNG,AV_CODEC_ID_PNG);
  EC(PPM,AV_CODEC_ID_PPM);
  EC(PBM,AV_CODEC_ID_PBM);
  EC(PGM,AV_CODEC_ID_PGM);
  EC(PGMYUV,AV_CODEC_ID_PGMYUV);
  EC(PAM,AV_CODEC_ID_PAM);
  EC(FFVHUFF,AV_CODEC_ID_FFVHUFF);
  EC(RV30,AV_CODEC_ID_RV30);
  EC(RV40,AV_CODEC_ID_RV40);
  EC(VC1,AV_CODEC_ID_VC1);
  EC(WMV3,AV_CODEC_ID_WMV3);
  EC(LOCO,AV_CODEC_ID_LOCO);
  EC(WNV1,AV_CODEC_ID_WNV1);
  EC(AASC,AV_CODEC_ID_AASC);
  EC(INDEO2,AV_CODEC_ID_INDEO2);
  EC(FRAPS,AV_CODEC_ID_FRAPS);
  EC(TRUEMOTION2,AV_CODEC_ID_TRUEMOTION2);
  EC(BMP,AV_CODEC_ID_BMP);
  EC(CSCD,AV_CODEC_ID_CSCD);
  EC(MMVIDEO,AV_CODEC_ID_MMVIDEO);
  EC(ZMBV,AV_CODEC_ID_ZMBV);
  EC(AVS,AV_CODEC_ID_AVS);
  EC(SMACKVIDEO,AV_CODEC_ID_SMACKVIDEO);
  EC(NUV,AV_CODEC_ID_NUV);
  EC(KMVC,AV_CODEC_ID_KMVC);
  EC(FLASHSV,AV_CODEC_ID_FLASHSV);
  EC(CAVS,AV_CODEC_ID_CAVS);
  EC(JPEG2000,AV_CODEC_ID_JPEG2000);
  EC(VMNC,AV_CODEC_ID_VMNC);
  EC(VP5,AV_CODEC_ID_VP5);
  EC(VP6,AV_CODEC_ID_VP6);
  EC(VP6F,AV_CODEC_ID_VP6F);
  EC(TARGA,AV_CODEC_ID_TARGA);
  EC(DSICINVIDEO,AV_CODEC_ID_DSICINVIDEO);
  EC(TIERTEXSEQVIDEO,AV_CODEC_ID_TIERTEXSEQVIDEO);
  EC(TIFF,AV_CODEC_ID_TIFF);
  EC(GIF,AV_CODEC_ID_GIF);
  EC(DXA,AV_CODEC_ID_DXA);
  EC(DNXHD,AV_CODEC_ID_DNXHD);
  EC(THP,AV_CODEC_ID_THP);
  EC(SGI,AV_CODEC_ID_SGI);
  EC(C93,AV_CODEC_ID_C93);
  EC(BETHSOFTVID,AV_CODEC_ID_BETHSOFTVID);
  EC(PTX,AV_CODEC_ID_PTX);
  EC(TXD,AV_CODEC_ID_TXD);
  EC(VP6A,AV_CODEC_ID_VP6A);
  EC(AMV,AV_CODEC_ID_AMV);
  EC(VB,AV_CODEC_ID_VB);
  EC(PCX,AV_CODEC_ID_PCX);
  EC(SUNRAST,AV_CODEC_ID_SUNRAST);
  EC(INDEO4,AV_CODEC_ID_INDEO4);
  EC(INDEO5,AV_CODEC_ID_INDEO5);
  EC(MIMIC,AV_CODEC_ID_MIMIC);
  EC(RL2,AV_CODEC_ID_RL2);
  EC(ESCAPE124,AV_CODEC_ID_ESCAPE124);
  EC(DIRAC,AV_CODEC_ID_DIRAC);
  EC(BFI,AV_CODEC_ID_BFI);
  EC(CMV,AV_CODEC_ID_CMV);
  EC(MOTIONPIXELS,AV_CODEC_ID_MOTIONPIXELS);
  EC(TGV,AV_CODEC_ID_TGV);
  EC(TGQ,AV_CODEC_ID_TGQ);
  EC(TQI,AV_CODEC_ID_TQI);
  EC(AURA,AV_CODEC_ID_AURA);
  EC(AURA2,AV_CODEC_ID_AURA2);
  EC(V210X,AV_CODEC_ID_V210X);
  EC(TMV,AV_CODEC_ID_TMV);
  EC(V210,AV_CODEC_ID_V210);
  EC(DPX,AV_CODEC_ID_DPX);
  EC(MAD,AV_CODEC_ID_MAD);
  EC(FRWU,AV_CODEC_ID_FRWU);
  EC(FLASHSV2,AV_CODEC_ID_FLASHSV2);
  EC(CDGRAPHICS,AV_CODEC_ID_CDGRAPHICS);
  EC(R210,AV_CODEC_ID_R210);
  EC(ANM,AV_CODEC_ID_ANM);
  EC(BINKVIDEO,AV_CODEC_ID_BINKVIDEO);
  EC(IFF_ILBM,AV_CODEC_ID_IFF_ILBM);
  EC(IFF_BYTERUN1,AV_CODEC_ID_IFF_BYTERUN1);
  EC(KGV1,AV_CODEC_ID_KGV1);
  EC(YOP,AV_CODEC_ID_YOP);
  EC(VP8,AV_CODEC_ID_VP8);
  EC(PICTOR,AV_CODEC_ID_PICTOR);
  EC(ANSI,AV_CODEC_ID_ANSI);
  EC(A64_MULTI,AV_CODEC_ID_A64_MULTI);
  EC(A64_MULTI5,AV_CODEC_ID_A64_MULTI5);
  EC(R10K,AV_CODEC_ID_R10K);
  EC(MXPEG,AV_CODEC_ID_MXPEG);
  EC(LAGARITH,AV_CODEC_ID_LAGARITH);
  EC(PRORES,AV_CODEC_ID_PRORES);
  EC(JV,AV_CODEC_ID_JV);
  EC(DFA,AV_CODEC_ID_DFA);
  EC(WMV3IMAGE,AV_CODEC_ID_WMV3IMAGE);
  EC(VC1IMAGE,AV_CODEC_ID_VC1IMAGE);
  EC(UTVIDEO,AV_CODEC_ID_UTVIDEO);
  EC(BMV_VIDEO,AV_CODEC_ID_BMV_VIDEO);
  EC(VBLE,AV_CODEC_ID_VBLE);
  EC(DXTORY,AV_CODEC_ID_DXTORY);
  EC(V410,AV_CODEC_ID_V410);
  EC(XWD,AV_CODEC_ID_XWD);
  EC(CDXL,AV_CODEC_ID_CDXL);
  EC(XBM,AV_CODEC_ID_XBM);
  EC(ZEROCODEC,AV_CODEC_ID_ZEROCODEC);
  EC(MSS1,AV_CODEC_ID_MSS1);
  EC(MSA1,AV_CODEC_ID_MSA1);
  EC(TSCC2,AV_CODEC_ID_TSCC2);
  EC(MTS2,AV_CODEC_ID_MTS2);
  EC(CLLC,AV_CODEC_ID_CLLC);
  EC(MSS2,AV_CODEC_ID_MSS2);
  EC(VP9,AV_CODEC_ID_VP9);
  EC(AIC,AV_CODEC_ID_AIC);
#ifdef Q_OS_WIN
  EC(HNM4_VIDEO,AV_CODEC_ID_HNM4_VIDEO);
#endif
  EC(BRENDER_PIX,AV_CODEC_ID_BRENDER_PIX);
  EC(Y41P,AV_CODEC_ID_Y41P);
  EC(ESCAPE130,AV_CODEC_ID_ESCAPE130);
  EC(EXR,AV_CODEC_ID_EXR);
  EC(AVRP,AV_CODEC_ID_AVRP);
  EC(N012V,AV_CODEC_ID_012V);
  EC(G2M,AV_CODEC_ID_G2M);
  EC(AVUI,AV_CODEC_ID_AVUI);
  EC(AYUV,AV_CODEC_ID_AYUV);
  EC(TARGA_Y216,AV_CODEC_ID_TARGA_Y216);
  EC(V308,AV_CODEC_ID_V308);
  EC(V408,AV_CODEC_ID_V408);
  EC(YUV4,AV_CODEC_ID_YUV4);
  EC(SANM,AV_CODEC_ID_SANM);
  EC(PAF_VIDEO,AV_CODEC_ID_PAF_VIDEO);
  EC(AVRN,AV_CODEC_ID_AVRN);
  EC(CPIA,AV_CODEC_ID_CPIA);
  EC(XFACE,AV_CODEC_ID_XFACE);
  EC(SGIRLE,AV_CODEC_ID_SGIRLE);
  EC(MVC1,AV_CODEC_ID_MVC1);
  EC(MVC2,AV_CODEC_ID_MVC2);
  EC(SNOW,AV_CODEC_ID_SNOW);
  EC(WEBP,AV_CODEC_ID_WEBP);
  EC(SMVJPEG,AV_CODEC_ID_SMVJPEG);
  EC(HEVC,AV_CODEC_ID_HEVC);
  EC(FIRST_AUDIO,AV_CODEC_ID_FIRST_AUDIO);
  EC(PCM_S16LE,AV_CODEC_ID_PCM_S16LE);
  EC(PCM_S16BE,AV_CODEC_ID_PCM_S16BE);
  EC(PCM_U16LE,AV_CODEC_ID_PCM_U16LE);
  EC(PCM_U16BE,AV_CODEC_ID_PCM_U16BE);
  EC(PCM_S8,AV_CODEC_ID_PCM_S8);
  EC(PCM_U8,AV_CODEC_ID_PCM_U8);
  EC(PCM_MULAW,AV_CODEC_ID_PCM_MULAW);
  EC(PCM_ALAW,AV_CODEC_ID_PCM_ALAW);
  EC(PCM_S32LE,AV_CODEC_ID_PCM_S32LE);
  EC(PCM_S32BE,AV_CODEC_ID_PCM_S32BE);
  EC(PCM_U32LE,AV_CODEC_ID_PCM_U32LE);
  EC(PCM_U32BE,AV_CODEC_ID_PCM_U32BE);
  EC(PCM_S24LE,AV_CODEC_ID_PCM_S24LE);
  EC(PCM_S24BE,AV_CODEC_ID_PCM_S24BE);
  EC(PCM_U24LE,AV_CODEC_ID_PCM_U24LE);
  EC(PCM_U24BE,AV_CODEC_ID_PCM_U24BE);
  EC(PCM_S24DAUD,AV_CODEC_ID_PCM_S24DAUD);
  EC(PCM_ZORK,AV_CODEC_ID_PCM_ZORK);
  EC(PCM_S16LE_PLANAR,AV_CODEC_ID_PCM_S16LE_PLANAR);
  EC(PCM_DVD,AV_CODEC_ID_PCM_DVD);
  EC(PCM_F32BE,AV_CODEC_ID_PCM_F32BE);
  EC(PCM_F32LE,AV_CODEC_ID_PCM_F32LE);
  EC(PCM_F64BE,AV_CODEC_ID_PCM_F64BE);
  EC(PCM_F64LE,AV_CODEC_ID_PCM_F64LE);
  EC(PCM_BLURAY,AV_CODEC_ID_PCM_BLURAY);
  EC(PCM_LXF,AV_CODEC_ID_PCM_LXF);
  EC(S302M,AV_CODEC_ID_S302M);
  EC(PCM_S8_PLANAR,AV_CODEC_ID_PCM_S8_PLANAR);
  EC(PCM_S24LE_PLANAR,AV_CODEC_ID_PCM_S24LE_PLANAR);
  EC(PCM_S32LE_PLANAR,AV_CODEC_ID_PCM_S32LE_PLANAR);
  EC(PCM_S16BE_PLANAR,AV_CODEC_ID_PCM_S16BE_PLANAR);
  EC(ADPCM_IMA_QT,AV_CODEC_ID_ADPCM_IMA_QT);
  EC(ADPCM_IMA_WAV,AV_CODEC_ID_ADPCM_IMA_WAV);
  EC(ADPCM_IMA_DK3,AV_CODEC_ID_ADPCM_IMA_DK3);
  EC(ADPCM_IMA_DK4,AV_CODEC_ID_ADPCM_IMA_DK4);
  EC(ADPCM_IMA_WS,AV_CODEC_ID_ADPCM_IMA_WS);
  EC(ADPCM_IMA_SMJPEG,AV_CODEC_ID_ADPCM_IMA_SMJPEG);
  EC(ADPCM_MS,AV_CODEC_ID_ADPCM_MS);
  EC(ADPCM_4XM,AV_CODEC_ID_ADPCM_4XM);
  EC(ADPCM_XA,AV_CODEC_ID_ADPCM_XA);
  EC(ADPCM_ADX,AV_CODEC_ID_ADPCM_ADX);
  EC(ADPCM_EA,AV_CODEC_ID_ADPCM_EA);
  EC(ADPCM_G726,AV_CODEC_ID_ADPCM_G726);
  EC(ADPCM_CT,AV_CODEC_ID_ADPCM_CT);
  EC(ADPCM_SWF,AV_CODEC_ID_ADPCM_SWF);
  EC(ADPCM_YAMAHA,AV_CODEC_ID_ADPCM_YAMAHA);
  EC(ADPCM_SBPRO_4,AV_CODEC_ID_ADPCM_SBPRO_4);
  EC(ADPCM_SBPRO_3,AV_CODEC_ID_ADPCM_SBPRO_3);
  EC(ADPCM_SBPRO_2,AV_CODEC_ID_ADPCM_SBPRO_2);
  EC(ADPCM_THP,AV_CODEC_ID_ADPCM_THP);
  EC(ADPCM_IMA_AMV,AV_CODEC_ID_ADPCM_IMA_AMV);
  EC(ADPCM_EA_R1,AV_CODEC_ID_ADPCM_EA_R1);
  EC(ADPCM_EA_R3,AV_CODEC_ID_ADPCM_EA_R3);
  EC(ADPCM_EA_R2,AV_CODEC_ID_ADPCM_EA_R2);
  EC(ADPCM_IMA_EA_SEAD,AV_CODEC_ID_ADPCM_IMA_EA_SEAD);
  EC(ADPCM_IMA_EA_EACS,AV_CODEC_ID_ADPCM_IMA_EA_EACS);
  EC(ADPCM_EA_XAS,AV_CODEC_ID_ADPCM_EA_XAS);
  EC(ADPCM_EA_MAXIS_XA,AV_CODEC_ID_ADPCM_EA_MAXIS_XA);
  EC(ADPCM_IMA_ISS,AV_CODEC_ID_ADPCM_IMA_ISS);
  EC(ADPCM_G722,AV_CODEC_ID_ADPCM_G722);
  EC(ADPCM_IMA_APC,AV_CODEC_ID_ADPCM_IMA_APC);
  EC(VIMA,AV_CODEC_ID_VIMA);
  EC(ADPCM_AFC,AV_CODEC_ID_ADPCM_AFC);
  EC(ADPCM_IMA_OKI,AV_CODEC_ID_ADPCM_IMA_OKI);
  EC(ADPCM_DTK,AV_CODEC_ID_ADPCM_DTK);
  EC(ADPCM_IMA_RAD,AV_CODEC_ID_ADPCM_IMA_RAD);
  EC(ADPCM_G726LE,AV_CODEC_ID_ADPCM_G726LE);
  EC(AMR_NB,AV_CODEC_ID_AMR_NB);
  EC(AMR_WB,AV_CODEC_ID_AMR_WB);
  EC(RA_144,AV_CODEC_ID_RA_144);
  EC(RA_288,AV_CODEC_ID_RA_288);
  EC(ROQ_DPCM,AV_CODEC_ID_ROQ_DPCM);
  EC(INTERPLAY_DPCM,AV_CODEC_ID_INTERPLAY_DPCM);
  EC(XAN_DPCM,AV_CODEC_ID_XAN_DPCM);
  EC(SOL_DPCM,AV_CODEC_ID_SOL_DPCM);
  EC(MP2,AV_CODEC_ID_MP2);
  EC(MP3,AV_CODEC_ID_MP3);
  EC(AAC,AV_CODEC_ID_AAC);
  EC(AC3,AV_CODEC_ID_AC3);
  EC(DTS,AV_CODEC_ID_DTS);
  EC(VORBIS,AV_CODEC_ID_VORBIS);
  EC(DVAUDIO,AV_CODEC_ID_DVAUDIO);
  EC(WMAV1,AV_CODEC_ID_WMAV1);
  EC(WMAV2,AV_CODEC_ID_WMAV2);
  EC(MACE3,AV_CODEC_ID_MACE3);
  EC(MACE6,AV_CODEC_ID_MACE6);
  EC(VMDAUDIO,AV_CODEC_ID_VMDAUDIO);
  EC(FLAC,AV_CODEC_ID_FLAC);
  EC(MP3ADU,AV_CODEC_ID_MP3ADU);
  EC(MP3ON4,AV_CODEC_ID_MP3ON4);
  EC(SHORTEN,AV_CODEC_ID_SHORTEN);
  EC(ALAC,AV_CODEC_ID_ALAC);
  EC(WESTWOOD_SND1,AV_CODEC_ID_WESTWOOD_SND1);
  EC(GSM,AV_CODEC_ID_GSM);
  EC(QDM2,AV_CODEC_ID_QDM2);
  EC(COOK,AV_CODEC_ID_COOK);
  EC(TRUESPEECH,AV_CODEC_ID_TRUESPEECH);
  EC(TTA,AV_CODEC_ID_TTA);
  EC(SMACKAUDIO,AV_CODEC_ID_SMACKAUDIO);
  EC(QCELP,AV_CODEC_ID_QCELP);
  EC(WAVPACK,AV_CODEC_ID_WAVPACK);
  EC(DSICINAUDIO,AV_CODEC_ID_DSICINAUDIO);
  EC(IMC,AV_CODEC_ID_IMC);
  EC(MUSEPACK7,AV_CODEC_ID_MUSEPACK7);
  EC(MLP,AV_CODEC_ID_MLP);
  EC(GSM_MS,AV_CODEC_ID_GSM_MS);
  EC(ATRAC3,AV_CODEC_ID_ATRAC3);
  EC(APE,AV_CODEC_ID_APE);
  EC(NELLYMOSER,AV_CODEC_ID_NELLYMOSER);
  EC(MUSEPACK8,AV_CODEC_ID_MUSEPACK8);
  EC(SPEEX,AV_CODEC_ID_SPEEX);
  EC(WMAVOICE,AV_CODEC_ID_WMAVOICE);
  EC(WMAPRO,AV_CODEC_ID_WMAPRO);
  EC(WMALOSSLESS,AV_CODEC_ID_WMALOSSLESS);
  EC(ATRAC3P,AV_CODEC_ID_ATRAC3P);
  EC(EAC3,AV_CODEC_ID_EAC3);
  EC(SIPR,AV_CODEC_ID_SIPR);
  EC(MP1,AV_CODEC_ID_MP1);
  EC(TWINVQ,AV_CODEC_ID_TWINVQ);
  EC(TRUEHD,AV_CODEC_ID_TRUEHD);
  EC(MP4ALS,AV_CODEC_ID_MP4ALS);
  EC(ATRAC1,AV_CODEC_ID_ATRAC1);
  EC(BINKAUDIO_RDFT,AV_CODEC_ID_BINKAUDIO_RDFT);
  EC(BINKAUDIO_DCT,AV_CODEC_ID_BINKAUDIO_DCT);
  EC(AAC_LATM,AV_CODEC_ID_AAC_LATM);
  EC(QDMC,AV_CODEC_ID_QDMC);
  EC(CELT,AV_CODEC_ID_CELT);
  EC(G723_1,AV_CODEC_ID_G723_1);
  EC(G729,AV_CODEC_ID_G729);
  EC(N8SVX_EXP,AV_CODEC_ID_8SVX_EXP);
  EC(N8SVX_FIB,AV_CODEC_ID_8SVX_FIB);
  EC(BMV_AUDIO,AV_CODEC_ID_BMV_AUDIO);
  EC(RALF,AV_CODEC_ID_RALF);
  EC(IAC,AV_CODEC_ID_IAC);
  EC(ILBC,AV_CODEC_ID_ILBC);
  EC(METASOUND,AV_CODEC_ID_METASOUND);
  EC(FFWAVESYNTH,AV_CODEC_ID_FFWAVESYNTH);
  EC(SONIC,AV_CODEC_ID_SONIC);
  EC(SONIC_LS,AV_CODEC_ID_SONIC_LS);
  EC(PAF_AUDIO,AV_CODEC_ID_PAF_AUDIO);
  EC(OPUS,AV_CODEC_ID_OPUS);
  EC(TAK,AV_CODEC_ID_TAK);
  EC(EVRC,AV_CODEC_ID_EVRC);
  EC(SMV,AV_CODEC_ID_SMV);
  EC(FIRST_SUBTITLE,AV_CODEC_ID_FIRST_SUBTITLE);
  EC(DVD_SUBTITLE,AV_CODEC_ID_DVD_SUBTITLE);
  EC(DVB_SUBTITLE,AV_CODEC_ID_DVB_SUBTITLE);
  EC(TEXT,AV_CODEC_ID_TEXT);
  EC(XSUB,AV_CODEC_ID_XSUB);
  EC(SSA,AV_CODEC_ID_SSA);
  EC(MOV_TEXT,AV_CODEC_ID_MOV_TEXT);
  EC(HDMV_PGS_SUBTITLE,AV_CODEC_ID_HDMV_PGS_SUBTITLE);
  EC(DVB_TELETEXT,AV_CODEC_ID_DVB_TELETEXT);
  EC(SRT,AV_CODEC_ID_SRT);
  EC(MICRODVD,AV_CODEC_ID_MICRODVD);
  EC(EIA_608,AV_CODEC_ID_EIA_608);
  EC(JACOSUB,AV_CODEC_ID_JACOSUB);
  EC(SAMI,AV_CODEC_ID_SAMI);
  EC(REALTEXT,AV_CODEC_ID_REALTEXT);
  EC(SUBVIEWER1,AV_CODEC_ID_SUBVIEWER1);
  EC(SUBVIEWER,AV_CODEC_ID_SUBVIEWER);
  EC(SUBRIP,AV_CODEC_ID_SUBRIP);
  EC(WEBVTT,AV_CODEC_ID_WEBVTT);
  EC(MPL2,AV_CODEC_ID_MPL2);
  EC(VPLAYER,AV_CODEC_ID_VPLAYER);
  EC(PJS,AV_CODEC_ID_PJS);
  EC(ASS,AV_CODEC_ID_ASS);
  EC(FIRST_UNKNOWN,AV_CODEC_ID_FIRST_UNKNOWN);
  EC(TTF,AV_CODEC_ID_TTF);
  EC(BINTEXT,AV_CODEC_ID_BINTEXT);
  EC(XBIN,AV_CODEC_ID_XBIN);
  EC(IDF,AV_CODEC_ID_IDF);
  EC(OTF,AV_CODEC_ID_OTF);
  EC(SMPTE_KLV,AV_CODEC_ID_SMPTE_KLV);
  EC(DVD_NAV,AV_CODEC_ID_DVD_NAV);
  #undef  EC
}
