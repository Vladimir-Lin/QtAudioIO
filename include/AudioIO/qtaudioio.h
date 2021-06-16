/****************************************************************************
 *                                                                          *
 * Copyright (C) 2016 Neutrino International Inc.                           *
 *                                                                          *
 * Author : Brian Lin <lin.foxman@gmail.com>                                *
 * Skype  : wolfram_lin                                                     *
 *                                                                          *
 ****************************************************************************/

#ifndef QT_AUDIOIO_H
#define QT_AUDIOIO_H

#include <QtFFmpeg>
#include <QtAudio>
#include <Essentials>
#include <Mathematics>

QT_BEGIN_NAMESPACE

#ifndef QT_STATIC
#    if defined(QT_BUILD_AUDIOIO_LIB)
#      define Q_AUDIOIO_EXPORT Q_DECL_EXPORT
#    else
#      define Q_AUDIOIO_EXPORT Q_DECL_IMPORT
#    endif
#else
#    define Q_AUDIOIO_EXPORT
#endif

namespace N
{

class Q_AUDIOIO_EXPORT AudioData      ;
class Q_AUDIOIO_EXPORT AudioConduit   ;
class Q_AUDIOIO_EXPORT AudioAcceptor  ;
class Q_AUDIOIO_EXPORT AudioChannel   ;
class Q_AUDIOIO_EXPORT AudioJunction  ;
class Q_AUDIOIO_EXPORT AudioIO        ;
class Q_AUDIOIO_EXPORT AvDecoder      ;
class Q_AUDIOIO_EXPORT AvEncoder      ;
class Q_AUDIOIO_EXPORT AudioIn        ;
class Q_AUDIOIO_EXPORT AudioOut       ;
class Q_AUDIOIO_EXPORT AudioTributary ;
class Q_AUDIOIO_EXPORT AudioPlayer    ;
class Q_AUDIOIO_EXPORT AudioRecorder  ;
class Q_AUDIOIO_EXPORT AudioJack      ;
class Q_AUDIOIO_EXPORT AudioWriter    ;
class Q_AUDIOIO_EXPORT AudioReader    ;
class Q_AUDIOIO_EXPORT AudioFeeder    ;
class Q_AUDIOIO_EXPORT NotifyPlayer   ;

Q_AUDIOIO_EXPORT bool initLIBAV  (void) ;
Q_AUDIOIO_EXPORT void SilentPlay (QString filename) ;

namespace Manager
{

Q_AUDIOIO_EXPORT ByteArrays Cameras (void) ;

}

namespace Acoustics
{

Q_AUDIOIO_EXPORT extern short AmplitudeDB    [ 88 ] ;
Q_AUDIOIO_EXPORT extern int   SamplingRates  [ 15 ] ;

// convert from decibels to a linear scale factor
Q_AUDIOIO_EXPORT double toLinear (int dB) ; // dB => Linear
Q_AUDIOIO_EXPORT double toLinear (int dB,double factor) ; // dB => Linear , dB * factor

Q_AUDIOIO_EXPORT int    AvFormat (int sampleFormat) ;
Q_AUDIOIO_EXPORT int    AvBits   (int sampleFormat) ;

Q_AUDIOIO_EXPORT CiosAudio::CaSampleFormat toAudioCore(int format) ;

Q_AUDIOIO_EXPORT QStringList ExtensionFilters (int type = 0) ; // 0 - decoder , 1 - encoder

Q_AUDIOIO_EXPORT NAMEs  Devices  (Equipment equipment) ;

Q_AUDIOIO_EXPORT TUID Load (QString filename) ;
Q_AUDIOIO_EXPORT bool Load (QString filename,AudioData & data) ;

}

class Q_AUDIOIO_EXPORT AudioData : public QByteArray
{
  public:

    explicit             AudioData      (void) ;
                         AudioData      (const AudioData & other) ;
    virtual             ~AudioData      (void) ;

    virtual int          BlobType       (void) const { return 0 ; }

    void                 zero           (void) ;
    void                 reset          (void) ;

    void                 set            (int                     rate     ,
                                         int                     bits     ,
                                         int                     channels ,
                                         Acoustics::SampleFormat format ) ;

