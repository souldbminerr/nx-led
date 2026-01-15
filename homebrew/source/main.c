// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Include the main libnx system header, for Switch development
#include <switch.h>
I2cSession session;

Result I2cSet_U8(I2cDevice dev, u8 reg, u8 val) {
    struct {
        u8 reg;
        u8 val;
    } __attribute__((packed)) cmd;

    I2cSession _session;
    Result res = i2cOpenSession(&_session, dev);
    if (R_FAILED(res))
        return res;

    cmd.reg = reg;
    cmd.val = val;
    res = i2csessionSendAuto(&_session, &cmd, sizeof(cmd), I2cTransactionOption_All);
    i2csessionClose(&_session);
    return res;
}

bool ledState;

// Main program entrypoint
int main(int argc, char* argv[])
{
    // This example uses a text console, as a simple way to output text to the screen.
    // If you want to write a software-rendered graphics application,
    //   take a look at the graphics/simplegfx example, which uses the libnx Framebuffer API instead.
    // If on the other hand you want to write an OpenGL based application,
    //   take a look at the graphics/opengl set of examples, which uses EGL instead.
    consoleInit(NULL);

    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);

    // Other initialization goes here. As a demonstration, we print hello world.
    printf("nx-led app\n");
    i2cInitialize();
    
    i2cOpenSession(&session, 0xA5);
    
    // Main loop
    while (appletMainLoop())
    {
        // Scan the gamepad. This should be done once for each frame
        padUpdate(&pad);

        // padGetButtonsDown returns the set of buttons that have been
        // newly pressed in this frame compared to the previous one
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus)
            break; // break in order to return to hbmenu

        if (kDown & HidNpadButton_A) {
            I2cSet_U8(0xA5, ledState ? 0x01 : 0x02, 0xFF); // Turn LED on/off
            ledState = !ledState;
        }
            
        // Your code goes here

        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }

    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}
