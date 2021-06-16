#include <qtaudioio.h>

N::AudioAcceptor:: AudioAcceptor ( QObject * parent )
                 : Conduit       (           parent )
                 , audio         ( NULL             )
{
}

N::AudioAcceptor:: AudioAcceptor ( QObject * parent , AudioData * wave )
                 : Conduit       (           parent                    )
                 , audio         (                                wave )
{
}

N::AudioAcceptor::~AudioAcceptor(void)
{
}

bool N::AudioAcceptor::Into(int junctionType)
{
  nKickOut ( junctionType!=1001 , false ) ;
  return true                             ;
}

bool N::AudioAcceptor::Throught(Conduit * conduit)
{
  nKickOut ( IsNull(conduit)       , false )  ;
  nKickOut ( conduit->type()!=1001 , false )  ;
  nKickOut ( IsNull(audio  )       , false )  ;
  AudioConduit * ac = (AudioConduit *)conduit ;
  nKickOut ( IsNull(ac->channel)   , false )  ;
  if (ac->Blob.AudioSize()>0)                 {
    ac -> Blob . Lock   ( )                   ;
    int    bas = ac -> Blob . AudioSize (   ) ;
    char * src = ac -> Blob . index     ( 0 ) ;
    audio -> add           ( src , bas )      ;
    ac    -> Blob . Unlock (           )      ;
    audio -> fresh         (           )      ;
  }                                           ;
  return true                                 ;
}

bool N::AudioAcceptor::Demolish(Conduit * Faucet)
{
  nKickOut ( Faucet->type()!=1001 , false ) ;
  nKickOut ( IsNull(audio)        , false ) ;
  audio -> demolish ( )                     ;
  audio = NULL                              ;
  return true                               ;
}