    int                  Bits           (void) const ;
    int                  Channels       (void) const ;
    int                  Rate           (void) const ;
    int                  Format         (void) const ;
    int                  AudioSize      (void) const ;

    QTime                Duration       (void) ;
    int                  Seconds        (void) ;
    int                  Milliseconds   (void) ;
    int                  Frames         (void) ;

    int                  BytesPerFrame  (void) ;
    int                  BytesPerSecond (void) ;

    bool                 load           (QIODevice & file) ;
    bool                 save           (QIODevice & file) ;

    virtual double       upperBound     (void) ;
    virtual double       lowerBound     (void) ;

    virtual bool         fresh          (void) ;
    virtual bool         demolish       (void) ;

    QByteArray         & assign         (const AudioData & other) ;
    QByteArray         & operator =     (const AudioData & other) ;
    bool                 operator ==    (const AudioData & other) ;
    bool                 equal          (const AudioData & other) ;

    virtual QByteArray & operator +=    (AudioData & other) ;
    virtual QByteArray & add            (AudioData & other) ;
    virtual QByteArray & add            (char * data,int len) ;

    virtual QByteArray & trim           (int fromData,int toData) ;
    int                  takeAt         (int pos,int length) ;

    int                  toVector       (Vector & vector,int channel = -1) ;
    // -1 : all , 0 - channel 0 , 1 - channel 1

    int                  buffer         (int seconds) ;
    char *               index          (int shift  ) ;
    char *               frame          (int atFrame) ;
    virtual bool         Lock           (void) ;
    virtual bool         Unlock         (void) ;

    QByteArray           header         (void) ;

    QAudioFormat         AudioFormat    (void) ;

  protected:

  private:

};

class Q_AUDIOIO_EXPORT AudioConduit : public Conduit
{ // Audio Input Feeder
  Q_OBJECT
  public:

    AudioChannel * channel  ;
    AudioData      Blob     ;
    int            interval ;
    int            bytes    ;
    TUID           total    ;
    QDateTime      BaseTime ;

    explicit AudioConduit  (QObject * parent = NULL) ;
    explicit AudioConduit  (QObject * parent,AudioChannel * wave) ;
    explicit AudioConduit  (QObject * parent,AudioChannel * wave,int interval) ;
    virtual ~AudioConduit  (void) ;

    virtual int  type      (void) const { return 1001 ; }
    virtual bool Outflow   (int junctionType) ;
    virtual bool Into      (int junctionType) ;
    virtual bool Throught  (Conduit * PreviousConduit) ;
    virtual bool Has       (void) ;
    virtual bool Transit   (void) ;

  protected:

  private:

  public slots:

    virtual void assign    (AudioChannel * channel) ;

  protected slots:

  private slots:

  signals:

};

class Q_AUDIOIO_EXPORT AudioAcceptor : public Conduit
{
  Q_OBJECT
  public:

    AudioData * audio ;

    explicit AudioAcceptor  (QObject * parent = NULL) ;
    explicit AudioAcceptor  (QObject * parent,AudioData * wave) ;
    virtual ~AudioAcceptor  (void) ;

    virtual int  type      (void) const { return 1002 ; }
    virtual bool Into      (int junctionType) ;
    virtual bool Throught  (Conduit * PreviousConduit) ;
    virtual bool Demolish  (Conduit * Faucet) ;

  protected:

  private:

  public slots:

  protected slots:

  private slots:

  signals:

};

class Q_AUDIOIO_EXPORT AudioChannel
{
  public:

    bool    Mute           ;
    VarArgs AudioParaments ;

    explicit AudioChannel      (void) ;
    virtual ~AudioChannel      (void) ;

    virtual int     type       (void) const ;
    virtual QString name       (void)   = 0 ;
    virtual QString source     (void)   = 0 ;
    virtual bool    hasData    (void)   = 0 ;
    virtual TUID    length     (void)   = 0 ;
    virtual TUID    bytes      (void)   = 0 ;
    virtual int     rate       (void)   = 0 ;
    virtual int     channels   (void)   = 0 ;
    virtual bool    hasChannel (int id) = 0 ;
    virtual QString channel    (int id) = 0 ;
    virtual bool    canSeek    (void)   = 0 ;
    virtual bool    assign     (AudioData & data) = 0 ;
    virtual bool    obtain     (AudioData & data,int bytes) = 0 ;
    virtual bool    play       (void)   = 0 ;
    virtual bool    Pause      (void)   = 0 ;
    virtual bool    seek       (TUID timestamp) = 0 ;
    virtual bool    Resume     (void)   = 0 ;
    virtual bool    abort      (void)   = 0 ;

