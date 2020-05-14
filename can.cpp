#include <stdio.h>
#include <stdlib.h>
#include "can_api.h"
#include <string.h>

using namespace std;

int main() {
    TCAN_HANDLE Handle;
    TCAN_STATUS Status;
    CHAR *ComPort = "/dev/ttyUSB0";
    CHAR *szBitrate = "1000";
    CHAR *acceptance_code = "00000000";
    CHAR *acceptance_mask = "00000000";
    VOID *flags = CAN_TIMESTAMP_OFF;
    DWORD Mode = LoopBack;

    char version[10];
    CAN_MSG SendMSG;
    CAN_MSG RecvMSG;
    Handle = -1;
    Status = 0;

    // Turn servo on
    SendMSG.Flags = CAN_FLAGS_EXTENDED;
    SendMSG.Id = 0x040;
    SendMSG.Size = 1;
    SendMSG.Data[0] = 0;

    Handle = CAN_Open( ComPort, szBitrate, acceptance_code, acceptance_mask, flags, Mode );

    printf ( "handle= %d\n", Handle );
    if ( Handle < 0 ) {
        return 0;
    }

    memset ( version, 0, sizeof ( char ) * 10 );
    Status = CAN_Flush ( Handle );
    Status = CAN_Version ( Handle, version );

    if ( Status == CAN_ERR_OK ) {
        printf ( "Version : %s\n", version );
    }

    Status = CAN_Write ( Handle, &SendMSG );
    if ( Status == CAN_ERR_OK ) {
        printf ( "Write Success\n" );
    }

    while(1)  {
        Status = CAN_Read ( Handle, &RecvMSG );

        if ( Status == CAN_ERR_OK ) 
            break;
    }

    float torque2pwm = 10.0f;
    short pwm[4] = {
        0.1*torque2pwm,
        0.1*torque2pwm,
        0.1*torque2pwm,
        0.1*torque2pwm
    }; 

    // Move
    SendMSG.Flags = 1;//CAN_FLAGS_EXTENDED;
    SendMSG.Id = 0x060;
    SendMSG.Size = 8;
    SendMSG.Data[0] = (unsigned char)( (pwm[0] >> 8) & 0x00ff);
    SendMSG.Data[1] = (unsigned char)(pwm[0] & 0x00ff);
    SendMSG.Data[2] = (unsigned char)( (pwm[1] >> 8) & 0x00ff);
    SendMSG.Data[3] = (unsigned char)(pwm[1] & 0x00ff);
    SendMSG.Data[4] = (unsigned char)( (pwm[2] >> 8) & 0x00ff);
    SendMSG.Data[5] = (unsigned char)(pwm[2] & 0x00ff);
    SendMSG.Data[6] = (unsigned char)( (pwm[3] >> 8) & 0x00ff);
    SendMSG.Data[7] = (unsigned char)(pwm[3] & 0x00ff);
    

    Status = CAN_Write ( Handle, &SendMSG );
    if ( Status == CAN_ERR_OK ) {
        printf ( "Write Success\n" );
    }

    while(1)  {
        Status = CAN_Read ( Handle, &RecvMSG );

        if ( Status == CAN_ERR_OK ) {
            printf("%d\n", Status);
            printf ( "Read ID=0x%X, Type=%s, DLC=%d, FrameType=%s, Data=", RecvMSG.Id, ( RecvMSG.Flags & CAN_FLAGS_STANDARD ) ? "STD" : "EXT", RecvMSG.Size, ( RecvMSG.Flags & CAN_FLAGS_REMOTE ) ? "REMOTE" : "DATA" );
            // printf("Read ID=0x%X", RecvMSG.Id);
            for ( int i = 0; i < RecvMSG.Size; i++ ) {
                printf ( "%X,", RecvMSG.Data[i] );
            }
            printf("\n");
            break;
        }
    }

    Status = CAN_Close ( Handle );

}