#include <stdio.h>
#include <file.h>
#include <string.h>

#include <pins.h>
#include <sci_io_driverlib.h>
#include <ti_ascii.h>
#include <cc.h>

FILE *fid;
int status;

void gpio_conf(void)
{
    // GPIO43 is the SCI Rx pin.
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCIRXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_DIR_MODE_IN);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCIRXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCIRXDA, GPIO_QUAL_ASYNC);
    // GPIO42 is the SCI Tx pin.
    GPIO_setPinConfig(DEVICE_GPIO_CFG_SCITXDA);
    GPIO_setDirectionMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_DIR_MODE_OUT);
    GPIO_setPadConfig(DEVICE_GPIO_PIN_SCITXDA, GPIO_PIN_TYPE_STD);
    GPIO_setQualificationMode(DEVICE_GPIO_PIN_SCITXDA, GPIO_QUAL_ASYNC);

    return;
}

// scia_init - SCIA  8-bit word, baud rate 0x001A, default, 1 STOP bit, no parity
void scia_init(void)
{
    // Note: Clocks were turned on to the SCIA peripheral
    // in the InitSysCtrl() function
    SCI_performSoftwareReset(SCIA_BASE);
    SCI_setConfig(SCIA_BASE, DEVICE_LSPCLK_FREQ, 115200, (SCI_CONFIG_WLEN_8 |
                                                        SCI_CONFIG_STOP_ONE |
                                                        SCI_CONFIG_PAR_NONE));

    SCI_resetChannels(SCIA_BASE);
//    SCI_resetRxFIFO(SCIA_BASE);
//    SCI_resetTxFIFO(SCIA_BASE);
//    SCI_clearInterruptStatus(SCIA_BASE, SCI_INT_TXFF | SCI_INT_RXFF);
//    SCI_enableFIFO(SCIA_BASE);
    SCI_enableInterrupt(SCIA_BASE, SCI_INT_RXRDY_BRKDT | SCI_INT_TXRDY);
    SCI_enableModule(SCIA_BASE);
    SCI_performSoftwareReset(SCIA_BASE);

    // Redirect STDOUT to SCI
    status = add_device("scia", _SSA, SCI_open, SCI_close, SCI_read, SCI_write,
                        SCI_lseek, SCI_unlink, SCI_rename);
    fid = fopen("scia","w");
    freopen("scia:", "w", stdout);
    setvbuf(stdout, NULL, _IONBF, 0);
    print_ascii_banner();

    printf("%s %d\n",__FUNCTION__, 1234);
    DPRINT("Hello world 0x%04X !!\n", 0xBEEF);
    float pi = 3.14159265;
    DPRINT("%f %f\n", pi, pi/2);


    return;
}

