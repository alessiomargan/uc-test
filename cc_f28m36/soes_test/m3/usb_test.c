#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "inc/hw_sysctl.h"
#include "board_drivers/set_pinout_f28m36x.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/flash.h"
#include "usblib/usblib.h"
#include "usblib/usbhid.h"
#include "usblib/host/usbhost.h"
#include "usblib/host/usbhhid.h"
#include "usblib/host/usbhhidkeyboard.h"
#include "utils/uartstdio.h"
#include <string.h>

#include <peripherals.h>

//*****************************************************************************
//
// The ASCII code for a backspace character.
//
//*****************************************************************************
#define ASCII_BACKSPACE 0x08

//*****************************************************************************
// The size of the host controller's memory pool in bytes.
//*****************************************************************************
#define HCD_MEMORY_SIZE         128

//*****************************************************************************
// The memory pool to provide to the Host controller driver.
//*****************************************************************************
unsigned char g_pHCDPool[HCD_MEMORY_SIZE];

//*****************************************************************************
// The size of the keyboard device interface's memory pool in bytes.
//*****************************************************************************
#define KEYBOARD_MEMORY_SIZE    128

//*****************************************************************************
// The memory pool to provide to the keyboard device.
//*****************************************************************************
unsigned char g_pucBuffer[KEYBOARD_MEMORY_SIZE];

//*****************************************************************************
// Declare the USB Events driver interface.
//*****************************************************************************
DECLARE_EVENT_DRIVER(g_sUSBEventDriver, 0, 0, USBHCDEvents);

//*****************************************************************************
// The global that holds all of the host drivers in use in the application.
// In this case, only the Keyboard class is loaded.
//*****************************************************************************
static tUSBHostClassDriver const * const g_ppHostClassDrivers[] =
{
    &g_USBHIDClassDriver,
    &g_sUSBEventDriver
};

//*****************************************************************************
// This global holds the number of class drivers in the g_ppHostClassDrivers
// list.
//*****************************************************************************
static const unsigned long g_ulNumHostClassDrivers =
    sizeof(g_ppHostClassDrivers) / sizeof(tUSBHostClassDriver *);


//*****************************************************************************
//
// The global value used to store the keyboard instance value.
//*****************************************************************************
static unsigned long g_ulKeyboardInstance;

//*****************************************************************************
// This enumerated type is used to hold the states of the keyboard.
//*****************************************************************************
enum
{
    // No device is present.
    STATE_NO_DEVICE,

    // Keyboard has been detected and needs to be initialized in the main
    // loop.
    STATE_KEYBOARD_INIT,

    // Keyboard is connected and waiting for events.
    STATE_KEYBOARD_CONNECTED,

    // Keyboard has received a key press that requires updating the keyboard
    // in the main loop.
    STATE_KEYBOARD_UPDATE,

    // An unsupported device has been attached.
    STATE_UNKNOWN_DEVICE,

    // A power fault has occurred.
    STATE_POWER_FAULT
}
g_eUSBState;

extern const tHIDKeyboardUsageTable g_sUSKeyboardMap;

//*****************************************************************************
//
// The current USB operating mode - Host, Device or unknown.
//
//*****************************************************************************
tUSBMode g_eCurrentUSBMode;

//*****************************************************************************
//
// This variable holds the current status of the modifiers keys.
//*****************************************************************************
unsigned long g_ulModifiers = 0;

//*****************************************************************************
// The current USB operating mode - Host, Device or unknown.
//*****************************************************************************
tUSBMode g_eCurrentUSBMode;
tUSBMode eLastMode;



//*****************************************************************************
// This is the generic callback from host stack.
// \param pvData is actually a pointer to a tEventInfo structure.
// This function will be called to inform the application when a USB event has
// occurred that is outside those related to the keyboard device.  At this
// point this is used to detect unsupported devices being inserted and removed.
// It is also used to inform the application when a power fault has occurred.
// This function is required when the g_USBGenericEventDriver is included in
// the host controller driver array that is passed in to the
// USBHCDRegisterDrivers() function.
// \return None.
//*****************************************************************************
void USBHCDEvents(void *pvData)
{
    tEventInfo *pEventInfo;

    // Cast this pointer to its actual type.
    pEventInfo = (tEventInfo *)pvData;

    switch(pEventInfo->ulEvent)
    {
		// New keyboard detected.
		case USB_EVENT_CONNECTED:
			// See if this is a HID Keyboard.
			if((USBHCDDevClass(pEventInfo->ulInstance, 0) == USB_CLASS_HID) &&
			   (USBHCDDevProtocol(pEventInfo->ulInstance, 0) == USB_HID_PROTOCOL_KEYB)) {
				// Indicate that the keyboard has been detected.
				UARTprintf("Keyboard Connected\n");
				// Proceed to the STATE_KEYBOARD_INIT state so that the main
				// loop can finish initialized the mouse since
				// USBHKeyboardInit() cannot be called from within a callback.
				g_eUSBState = STATE_KEYBOARD_INIT;
			}
			break;
		// Unsupported device detected.
		case USB_EVENT_UNKNOWN_CONNECTED:
			UARTprintf("Unsupported Device Class (0x%02x) Connected.\n",
						pEventInfo->ulInstance);
			// An unknown device was detected.
			g_eUSBState = STATE_UNKNOWN_DEVICE;
			break;
		// Device has been unplugged.
		case USB_EVENT_DISCONNECTED:
			// Indicate that the device has been disconnected.
			UARTprintf("Device Disconnected\n");
			// Change the state so that the main loop knows that the device is no longer present.
			g_eUSBState = STATE_NO_DEVICE;
			break;
		// Power Fault occurred.
		case USB_EVENT_POWER_FAULT:
			UARTprintf("Power Fault\n");
			// No power means no device is present.
			g_eUSBState = STATE_POWER_FAULT;
			break;

		default:
			UARTprintf("Unhandled USB event\n");
        	break;
    }
}

