#include <windows.h>
#include <winuser.h>
#include <wingdi.h>
#include <math.h>

int scalaFluxDarkroom[] = {255, 255, 254, 254, 253, 253, 252, 252, 251, 251, 250, 250, 249, 249, 248, 248, 247, 247, 246, 246, 245, 245, 244, 244, 243, 243, 242, 242, 241, 241, 240, 239, 239, 238, 238, 237, 237, 236, 236, 235, 235, 234, 233, 233, 232, 232, 231, 231, 230, 230, 229, 228, 228, 227, 227, 226, 226, 225, 225, 224, 223, 223, 222, 222, 221, 220, 220, 219, 219, 218, 218, 217, 216, 216, 215, 215, 214, 213, 213, 212, 212, 211, 210, 210, 209, 209, 208, 207, 207, 206, 205, 205, 204, 204, 203, 202, 202, 201, 200, 200, 199, 198, 198, 197, 196, 196, 195, 195, 194, 193, 193, 192, 191, 191, 190, 189, 189, 188, 187, 186, 186, 185, 184, 184, 183, 182, 182, 181, 180, 179, 179, 178, 177, 177, 176, 175, 174, 174, 173, 172, 171, 171, 170, 169, 168, 168, 167, 166, 165, 165, 164, 163, 162, 161, 161, 160, 159, 158, 157, 157, 156, 155, 154, 153, 152, 152, 151, 150, 149, 148, 147, 146, 146, 145, 144, 143, 142, 141, 140, 139, 138, 137, 136, 136, 135, 134, 133, 132, 131, 130, 129, 128, 127, 126, 125, 124, 123, 122, 121, 119, 118, 117, 116, 115, 114, 113, 112, 111, 109, 108, 107, 106, 105, 103, 102, 101, 100, 98, 97, 96, 94, 93, 92, 90, 89, 87, 86, 84, 83, 81, 80, 78, 76, 75, 73, 71, 69, 68, 66, 64, 62, 59, 57, 55, 53, 50, 48, 45, 42, 39, 35, 32, 27, 22, 16, 0};

void error(char* msg)
{
    printf("%s", msg);
    exit(-1);
}

void initDisplayDevice(DISPLAY_DEVICE *DisplayDevice)
{
    ZeroMemory(DisplayDevice, sizeof(DISPLAY_DEVICE));
    DisplayDevice->cb = sizeof(DISPLAY_DEVICE);
}

void FaiDisplay(char* deviceName, int mode)
{
    HDC deviceContext;
    WORD ramp[3][256];
    int i = 0, j = 0;
    
    printf("Doing %s, mode %i\n", deviceName, mode);
    
    if (!(deviceContext = CreateDC("DISPLAY", deviceName, NULL, NULL)))
        error("problema CreateDC()");
    
    if (!GetDeviceGammaRamp(deviceContext, ramp))
        error("problema GetDeviceGammaRamp()");
    
    switch (mode)
    {
        case 0: //DARKROOM
            for (i = 0; i < 256; i++)
            {
                ramp[0][i] = scalaFluxDarkroom[i] << 8;
                ramp[1][i] = 0 << 8;
                ramp[2][i] = 0 << 8;
            }
            break;
        case 1: //SOFT
            for (i = 0; i < 256; i++)
            {
                ramp[0][i] = i << 8;
                ramp[1][i] = (int)(i * (150.0/256.0)) << 8; //TODO: get these numbers from a configuration file
                ramp[2][i] = (int)(i * (90.0/256.0)) << 8;
            }
            break;
        case 2: //FULL COLOURS
            for (i = 0; i < 256; i++)
            {
                ramp[0][i] = i << 8;
                ramp[1][i] = i << 8;
                ramp[2][i] = i << 8;
            }
            break;
    }
    
    if (!SetDeviceGammaRamp(deviceContext,ramp))
        error("problema SetDeviceGammaRamp()");
    
    if (!DeleteDC(deviceContext))
        error("problema DeleteDC()");
}

// void evento()
// {
    // MessageBoxEx(NULL, "merdaccia", "stronzo",MB_OK, 0);
// }

void FaiTuttiDisplay(int mode)
{
    int i = 0;
    DISPLAY_DEVICE DisplayDevice;
    
    initDisplayDevice(&DisplayDevice);

    i = 0;
    while (EnumDisplayDevices(NULL, i, &DisplayDevice, 1))
    {
        if (DisplayDevice.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP)
            FaiDisplay(DisplayDevice.DeviceName, mode);
        i++;
    }
}

int main(int argc, char** argv)
{
    MSG msg;
    int mode = 0;
    
    if (!RegisterHotKey(NULL,
                       0,
                       MOD_ALT | MOD_CONTROL,
                       0x44 // D key
                       ))
        error("problema RegisterHotKey()");
    
    
    FaiTuttiDisplay(mode);
    
    while (GetMessage(&msg, NULL, 0, 0) != 0)
    {
        if (msg.message == WM_HOTKEY)
        {
            if (mode == 2)
                mode = 0;
            else
                mode++;
            
            FaiTuttiDisplay(mode);
        }
    }
    
    return 0;
    
}