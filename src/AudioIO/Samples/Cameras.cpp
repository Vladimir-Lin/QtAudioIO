#include <qtaudioio.h>

QList<QByteArray> N::Manager::Cameras (void)
{
  return QCamera::availableDevices() ;
}