//*****************************************************************************
//
// USB Mode callback
//
// \param ulIndex is the zero-based index of the USB controller making the
//        callback.
// \param eMode indicates the new operating mode.
//
// This function is called by the USB library whenever an OTG mode change
// occurs and, if a connection has been made, informs us of whether we are to
// operate as a host or device.
//
// \return None.
//
//*****************************************************************************
void ModeCallback(unsigned long ulIndex, tUSBMode eMode)
{
    // Save the new mode.
    g_eCurrentUSBMode = eMode;

    switch(eMode)
    {
        case USB_MODE_HOST:
            UARTprintf("\nHost Mode.\n");
            break;
        case USB_MODE_DEVICE:
            UARTprintf("\nDevice Mode.\n");
            break;
        case USB_MODE_NONE:
            UARTprintf("\nIdle Mode.\n");
            break;
        default:
            UARTprintf("ERROR: Bad Mode!\n");
            break;
    }
}

//*****************************************************************************
// This is the callback from the USB HID keyboard handler.
// \param pvCBData is ignored by this function.
// \param ulEvent is one of the valid events for a keyboard device.
// \param ulMsgParam is defined by the event that occurs.
// \param pvMsgData is a pointer to data that is defined by the event that
// occurs.
// This function will be called to inform the application when a keyboard has
// been plugged in or removed and any time a key is pressed or released.
// \return This function will return 0.
//*****************************************************************************
unsigned long KeyboardCallback(void *pvCBData, unsigned long ulEvent,
                 unsigned long ulMsgParam, void *pvMsgData)
{
    unsigned char ucChar;

    switch(ulEvent)
    {
    // New Key press detected.
    case USBH_EVENT_HID_KB_PRESS:
    	// If this was a Caps Lock key then update the Caps Lock state.
        if(ulMsgParam == HID_KEYB_USAGE_CAPSLOCK) {
            // The main loop needs to update the keyboard's Caps Lock state.
            g_eUSBState = STATE_KEYBOARD_UPDATE;
            // Toggle the current Caps Lock state.
            g_ulModifiers ^= HID_KEYB_CAPS_LOCK;
        } else if(ulMsgParam == HID_KEYB_USAGE_SCROLLOCK) {
			// The main loop needs to update the keyboard's Scroll Lock state.
            g_eUSBState = STATE_KEYBOARD_UPDATE;
			// Toggle the current Scroll Lock state.
			g_ulModifiers ^= HID_KEYB_SCROLL_LOCK;
		} else if(ulMsgParam == HID_KEYB_USAGE_NUMLOCK) {
			// The main loop needs to update the keyboard's Scroll Lock state.
            g_eUSBState = STATE_KEYBOARD_UPDATE;
            // Toggle the current Num Lock state.
            g_ulModifiers ^= HID_KEYB_NUM_LOCK;
		} else {
			// Was this the backspace key?
			if((unsigned char)ulMsgParam == HID_KEYB_USAGE_BACKSPACE) {
				// Yes - set the ASCII code for a backspace key.  This is
				// not returned by USBHKeyboardUsageToChar since this only
				// returns printable characters.
				ucChar = ASCII_BACKSPACE;
			} else {
				// This is not backspace so try to map the usage code to a
				// printable ASCII character.
				ucChar = (unsigned char)
						 USBHKeyboardUsageToChar(g_ulKeyboardInstance,
												 &g_sUSKeyboardMap,
												 (unsigned char)ulMsgParam);
			}
			// A zero value indicates there was no textual mapping of this usage code.
			if(ucChar != 0) {
				UARTprintf("%c", ucChar);
			}
		}
        break;

    case USBH_EVENT_HID_KB_MOD:
        // This application ignores the state of the shift or control
        // and other special keys.
        break;

    case USBH_EVENT_HID_KB_REL:
        // This applications ignores the release of keys as well.
        break;
    }

    return(0);
}



