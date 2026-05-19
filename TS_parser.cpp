#include "tsCommon.h"
#include "tsTransportStream.h"
#include <iostream>
//=============================================================================================================================================================================
//int main(int argc, char *argv[ ], char *envp[ ])
int main(int argc, char *argv[ ], char *envp[ ])
{
  // TODO - open file

FILE* file = std::fopen("example_new.ts", "rb"); //read in binary mode -rb
  // TODO - check if file if opened
  if (!file){
    std::cerr << "File opening failed";
    return 0;
  }

  xTS_PacketHeader    TS_PacketHeader;
  xTS_AdaptationField TS_AdaptationField;
  xPES_Assembler PES_Assembler;
  PES_Assembler.Init(136);
  int32_t TS_PacketId = 0;
  uint8_t TS_PacketBuffer[188]; // buffer 188B
  while(std::fread(TS_PacketBuffer, 1, 188, file) ==188)
  {
    // TODO - read from file
    TS_PacketHeader.Reset();
    TS_AdaptationField.Reset();
    TS_PacketHeader.Parse(TS_PacketBuffer);

    printf("%010d ", TS_PacketId);
    TS_PacketHeader.Print();
    printf(" ");
    if(TS_AdaptationField.Parse(TS_PacketBuffer, TS_PacketHeader.getAFC()) > 0){
    TS_AdaptationField.Print();
    }
    printf("\n");

    xPES_Assembler::eResult Result = PES_Assembler.AbsorbPacket(TS_PacketBuffer, &TS_PacketHeader, &TS_AdaptationField);

    switch(Result){
      case xPES_Assembler::eResult::StreamPackedLost : printf("PcktLost "); break;
    case xPES_Assembler::eResult::AssemblingStarted : printf("Started "); PES_Assembler.PrintPESH(); break;
    case xPES_Assembler::eResult::AssemblingContinue: printf("Continue "); break;
    case xPES_Assembler::eResult::AssemblingFinished: {
      int HL = PES_Assembler.getHeaderLength();
      int Len = PES_Assembler.getNumPacketBytes();
      
      //fwrite(PES_Assembler.getPacket() + HL, 1, Len - HL, stdout);
      printf("Finished "); printf("PES: Len=%d", PES_Assembler.getNumPacketBytes()); break;
    }
    default: break;


    }
    TS_PacketId++;
  }
  
  // TODO - close file
  std::fclose(file);

  return EXIT_SUCCESS;
}

//=============================================================================================================================================================================
