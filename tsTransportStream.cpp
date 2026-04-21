#include "tsTransportStream.h"

//=============================================================================================================================================================================
// xTS_PacketHeader
//=============================================================================================================================================================================


/// @brief Reset - reset all TS packet header fields
void xTS_PacketHeader::Reset()
{
  //write me
  m_SB    = 0;
  m_TEI   = 0;
  m_PUSI  = 0;
  m_TP    = 0;
  m_PID   = 0;
  m_TCC   = 0;
  m_AFC   = 0;
  m_CC    = 0; 

}

/**
  @brief Parse all TS packet header fields
  @param Input is pointer to buffer containing TS packet
  @return Number of parsed bytes (4 on success, -1 on failure) 
 */
int32_t xTS_PacketHeader::Parse(const uint8_t* Input)
{
  const uint32_t* HeaderPtr = (const uint32_t*) Input;
  uint32_t Header = xSwapBytes32(*HeaderPtr);

  m_SB    = uint8_t((Header & 0b11111111000000000000000000000000) >> 24);
  m_TEI   = uint8_t((Header & 0b00000000100000000000000000000000) >> 23);
  m_PUSI  = uint8_t((Header & 0b00000000010000000000000000000000) >> 22);
  m_TP    = uint8_t((Header & 0b00000000001000000000000000000000) >> 21);
  m_PID   = uint16_t((Header & 0b00000000000111111111111100000000) >> 8);
  m_TCC   = uint8_t((Header & 0b00000000000000000000000011000000) >> 6);
  m_AFC   = uint8_t((Header & 0b00000000000000000000000000110000) >> 4);
  m_CC    = uint8_t((Header & 0b00000000000000000000000000001111)); 
  
  return xTS::TS_HeaderLength;
}

/// @brief Print all TS packet header fields
void xTS_PacketHeader::Print() const
{
  printf("TS: ");
  printf("SB=%d",getSyncByte());
  printf(" TEI=%d",getTEI());
  printf(" PUSI=%d",getPUSI());
  printf(" TP=%d",getTP());
  printf(" PID=%d",getPID());
  printf(" TCC=%d",getTCC());
  printf(" AFC=%d",getAFC());
  printf(" CC=%d",getCC());
  //print sth
}

//=============================================================================================================================================================================