void Configure_USB_host(void)
{

	// USB Pins
#ifdef _CONTROL_CARD
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPION);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOG);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOJ);

	GPIOPinConfigure(GPIO_PN6_USB0EPEN);
	GPIOPinConfigure(GPIO_PN7_USB0PFLT);
	GPIOPinTypeUSBDigital(GPIO_PORTN_BASE, GPIO_PIN_6 | GPIO_PIN_7);
	GPIOPinTypeUSBAnalog(GPIO_PORTF_BASE, GPIO_PIN_6);
	GPIOPinTypeUSBAnalog(GPIO_PORTG_BASE, GPIO_PIN_2 | GPIO_PIN_5 | GPIO_PIN_6);

	// Enable Clocking to the USB controller.
	SysCtlPeripheralEnable(SYSCTL_PERIPH_USB0);

	// Register interrupt handlers in the RAM vector table
	IntRegister(INT_USB0, USB0OTGModeIntHandler);

	// Configure the power pins for host controller.
	GPIOPinConfigure(GPIO_PC5_USB0EPEN);
	GPIOPinConfigure(GPIO_PJ1_USB0PFLT);
	GPIOPinTypeUSBDigital(GPIO_PORTC_BASE, GPIO_PIN_5);
	GPIOPinTypeUSBDigital(GPIO_PORTJ_BASE, GPIO_PIN_1);

    // Initially wait for device connection.
    g_eUSBState = STATE_NO_DEVICE;
    eLastMode = USB_MODE_OTG;
    g_eCurrentUSBMode = USB_MODE_OTG;

    // Initialize the USB stack mode and pass in a mode callback.
    USBStackModeSet(0, USB_MODE_OTG, ModeCallback);

    // Register the host class drivers.
    USBHCDRegisterDrivers(0, g_ppHostClassDrivers, g_ulNumHostClassDrivers);

    // Open an instance of the keyboard driver.  The keyboard does not need
    // to be present at this time, this just save a place for it and allows
    // the applications to be notified when a keyboard is present.
    g_ulKeyboardInstance = USBHKeyboardOpen(KeyboardCallback, g_pucBuffer,
                                            KEYBOARD_MEMORY_SIZE);

    // Initialize the power configuration. This sets the power enable signal
    // to be active high and does not enable the power fault.
    USBHCDPowerConfigInit(0, USBHCD_VBUS_AUTO_HIGH | USBHCD_VBUS_FILTER);

    // Initialize the USB controller for OTG operation with a 2ms polling rate.
    USBOTGModeInit(0, 2000, g_pHCDPool, HCD_MEMORY_SIZE);

    UARTprintf("%s\n",__FUNCTION__);
#else

#endif

}


void usb_handler(void)
{
	const char * pcString;

    // Tell the OTG library code how much time has passed in
    // milliseconds since the last call.
    USBOTGMain(GetTickms());

    // Has the USB mode changed since last time we checked?
    if(g_eCurrentUSBMode != eLastMode) {
        // Remember the new mode.
        eLastMode = g_eCurrentUSBMode;

        switch(eLastMode)
        {
            case USB_MODE_HOST:
                pcString = "HOST";
                break;
            case USB_MODE_DEVICE:
                pcString = "DEVICE";
                break;
            case USB_MODE_NONE:
                pcString = "NONE";
                break;
            default:
                pcString = "UNKNOWN";
                break;
        }

        UARTprintf("USB mode changed to %s\n", pcString);
    }

    switch(g_eUSBState)
    {
    	// This state is entered when they keyboard is first detected.
    	case STATE_KEYBOARD_INIT:
			// Initialized the newly connected keyboard.
			USBHKeyboardInit(g_ulKeyboardInstance);
			// Proceed to the keyboard connected state.
			g_eUSBState = STATE_KEYBOARD_CONNECTED;
			USBHKeyboardModifierSet(g_ulKeyboardInstance, g_ulModifiers);
			break;

    	case STATE_KEYBOARD_UPDATE:
			// If the application detected a change that required an
			// update to be sent to the keyboard to change the modifier
			// state then call it and return to the connected state.
			g_eUSBState = STATE_KEYBOARD_CONNECTED;
			USBHKeyboardModifierSet(g_ulKeyboardInstance, g_ulModifiers);
			break;

		case STATE_KEYBOARD_CONNECTED:
			// Nothing is currently done in the main loop when the keyboard
			// is connected.
			break;

		case STATE_UNKNOWN_DEVICE:
			// Nothing to do as the device is unknown.
			break;

		case STATE_NO_DEVICE:
			// Nothing is currently done in the main loop when the keyboard
			// is not connected.
			break;

		default:
			break;
	}


}