  protected:

  private:

};

class Q_AUDIOIO_EXPORT AudioJunction : public Junction
{
  Q_OBJECT
  public:

    explicit AudioJunction (QObject * parent = NULL) ;
    virtual ~AudioJunction (void) ;

    virtual int  type      (void) const { return 1001 ; }

    AudioChannel * Supply  (void) ;

  protected:

  private:

  public slots:

  protected slots:

  private slots:

  signals:

};

class Q_AUDIOIO_EXPORT AudioIO : public QIODevice
                               , public AudioData
                               , public CiosAudio::Conduit
{
  Q_OBJECT
  Q_PROPERTY(ReadMethod method READ Method WRITE setMethod)
  public:

    typedef enum   {
      Removal  = 0 ,
      Indexing = 1 }
      ReadMethod   ;

    explicit       AudioIO      (QObject * parent = NULL) ;
    explicit       AudioIO      (const AudioData & other,QObject * parent = NULL) ;
                   AudioIO      (const AudioIO & io) ;
    virtual       ~AudioIO      (void) ;

    virtual int    BlobType     (void) const { return 1 ; }

    AudioIO &      operator =   (const AudioIO & io) ;
    virtual qint64 take         (int pos,int len) ;
    int            setIndex     (int index) ;
    int            currentIndex (void) ;
    TUID           currentMS    (void) ;
    void           setMethod    (ReadMethod method) ; // 0 - removal , 1 - indexing
    ReadMethod     Method       (void) ;
    bool           AutoStop     (void) ;
    bool           setAutoStop  (bool stopIt) ;

    virtual bool   Lock         (void) ;
    virtual bool   Unlock       (void) ;

    virtual QByteArray & add    (char * data,int len) ;

    virtual int  obtain           (void) ;
    virtual int  put              (void) ;
    virtual void finish           (ConduitDirection direction = NoDirection ,
                                   FinishCondition  condition = Correct   ) ;

    virtual void LockConduit      (void) ;
    virtual void UnlockConduit    (void) ;

    virtual qint64 TotalRead      (void) const ;
    virtual qint64 TotalWritten   (void) const ;

  protected:

    QMutex     mutex        ;
    QMutex     conduitMutex ;
    ReadMethod method       ;
    int        accessIndex  ;
    bool       BootTrigger  ;
    bool       autoStop     ;
    qint64     totalRead    ;
    qint64     totalWritten ;

    virtual qint64 readData     (char       * data,qint64 maxSize) ;
    virtual qint64 writeData    (const char * data,qint64 maxSize) ;

    virtual void   Error        (int error,QString message) ;
    virtual void   started      (void) ;
    virtual bool   proceed      (void) ;
    virtual void   finished     (void) ;
    virtual void   progress     (int current,int total) ;

  private:

  public slots:

  protected slots:

  private slots:

  signals:

    void Empty               (void) ;

};

class Q_AUDIOIO_EXPORT AvDecoder : public FFmpeg::Decoder
{ // This class is not thread safe
  public:

    explicit    AvDecoder   (void) ;
    virtual    ~AvDecoder   (void) ;

    virtual int DecodeAudio (AVPacket  & Packet ,
                             AudioData & data ) ;

  protected:

  private:

};

class Q_AUDIOIO_EXPORT AvEncoder : public FFmpeg::Encoder
{ // This class is not thread safe
  public:

    explicit    AvEncoder   (void) ;
    virtual    ~AvEncoder   (void) ;

    static void setEncoders (IMAPs & Encoders,int DeviceType) ;

  protected:

