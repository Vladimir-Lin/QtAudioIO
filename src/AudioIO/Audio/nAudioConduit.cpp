#include <qtaudioio.h>

N::AudioConduit:: AudioConduit ( QObject * parent )
                : Conduit      (           parent )
                , channel      ( NULL             )
                , interval     ( 100              )
                , bytes        ( 0                )
                , total        ( 0                )
{
}

N::AudioConduit:: AudioConduit ( QObject * parent , AudioChannel * wave )
                : Conduit      (           parent                       )
                , channel      ( NULL                                   )
                , interval     ( 100                                    )
                , bytes        ( 0                                      )
                , total        ( 0                                      )
{
  assign ( wave ) ;
}

N::AudioConduit:: AudioConduit ( QObject * parent,AudioChannel * wave,int val )
                : Conduit      (           parent                             )
                , channel      ( NULL                                         )
                , interval     ( val                                          )
                , bytes        ( 0                                            )
                , total        ( 0                                            )
{
  assign ( wave ) ;
}

N::AudioConduit::~AudioConduit(void)
{
}

bool N::AudioConduit::Outflow(int junctionType)
{
  nKickOut ( junctionType!=1001 , false ) ;
  return true                             ;
}

bool N::AudioConduit::Into(int junctionType)
{ Q_UNUSED ( junctionType ) ;
  return false              ;
}

bool N::AudioConduit::Throught(Conduit * PreviousConduit)
{ Q_UNUSED ( PreviousConduit ) ;
  return false                 ;
}

bool N::AudioConduit::Has(void)
{
  nKickOut ( IsNull(channel)           , false ) ;
  nKickOut ( !channel->hasData()       , false ) ;
  channel -> obtain ( Blob , bytes )             ;
  TUID bas = Blob . AudioSize ( )                ;
  if (total==0)                                  {
    BaseTime = QDateTime::currentDateTime()      ;
    bas *= 1000000                               ;
    bas /= interval                              ;
    bas /= Blob . BytesPerSecond ( )             ;
    BaseTime = BaseTime.addMSecs((int)-bas)      ;
  }                                              ;
  total += bas                                   ;
  return true                                    ;
}

bool N::AudioConduit::Transit(void)
{ // clear audio data
  if (Blob.AudioSize()>0) {
    Blob . zero ( )       ;
  }                       ;
  return true             ;
}

void N::AudioConduit::assign(AudioChannel * wave)
{
  channel = wave                     ;
  nDropOut ( IsNull(channel) )       ;
  channel -> assign ( Blob   )       ;
  bytes  = Blob . BytesPerSecond ( ) ;
  bytes *= interval                  ;
  bytes /= 1000                      ;
  total  = 0                         ;
}
