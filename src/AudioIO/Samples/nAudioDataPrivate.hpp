#ifndef NAUDIODATAPRIVATE_HPP
#define NAUDIODATAPRIVATE_HPP

typedef struct       {
  int32_t SampleRate ;
  int32_t Bits       ;
  int32_t Channels   ;
  int32_t DataSize   ;
  int32_t Format     ; // new
  int32_t Original   ;
  int32_t Alignment  ; // 0 - Interleaved , 1 - Planar
  int32_t unused03   ;
  int32_t unused04   ;
  int32_t unused05   ;
  int32_t unused06   ;
  int32_t unused07   ;
  int32_t unused08   ;
  int32_t unused09   ;
  int32_t unused10   ;
  int32_t unused11   ;
} RawFormat          ;

#endif // NAUDIODATAPRIVATE_HPP