  private:

};

class Q_AUDIOIO_EXPORT AudioIn : public AudioIO
                               , public Thread
{
  Q_OBJECT
  Q_PROPERTY(QString name READ audioDevice WRITE setDevice)
  Q_PROPERTY(int deviceId READ inputDeviceId WRITE setDeviceId)
  Q_PROPERTY(int interval READ inputInterval WRITE setInterval)
  public:

    explicit AudioIn        (QObject * parent = NULL) ;
    explicit AudioIn        (const AudioData & other,QObject * parent = NULL) ;
    virtual ~AudioIn        (void) ;

    virtual int BlobType    (void) const { return 11 ; }

    QString  audioDevice    (void) ;
    void     setDevice      (QString device) ;
    int      inputDeviceId  (void) ;
    void     setDeviceId    (int id) ;
    int      inputInterval  (void) ;
    void     setInterval    (int interval) ;
    double   currentVolume  (int channel = -1) ;
    void     setVolume      (double volume,int channel = -1) ;

  protected:

    CiosAudio::Core   * core            ;
    CiosAudio::Stream * stream          ;
    bool                playing         ;
    long long           BytesPerSecond  ;
    /////////////////////////////////////
    QString             name            ;
    int                 deviceId        ;
    int                 interval        ;
    DMAPs               Volume          ;

    virtual void Error      (int error,QString message) ;
    virtual bool proceed    (void) ;
    virtual void progress   (int current,int total) ;

    virtual void run        (void) ;

  private:

  public slots:

    virtual void start      (void) ;
    virtual void stop       (void) ;

  protected slots:

  private slots:

  signals:

    void     Stopped        (void) ;

};

class Q_AUDIOIO_EXPORT AudioOut : public AudioIO
                                , public Thread
{
  Q_OBJECT
  Q_PROPERTY(QString name READ audioDevice WRITE setDevice)
  Q_PROPERTY(int deviceId READ outputDeviceId WRITE setDeviceId)
  Q_PROPERTY(int interval READ outputInterval WRITE setInterval)
  public:

    explicit AudioOut         (QObject * parent = NULL) ;
    explicit AudioOut         (const AudioData & other,QObject * parent = NULL) ;
    virtual ~AudioOut         (void) ;

    virtual int BlobType      (void) const { return 12 ; }

    QString  audioDevice      (void) ;
    void     setDevice        (QString device) ;
    int      outputDeviceId   (void) ;
    void     setDeviceId      (int id) ;
    int      outputInterval   (void) ;
    void     setInterval      (int interval) ;
    double   currentVolume    (int channel = -1) ;
    void     setVolume        (double volume,int channel = -1) ;

  protected:

    CiosAudio::Core   * core            ;
    CiosAudio::Stream * stream          ;
    bool                playing         ;
    long long           BytesPerSecond  ;
    /////////////////////////////////////
    QString             name            ;
    int                 deviceId        ;
    int                 interval        ;
    DMAPs               Volume          ;

    virtual void Error        (int error,QString message) ;
    virtual bool proceed      (void) ;
    virtual void progress     (int current,int total) ;

    virtual void run          (void) ;

  private:

  public slots:

    virtual void start        (const AudioData & data) ;
    virtual void start        (void) ;
    virtual void stop         (void) ;

  protected slots:

  private slots:

  signals:

    void     Stopped        (void) ;

};

class Q_AUDIOIO_EXPORT AudioTributary : public AudioIO
{
  Q_OBJECT
  public:

    QMap<int,BlobAcceptor *> Acceptors ;

    explicit AudioTributary (QObject * parent = NULL) ;
    virtual ~AudioTributary (void) ;

    virtual int          BlobType       (void) const { return 13 ; }

    virtual QByteArray & add            (char * data,int len) ;

    virtual bool         fresh          (void) ;
    virtual bool         demolish       (void) ;
    virtual bool         Lock           (void) ;
    virtual bool         Unlock         (void) ;

  protected:

    QMutex mutex ;

    virtual void         distribute     (void) ;
    virtual void         single         (void) ;
    virtual void         tributary      (void) ;

  private:

  public slots:

  protected slots:

  private slots:

