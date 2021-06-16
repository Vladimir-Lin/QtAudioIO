#include <qtaudioio.h>

bool N::initLIBAV(void)
{
  ::avcodec_register_all  (              ) ;
  ::avdevice_register_all (              ) ;
  ::avfilter_register_all (              ) ;
  ::av_register_all       (              ) ;
  ::av_log_set_level      ( AV_LOG_QUIET ) ;
  return true                              ;
}
