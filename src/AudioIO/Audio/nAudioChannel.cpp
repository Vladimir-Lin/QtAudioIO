#include <qtaudioio.h>

N::AudioChannel:: AudioChannel(void )
                : Mute        (false)
{
}

N::AudioChannel::~AudioChannel(void)
{
}

int N::AudioChannel::type(void) const
{
  return 0 ;
}
