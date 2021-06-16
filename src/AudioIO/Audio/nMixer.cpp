#include <Exciton>

nMixer:: nMixer(void)
{
  Device = "";
}

nMixer::~nMixer(void)
{
}

bool nMixer::open(void)
{
  #if defined(WIN64) || defined(WIN32)
  if (MMSYSERR_NOERROR!=::mixerOpen(&mixerHandle,0,0,0,0)) return false;
  if (MMSYSERR_NOERROR!=::mixerGetDevCaps(
                          (UINT)mixerHandle ,
                          &mixCaps          ,
                          sizeof(MIXERCAPS)
                        )) return false;
  mixerLine.cbStruct        = sizeof(MIXERLINE);
  mixerLine.dwComponentType = MIXERLINE_COMPONENTTYPE_DST_SPEAKERS;
  if (MMSYSERR_NOERROR!=::mixerGetLineInfo(
                          (HMIXEROBJ)mixerHandle,
                          &mixerLine,
                          MIXER_GETLINEINFOF_COMPONENTTYPE
                        )) return false;
  #endif
  #ifdef MACOSX
  #endif
  #ifdef LINUX
  #endif
  return true;
}

bool nMixer::close(void)
{
  #if defined(WIN64) || defined(WIN32)
  if (MMSYSERR_INVALHANDLE==::mixerClose(mixerHandle)) return false;
  #endif
  #ifdef MACOSX
  #endif
  #ifdef LINUX
  #endif
  return true;
}

bool nMixer::setMute(void)
{
  #if defined(WIN64) || defined(WIN32)
  mixerLineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_MUTE;
  if (MMSYSERR_NOERROR!=::mixerGetLineControls(
                          (HMIXEROBJ)mixerHandle,
                          &mixerLineControls,
                          MIXER_GETLINECONTROLSF_ONEBYTYPE
                        )) return false;
  MIXERCONTROLDETAILS_BOOLEAN mcb = {0};
  mcb.fValue    = true;
  mixerControlDetails.paDetails = &mcb;
  mixerControlDetails.cbDetails = sizeof(MIXERCONTROLDETAILS_BOOLEAN);
  if (MMSYSERR_NOERROR!=::mixerSetControlDetails(
                          (HMIXEROBJ)mixerHandle,
                          &mixerControlDetails,
                          MIXER_SETCONTROLDETAILSF_VALUE
                        )) return false;
  #endif
  #ifdef MACOSX
  #endif
  #ifdef LINUX
  #endif
  return true;
}

int nMixer::getVolume(void)
{
  long long longVolume = 0;
  #if defined(WIN64) || defined(WIN32)
  mixerLineControls.cbStruct      = sizeof(MIXERLINECONTROLS);
  mixerLineControls.dwLineID      = mixerLine.dwLineID;
  mixerLineControls.dwControlType = MIXERCONTROL_CONTROLTYPE_VOLUME;
  mixerLineControls.cControls     = 1;
  mixerLineControls.pamxctrl      = &volumeControl;
  mixerLineControls.cbmxctrl      = sizeof(MIXERCONTROL);
  if (MMSYSERR_NOERROR!=::mixerGetLineControls(
                          (HMIXEROBJ)mixerHandle,
                          &mixerLineControls,
                          MIXER_GETLINECONTROLSF_ONEBYTYPE
                        )) return 0;
  mixerControlDetails.cbStruct       = sizeof(MIXERCONTROLDETAILS);
  mixerControlDetails.dwControlID    = volumeControl.dwControlID;
  mixerControlDetails.cChannels      = 1;
  mixerControlDetails.cMultipleItems = volumeControl.cMultipleItems;
  mixerControlDetails.paDetails      = &mixerValue;
  mixerControlDetails.cbDetails      = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
  if (MMSYSERR_NOERROR!=::mixerGetControlDetails(
                          (HMIXEROBJ)mixerHandle,
                          &mixerControlDetails,
                          MIXER_GETCONTROLDETAILSF_VALUE
                        )) return 0;
  longVolume    = 100;
  longVolume   *= mixerValue.dwValue;
  longVolume   /= volumeControl.Bounds.dwMaximum;
  #endif
  #ifdef MACOSX
  #endif
  #ifdef LINUX
  #endif
  return (int)longVolume;
}

bool nMixer::setVolume(int Volume)
{
  #if defined(WIN64) || defined(WIN32)
  long long dw;
  MIXERCONTROLDETAILS mcd = {0};
  MIXERCONTROLDETAILS_UNSIGNED mcdu = {0};
  dw              = Volume;
  dw             *= volumeControl.Bounds.dwMaximum;
  dw             /= 100;
  mcdu.dwValue    = dw;
  mcd.cbStruct    = sizeof(MIXERCONTROLDETAILS);
  mcd.hwndOwner   = 0;
  mcd.dwControlID = volumeControl.dwControlID;
  mcd.paDetails   = &mcdu;
  mcd.cbDetails   = sizeof(MIXERCONTROLDETAILS_UNSIGNED);
  mcd.cChannels   = 1;
  if (MMSYSERR_NOERROR!=::mixerSetControlDetails(
                          (HMIXEROBJ)mixerHandle,
                          &mcd,
                          MIXER_SETCONTROLDETAILSF_VALUE
                        )) return false;
  #endif
  #ifdef MACOSX
  #endif
  #ifdef LINUX
  #endif
  return true;
}

void nMixer::mute(void)
{
  #if defined(WIN64) || defined(WIN32)
  if (open()) setMute();
  close();
  #endif
  #ifdef MACOSX
  #endif
  #ifdef LINUX
  #endif
}

int nMixer::volume(void)
{
  int CurrentVolume = 0;
  #if defined(WIN64) || defined(WIN32)
  if (open()) CurrentVolume = getVolume();
  close();
  #endif
  #ifdef MACOSX
  #endif
  #ifdef LINUX
  #endif
  return CurrentVolume;
}

bool nMixer::setValue(int Volume)
{
  bool rt = false;
  #if defined(WIN64) || defined(WIN32)
  if (open()) {
    getVolume();
    rt = setVolume(Volume);
  };
  close();
  #endif
  #ifdef MACOSX
  #endif
  #ifdef LINUX
  #endif
  return rt;
}

bool nMixer::setDevice(QString device)
{
  Device = device;
  return false;
}

QStringList nMixer::devices(void)
{
  QStringList Devices;
  return Devices;
}
