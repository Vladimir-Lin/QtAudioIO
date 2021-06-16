#include <qtaudioio.h>

N::AudioTributary:: AudioTributary (QObject * parent)
                  : AudioIO        (          parent)
{
}

N::AudioTributary::~AudioTributary (void)
{
}

QByteArray & N::AudioTributary::add(char * data,int len)
{
  AudioIO :: add ( data , len ) ;
  distribute     (            ) ;
  return ME                     ;
}

bool N::AudioTributary::fresh(void)
{
  return true ;
}

bool N::AudioTributary::demolish(void)
{
  CUIDs IDs = Acceptors.keys() ;
  int   id                     ;
  foreach (id,IDs)             {
    Acceptors[id]->Demolish()  ;
  }                            ;
  return true                  ;
}

bool N::AudioTributary::Lock(void)
{
  mutex . lock ( ) ;
  return true      ;
}

bool N::AudioTributary::Unlock(void)
{
  mutex . unlock ( ) ;
  return true        ;
}

void N::AudioTributary::single(void)
{
  nDropOut ( !Acceptors.contains(0) ) ;
  int s = AudioSize ( )               ;
  if ( s <= 0 ) return                ;
  char * d = index ( 0 )              ;
  if (IsNull(d)) return               ;
  QByteArray D ( d , s )              ;
  Acceptors [ 0 ] -> Feeding ( D )    ;
}

void N::AudioTributary::tributary(void)
{
  CUIDs  IDs = Acceptors.keys()   ;
  int    id                       ;
  int    s   = Bits  (   ) / 8    ;
  int    b   = s * Channels ( )   ;
  int    f   = Frames(   )        ;
  char * d   = index ( 0 )        ;
  foreach (id,IDs)                {
    if (Acceptors.contains(id))   {
      QByteArray D                ;
      char * v                    ;
      v = d + ( s * id )          ;
      for (int i=0;i<f;i++)       {
        D . append ( v , s )      ;
        v += b                    ;
      }                           ;
      if (D.size()>0)             {
        Acceptors[id]->Feeding(D) ;
      }                           ;
    }                             ;
  }                               ;
}

void N::AudioTributary::distribute(void)
{
  Lock   ( )          ;
  switch (Channels()) {
    case 0            :
    break             ;
    case 1            :
      single      ( ) ;
    break             ;
    default           :
      tributary   ( ) ;
    break             ;
  }                   ;
  AudioData::zero ( ) ;
  Unlock ( )          ;
}
