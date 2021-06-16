#include <qtaudioio.h>

N::AudioRecorder:: AudioRecorder (QObject * parent)
                 : AudioIn       (          parent)
{
  WidgetClass ;
}

N::AudioRecorder::~AudioRecorder(void)
{
}

void N::AudioRecorder::Start(void)
{
}

void N::AudioRecorder::Stop(void)
{
}

bool N::AudioRecorder::isRecording(void)
{
  return NotNull ( stream ) ;
}

void N::AudioRecorder::started(void)
{
}

void N::AudioRecorder::waiting(void)
{
}

void N::AudioRecorder::finished(void)
{
}

void N::AudioRecorder::run(void)
{
}
