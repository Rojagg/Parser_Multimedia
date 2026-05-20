#include "tsCommon.h"
#include "tsTransportStream.h"
#include <iostream>

int main(int argc, char *argv[], char *envp[])
{
    FILE* file = std::fopen("example_new.ts", "rb");
    if (!file)
    {
        std::cerr << "File opening failed";
        return 0;
    }

    FILE* output = std::fopen("audio_out.mp2", "wb");
    if (!output)
    {
        std::cerr << "Output file opening failed";
        std::fclose(file);
        return 0;
    }

    xTS_PacketHeader    TS_PacketHeader;
    xTS_AdaptationField TS_AdaptationField;
    xPES_Assembler      PES_Assembler;
    PES_Assembler.Init(136);

    int32_t TS_PacketId = 0;
    uint8_t TS_PacketBuffer[188];

    while(std::fread(TS_PacketBuffer, 1, 188, file) == 188)
    {
        TS_PacketHeader.Reset();
        TS_AdaptationField.Reset();
        TS_PacketHeader.Parse(TS_PacketBuffer);
        TS_AdaptationField.Parse(TS_PacketBuffer, TS_PacketHeader.getAFC());

        xPES_Assembler::eResult Result = PES_Assembler.AbsorbPacket(TS_PacketBuffer, &TS_PacketHeader, &TS_AdaptationField);

        if(TS_PacketHeader.getPID() == 136)
        {
            printf("%010d ", TS_PacketId);
            TS_PacketHeader.Print();
            printf(" ");
            if(TS_PacketHeader.hasAdaptationField())
            {
                TS_AdaptationField.Print();
            }
            printf("\n");

            switch(Result)
            {
                case xPES_Assembler::eResult::StreamPackedLost:
                    printf("PcktLost\n"); break;
                case xPES_Assembler::eResult::AssemblingStarted:
                    printf("Started "); PES_Assembler.PrintPESH(); printf("\n"); break;
                case xPES_Assembler::eResult::AssemblingContinue:
                    printf("Continue\n"); break;
                case xPES_Assembler::eResult::AssemblingFinished:
                {
                    int32_t HL  = PES_Assembler.getHeaderLength();
                    int32_t Len = PES_Assembler.getNumPacketBytes();
                    std::fwrite(PES_Assembler.getPacket() + HL, 1, Len - HL, output);
                    printf("Finished PES: Len=%d\n", Len);
                    break;
                }
                default: break;
            }
        }

        TS_PacketId++;
    }

    std::fclose(file);
    std::fclose(output);
    return EXIT_SUCCESS;
}