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

/// @brief Reset - reset all TS packet header fields
void xTS_AdaptationField::Reset()
{
    m_AFL = 0;
    m_DC = 0;
    m_RA = 0;
    m_SP = 0;
    m_PR = 0;
    m_OR = 0;
    m_SF = 0;
    m_EX = 0;
}
/**
@brief Parse adaptation field
@param PacketBuffer is pointer to buffer containing TS packet
@param AdaptationFieldControl is value of Adaptation Field Control field of
corresponding TS packet header
@return Number of parsed bytes (length of AF or -1 on failure)
*/
int32_t xTS_AdaptationField::Parse(const uint8_t* PacketBuffer, uint8_t AdaptationFieldControl)
{
  if (AdaptationFieldControl != 2 && AdaptationFieldControl !=3) return -1;

  const uint8_t* AF = PacketBuffer + xTS::TS_HeaderLength;

  m_AFL = AF[0];

  if(m_AFL == 0) return 1;
  
  uint8_t flags = AF[1];

  m_DC = (flags >> 7) & 0x01;
  m_RA = (flags >> 6) & 0x01;
  m_SP = (flags >> 5) & 0x01;
  m_PR = (flags >> 4) & 0x01;
  m_OR = (flags >> 3) & 0x01;
  m_SF = (flags >> 2) & 0x01;
  m_TP = (flags >> 1) & 0x01;
  m_EX = (flags >> 0) & 0x01;

  return 1 + m_AFL;
}
/// @brief Print all TS packet header fields
void xTS_AdaptationField::Print() const
{
   printf("AF: ");
   printf("L:%d ",m_AFL);
  printf("DC=%d",m_DC);
  printf(" RA=%d",m_RA);
  printf(" SP=%d",m_SP);
  printf(" PR=%d",m_PR);
  printf(" OR=%d",m_OR);
  printf(" SF=%d",m_SF);
  printf(" TP=%d",m_TP);
  printf(" EX=%d",m_EX);
}