  signals:

};

class Q_AUDIOIO_EXPORT AudioPlayer : public AudioIO
                                   , public Thread
{
  Q_OBJECT
  public:

    bool                        *   KeepRunning ;
    QList < AudioData           * > AudioQueue  ;
    QMap  < QString , AudioData * > AudioBank   ;
    NAMEs                           SoundMaps   ;

    explicit     AudioPlayer   (QObject * parent = NULL) ;
    explicit     AudioPlayer   (const AudioPlayer & audio) ;
    virtual     ~AudioPlayer   (void);

    virtual int  BlobType      (void) const { return 21 ; }

    void         operator   <= (AudioData & RawWave) ;

    QString   audioDevice      (void) ;
    void      setDevice        (QString device) ;
    int       outputDeviceId   (void) ;
    void      setDeviceId      (int id) ;
    int       outputInterval   (void) ;
    void      setInterval      (int interval) ;
    double    currentVolume    (int channel = -1) ;
    void      setVolume        (double volume,int channel = -1) ;

  protected:

    bool                Monitoring      ;
    bool                shoot           ;
    bool                completeStopped ;
    /////////////////////////////////////
    CiosAudio::Core   * core            ;
    CiosAudio::Stream * stream          ;
    bool                playing         ;
    long long           BytesPerSecond  ;
    /////////////////////////////////////
    QString             name            ;
    int                 deviceId        ;
    int                 interval        ;
    DMAPs               Volume          ;

    virtual bool proceed       (void) ;
    virtual void progress      (int current,int total) ;

    virtual void run           (void) ;

  private:

    bool         contains      (AudioData * data) ;
    void         WatchQueue    (void) ;

  public slots:

    virtual void Append        (QString name,AudioData & RawWave) ;
    virtual void Append        (AudioData & RawWave) ;
    virtual void Play          (QString name) ;
    virtual void Play          (SUID uuid) ;
    virtual void Stop          (void) ;
    virtual void Monitor       (void) ;
    virtual void Shutdown      (void) ;

    void         NextQueue     (void) ;

  protected slots:

  private slots:

  signals:

    void FindNext              (void);
    void AudioTime             (long long ms,long long total) ;
    void AudioError            (int error,QString message) ;

};

class Q_AUDIOIO_EXPORT AudioRecorder : public AudioIn
{
  Q_OBJECT
  public:

    explicit AudioRecorder   (QObject * parent = NULL);
    virtual ~AudioRecorder   (void);

    virtual int  BlobType       (void) const { return 22 ; }

    virtual void Start       (void) ;
    virtual void Stop        (void) ;
    virtual bool isRecording (void) ;

  protected:

    virtual void run         (void) ;
    virtual void started     (void) ;
    virtual void waiting     (void) ;
    virtual void finished    (void) ;

  private:

  public slots:

  protected slots:

  private slots:

