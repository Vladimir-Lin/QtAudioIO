#include <qtaudioio.h>

N::AudioJunction:: AudioJunction (QObject * parent)
                 : Junction      (          parent)
{
}

N::AudioJunction::~AudioJunction (void)
{
}

N::AudioChannel * N::AudioJunction::Supply(void)
{
  if (conduits.count()<=0) return NULL               ;
  AudioConduit * ac = NULL                           ;
  for (int i=0;IsNull(ac) && i<conduits.count();i++) {
    if ( 1001 == conduits[i]->type() )               {
      ac = (AudioConduit *)conduits[i]               ;
    }                                                ;
  }                                                  ;
  if (IsNull(ac)) return NULL                        ;
  return ac->channel                                 ;
}
