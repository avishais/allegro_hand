#include <stdio.h>
#include <stdlib.h>
#include "can_api.h"
#include <string.h>


using namespace std;

int main() {
    TCAN_HANDLE Handle;
    TCAN_STATUS Status;
    CHAR *ComPort = "/dev/ttyUSB1";
    CHAR *szBitrate = "1000";
    CHAR *acceptance_code = "1FFFFFFF";
    CHAR *acceptance_mask = "00000000";
    VOID *flags = CAN_TIMESTAMP_OFF;
    DWORD Mode = LoopBack;

    char version[10];
    CAN_MSG SendMSG;
    CAN_MSG RecvMSG;
    Handle = -1;
    Status = 0;

    Handle = CAN_Open ( ComPort, szBitrate, acceptance_code, acceptance_mask, flags, Mode );

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
}