  signals:

};

class Q_AUDIOIO_EXPORT AudioJack : public AudioIn
                                 , public AudioChannel
{
  Q_OBJECT
  public:

    explicit AudioJack         (QObject * parent = NULL) ;
    virtual ~AudioJack         (void) ;

    virtual int     BlobType   (void) const { return 31 ; }

    virtual int     type       (void) const ;
    virtual QString name       (void) ;
    virtual QString source     (void) ;
    virtual bool    hasData    (void) ;
    virtual TUID    length     (void) ;
    virtual TUID    bytes      (void) ;
    virtual int     rate       (void) ;
    virtual int     channels   (void) ;
    virtual bool    hasChannel (int id) ;
    virtual QString channel    (int id) ;
    virtual bool    canSeek    (void) ;
    virtual bool    assign     (AudioData & data) ;
    virtual bool    obtain     (AudioData & data,int bytes) ;
    virtual bool    play       (void) ;
    virtual bool    Pause      (void) ;
    virtual bool    seek       (TUID timestamp) ;
    virtual bool    Resume     (void) ;
    virtual bool    abort      (void) ;

  protected:

  private:

} ;

class Q_AUDIOIO_EXPORT AudioWriter : public QIODevice
                                   , public AudioData
                                   , public AvEncoder
                                   , public Thread
{
  Q_OBJECT
  Q_PROPERTY(ReadMethod method     READ Method   WRITE setMethod  )
  Q_PROPERTY(QString    WriterName READ name     WRITE setName    )
  Q_PROPERTY(int        interval   READ Interval WRITE setInterval)
  public:

    QIODevice * Output ;

    typedef enum   {
      Removal  = 0 ,
      Indexing = 1 }
      ReadMethod   ;

    explicit       AudioWriter   (QObject * parent = NULL) ;
    virtual       ~AudioWriter   (void) ;

    virtual int    BlobType      (void) const { return 41 ; }

    virtual QByteArray & add     (char * data,int len) ;

    virtual qint64 take          (int pos,int len) ;
    int            setIndex      (int index) ;
    int            currentIndex  (void) ;
    TUID           currentMS     (void) ;
    void           setMethod     (ReadMethod method) ; // 0 - removal , 1 - indexing
    ReadMethod     Method        (void) ;

    virtual bool   prepare       (void) ;
    virtual int    encode        (QIODevice & IO,bool flush) ;
    virtual bool   cease         (void) ;
    virtual bool   fresh         (void) ;
    virtual bool   demolish      (void) ;

    void           setName       (QString name) ;
    QString        name          (void) const ;
    void           setInterval   (int value) ;
    int            Interval      (void) const ;

    virtual void   Error         (int error,QString message) ;
    virtual void   started       (void) ;
    virtual bool   proceed       (void) ;
    virtual void   finished      (void) ;
    virtual void   progress      (TUID current,TUID total) ;

    virtual bool   Lock          (void) ;
    virtual bool   Unlock        (void) ;

  protected:

    QMutex     mutex       ;
    ReadMethod method      ;
    int        accessIndex ;
    bool       going       ;
    QString    WriterName  ;
    int        interval    ;
    TUID       packed      ;

    virtual qint64 readData      (char       * data,qint64 maxSize) ;
    virtual qint64 writeData     (const char * data,qint64 maxSize) ;

    virtual void   addLength     (int length) ;
    virtual void   addCompressed (int length) ;
    virtual void   run           (void) ;

  private:

  public slots:

  protected slots:

    void RelayStarted  (void) ;
    void RelayFinished (void) ;
    void RelayEmpty    (void) ;

  private slots:

  signals:

    void Started       (void) ;
    void Finished      (void) ;
    void Finished      (AudioWriter * writer) ;
    void Empty         (void) ;
    void AlterStarted  (void) ;
    void AlterFinished (void) ;
    void AlterEmpty    (void) ;

};

class Q_AUDIOIO_EXPORT AudioReader : public QIODevice
                                   , public AudioData
                                   , public AvDecoder
{
  Q_OBJECT
  Q_PROPERTY(ReadMethod method READ Method WRITE setMethod)
  public:

    typedef enum   {
      Removal  = 0 ,
      Indexing = 1 }
      ReadMethod   ;

    VarArgLists * PostSequences ;

    explicit       AudioReader  (QObject * parent = NULL) ;
    virtual       ~AudioReader  (void) ;

    virtual int    BlobType     (void) const { return 42 ; }

    virtual QByteArray & add    (char * data,int len) ;

    virtual qint64 take         (int pos,int len) ;
    int            setIndex     (int index) ;
    int            currentIndex (void) ;
    TUID           currentMS    (void) ;
    void           setMethod    (ReadMethod method) ; // 0 - removal , 1 - indexing
    ReadMethod     Method       (void) ;

    virtual bool   Peep         (void) ;
    virtual bool   prepare      (void) ;
    virtual bool   decode       (void) ;
    virtual bool   cease        (void) ;
    virtual bool   fresh        (void) ;
    virtual bool   demolish     (void) ;

    virtual bool   Lock         (void) ;
    virtual bool   Unlock       (void) ;

  protected:

    QMutex     mutex       ;
    ReadMethod method      ;
    int        accessIndex ;
    bool       going       ;
    AVPacket   Packet      ;

    virtual qint64 readData     (char       * data,qint64 maxSize) ;
    virtual qint64 writeData    (const char * data,qint64 maxSize) ;

    virtual void   Error        (int error,QString message) ;
    virtual void   started      (void) ;
    virtual bool   proceed      (void) ;
    virtual void   finished     (void) ;
    virtual void   progress     (int current,int total) ;

  private:

  public slots:

  protected slots:

    void RelayStarted  (void) ;
    void RelayFinished (void) ;
    void RelayEmpty    (void) ;

  private slots:

  signals:

    void Started       (void) ;
    void Finished      (void) ;
    void Finished      (AudioReader * reader) ;
    void Empty         (void) ;
    void AlterStarted  (void) ;
    void AlterFinished (void) ;
    void AlterEmpty    (void) ;

};

class Q_AUDIOIO_EXPORT AudioFeeder : public AudioReader
                                   , public AudioChannel
                                   , public Thread
{
  Q_OBJECT
  Q_PROPERTY(int  bufferTime     READ BufferTime WRITE setBufferTime)
  Q_PROPERTY(int  interval       READ Interval   WRITE setInterval  )
  Q_PROPERTY(bool waitCompletion READ Waiting    WRITE setCompletion)
  public:

    explicit AudioFeeder          (void) ;
    virtual ~AudioFeeder          (void) ;

    virtual int     BlobType      (void) const { return 43 ; }

    virtual int     type          (void) const ;
    virtual QString name          (void) ;
    virtual QString source        (void) ;
    virtual bool    hasData       (void) ;
    virtual TUID    length        (void) ;
    virtual TUID    bytes         (void) ;
    virtual int     rate          (void) ;
    virtual int     channels      (void) ;
    virtual bool    hasChannel    (int id) ;
    virtual QString channel       (int id) ;
    virtual bool    canSeek       (void) ;
    virtual bool    assign        (AudioData & data) ;
    virtual bool    obtain        (AudioData & data,int bytes) ;
    virtual bool    play          (void) ;
    virtual bool    Pause         (void) ;
    virtual bool    seek          (TUID timestamp) ;
    virtual bool    Resume        (void) ;
    virtual bool    abort         (void) ;

    virtual int     Interval      (void) ;
    virtual void    setInterval   (int interval) ;
    virtual int     BufferTime    (void) ;
    virtual void    setBufferTime (int buffer) ;
    virtual bool    Waiting       (void) ;
    virtual void    setCompletion (bool wait) ;

  protected:

    int  bufferTime     ;
    int  interval       ;
    bool waitCompletion ;

    virtual void    run           (void) ;
    virtual void    Completion    (void) ;

  private:

} ;

class Q_AUDIOIO_EXPORT NotifyPlayer : public QObject
                                    , public Thread
{
  Q_OBJECT
  public:

    bool                        *   KeepRunning ;
    QMap  < QString , AudioData * > AudioBank   ;
    NAMEs                           SoundMaps   ;

    explicit     NotifyPlayer  (QObject * parent = NULL) ;
    explicit     NotifyPlayer  (const NotifyPlayer & audio) ;
    virtual     ~NotifyPlayer  (void);

    QString   audioDevice      (void) ;
    void      setDevice        (QString device) ;
    int       outputDeviceId   (void) ;
    void      setDeviceId      (int id) ;
    int       outputInterval   (void) ;
    void      setInterval      (int interval) ;
    double    currentVolume    (int channel = -1) ;
    void      setVolume        (double volume,int channel = -1) ;

  protected:

    /////////////////////////////////////
    QString name     ;
    int     deviceId ;
    int     interval ;
    DMAPs   Volume   ;

    virtual void run           (int Type,ThreadData * data) ;
    virtual void Play          (ThreadData * data) ;

  private:

  public slots:

    virtual void Append        (QString name,AudioData & RawWave) ;
    virtual void Play          (QString name,QString device,int deviceId) ;
    virtual void Play          (QString name) ;
    virtual void Play          (SUID uuid,QString device,int deviceId) ;
    virtual void Play          (SUID uuid) ;
    virtual void Stop          (void) ;
    virtual void Shutdown      (void) ;

  protected slots:

  private slots:

  signals:

};

}

Q_DECLARE_METATYPE(N::AudioData)
Q_DECLARE_METATYPE(N::AvDecoder)
Q_DECLARE_METATYPE(N::AvEncoder)

Q_DECLARE_INTERFACE(N::AudioChannel , "com.neutrino.audio.channel" )

QT_END_NAMESPACE

#endif
