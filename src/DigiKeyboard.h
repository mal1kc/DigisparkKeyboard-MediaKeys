/*
 * Based on Obdev's AVRUSB code and under the same license.
 *
 * TODO: Make a proper file header. :-)
 * Modified for Digispark by Digistump
 */
#ifndef __DigiKeyboard_h__
#define __DigiKeyboard_h__

#include <Arduino.h>
#include <avr/delay.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <string.h>

#include "usbdrv.h"
//#include "scancode-ascii-table.h"

//#define kbd_es_es

#ifdef kbd_be_be
#include "digi_be_be.h"
#endif
#ifdef kbd_cz_cz
#include "digi_cz_cz.h"
#endif
#ifdef kbd_da_dk
#include "digi_da_dk.h"
#endif
#ifdef kbd_de_de
#include "digi_de_de.h"
#endif
#ifdef kbd_es_es
#include "digi_es_es.h"
#endif
#ifdef kbd_fi_fi
#include "digi_fi_fi.h"
#endif
#ifdef kbd_fr_fr
#include "digi_fr_fr.h"
#endif
#ifdef kbd_it_it
#include "digi_it_it.h"
#endif
#ifdef kbd_pt_pt
#include "digi_pt_pt.h"
#endif
#ifdef kbd_tr_tr
#include "digi_tr_tr.h"
#endif
#ifndef _kbd_lang
#include "digi_en_us.h"
#endif

// TODO: Work around Arduino 12 issues better.
//#include <WConstants.h>
//#undef int()

typedef uint8_t byte;

#define BUFFER_SIZE 2 // Minimum of 2: 1 for modifiers + 1 for keystroke

static uchar idleRate; // in 4 ms units

/* We use a simplifed keyboard report descriptor which does not support the
 * boot protocol. We don't allow setting status LEDs and but we do allow
 * simultaneous key presses.
 * The report descriptor has been created with usb.org's "HID Descriptor Tool"
 * which can be downloaded from http://www.usb.org/developers/hidpage/.
 * Redundant entries (such as LOGICAL_MINIMUM and USAGE_PAGE) have been omitted
 * for the second INPUT item.
 */
const PROGMEM char
    usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
        /* USB report descriptor */
        0x05, 0x01, // USAGE_PAGE (Generic Desktop)
        0x09, 0x06, // USAGE (Keyboard)
        0xa1, 0x01, // COLLECTION (Application)
        0x85, 0x01, // Report ID (1) // for scpecify keyboard keys
        0x05, 0x07, //   USAGE_PAGE (Keyboard)
        0x19, 0xe0, //   USAGE_MINIMUM (Keyboard LeftControl)
        0x29, 0xe7, //   USAGE_MAXIMUM (Keyboard Right GUI)
        0x15, 0x00, //   LOGICAL_MINIMUM (0)
        0x25, 0x01, //   LOGICAL_MAXIMUM (1)
        0x75, 0x01, //   REPORT_SIZE (1)
        0x95, 0x08, //   REPORT_COUNT (8)
        0x81, 0x02, //   INPUT (Data,Var,Abs)
        0x95, 0x01, //   REPORT_COUNT (simultaneous keystrokes)
        0x75, 0x08, //   REPORT_SIZE (8)
        0x25, 0x65, //   LOGICAL_MAXIMUM (101)
        0x19, 0x00, //   USAGE_MINIMUM (Reserved (no event indicated))
        0x29, 0x65, //   USAGE_MAXIMUM (Keyboard Application)
        0x81, 0x00, //   INPUT (Data,Ary,Abs)
        0xc0,       // END_COLLECTION
                    // 37
        0x05, 0x0C, // Usage Page (Consumer)
        0x09, 0x01, // Usage (Consumer Control)
        0xA1, 0x01, // Collection (Application)
        0x85, 0x02, //   Report ID (2)
        0x05, 0x0C, //   Usage Page (Consumer)
        0x15, 0x00, //   Logical Minimum (0)
        0x25, 0x01, //   Logical Maximum (1)
        0x75, 0x01, //   Report Size (1)
        0x95, 0x08, //   Report Count (4) // keys count i use
        /* 0x95, 0x08, //   Report Count (7) // if i use volum mute + - */
        0x09, 0xB5, //   Usage (Scan Next Track)
        0x09, 0xB6, //   Usage (Scan Previous Track)
        0x09, 0xCD, //   Usage (Play/Pause)
        /* 0x09, 0xE2, //   Usage (Mute) */
        /* 0x09, 0xE9, //   Usage (Volume Increment) */
        /* 0x09, 0xEA, //   Usage (Volume Decrement) */
        0x81, 0x02, //   Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No
                    //   Null Position)
        0xC0,       // End Collection
                    // + 33 +- 6 (vol + - x)
                    // 70 with vol + - x
                    // 64 without
};

/* Keyboard usage values, see usb.org's HID-usage-tables document, chapter
 * 10 Keyboard/Keypad Page for more codes.
 */
/* #define MOD_CONTROL_LEFT (1 << 0) */
/* #define MOD_SHIFT_LEFT (1 << 1) */
/* #define MOD_ALT_LEFT (1 << 2) */
/* #define MOD_GUI_LEFT (1 << 3) */
/* #define MOD_CONTROL_RIGHT (1 << 4) */
/* #define MOD_SHIFT_RIGHT (1 << 5) */
/* #define MOD_ALT_RIGHT (1 << 6) */
/* #define MOD_GUI_RIGHT (1 << 7) */
#define MOD_CONTROL_LEFT 0x01
#define MOD_SHIFT_LEFT 0x02
#define MOD_ALT_LEFT 0x04
#define MOD_GUI_LEFT 0x08 // (Win/Apple/Meta)
#define MOD_CTRL_RIGHT 0x10
#define MOD_SHIFT_RIGHT 0x20
#define MOD_ALT_RIGHT 0x40
#define MOD_GUI_RIGHT 0x80

#define KEY_A 4
#define KEY_B 5
#define KEY_C 6
#define KEY_D 7
#define KEY_E 8
#define KEY_F 9
#define KEY_G 10
#define KEY_H 11
#define KEY_I 12
#define KEY_J 13
#define KEY_K 14
#define KEY_L 15
#define KEY_M 16
#define KEY_N 17
#define KEY_O 18
#define KEY_P 19
#define KEY_Q 20
#define KEY_R 21
#define KEY_S 22
#define KEY_T 23
#define KEY_U 24
#define KEY_V 25
#define KEY_W 26
#define KEY_X 27
#define KEY_Y 28
#define KEY_Z 29
#define KEY_1 30
#define KEY_2 31
#define KEY_3 32
#define KEY_4 33
#define KEY_5 34
#define KEY_6 35
#define KEY_7 36
#define KEY_8 37
#define KEY_9 38
#define KEY_0 39

#define KEY_ENTER 40

#define KEY_SPACE 44

#define KEY_F1 58
#define KEY_F2 59
#define KEY_F3 60
#define KEY_F4 61
#define KEY_F5 62
#define KEY_F6 63
#define KEY_F7 64
#define KEY_F8 65
#define KEY_F9 66
#define KEY_F10 67
#define KEY_F11 68
#define KEY_F12 69

#define KEY_ARROW_LEFT 0x50

#define KEY_MEDIA_PLAYPAUSE 0xBC
#define KEY_MEDIA_NEXT 0xB5
#define KEY_MEDIA_PREV 0xB6

class DigiKeyboardDevice : public Print {
public:
  DigiKeyboardDevice() {
    cli();
    usbDeviceDisconnect();
    _delay_ms(250);
    usbDeviceConnect();

    usbInit();

    sei();

    // TODO: Remove the next two lines once we fix
    //       missing first keystroke bug properly.
    memset(reportBuffer, 0, sizeof(reportBuffer));
    usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
  }

  void update() { usbPoll(); }

  // delay while updating until we are finished delaying
  void delay(long milli) {
    unsigned long last = millis();
    while (milli > 0) {
      unsigned long now = millis();
      milli -= now - last;
      last = now;
      update();
    }
  }

  void sendMediaKeyStroke(byte mediaKey) {

    while (!usbInterruptIsReady()) {
      usbPoll();
      _delay_ms(5);
    }

    byte reportBuff[2] = {0}; // create a new report buffer with 3 bytes

    reportBuff[0] =
        2; // start with report id // in my lib it will be 2 for consumer page
    reportBuff[1] = mediaKey; // set mediakey bit to 1

    usbSetInterrupt(reportBuff,
                    sizeof(reportBuff)); // send the report buffer //
    // unpress
    while (!usbInterruptIsReady()) {
      usbPoll();
      _delay_ms(5);
    }

    reportBuff[0, 1] = 0; // unpress
    usbSetInterrupt(reportBuff, sizeof(reportBuff));
  }

  // sendKeyStroke: sends a key press AND release
  void sendKeyStroke(byte keyStroke) { sendKeyStroke(keyStroke, 0); }

  // sendKeyStroke: sends a key press AND release with modifiers
  void sendKeyStroke(byte keyStroke, byte modifiers) {
    sendKeyPress(keyStroke, modifiers);
    // This stops endlessly repeating keystrokes:
    sendKeyPress(0, 0);
  }

  // sendKeyPress: sends a key press only - no release
  // to release the key, send again with keyPress=0
  void sendKeyPress(byte keyPress) { sendKeyPress(keyPress, 0); }

  // sendKeyPress: sends a key press only, with modifiers - no release
  // to release the key, send again with keyPress=0
  void sendKeyPress(byte keyPress, byte modifiers) {
    while (!usbInterruptIsReady()) {
      // Note: We wait until we can send keyPress
      //       so we know the previous keyPress was
      //       sent.
      usbPoll();
      _delay_ms(5);
    }

    memset(reportBuffer, 0, sizeof(reportBuffer));
    reportBuffer[0] = 1; // reportID for keyboard
    reportBuffer[1] = modifiers;
    reportBuffer[2] = keyPress;

    usbSetInterrupt(reportBuffer, sizeof(reportBuffer));
  }
  size_t write(uint8_t chr) {
    uint8_t data = pgm_read_byte_near(ascii_to_scan_code_table + (chr - 8));

    uint8_t tmpmodifiers = 0;

    if (chr == '<') {
      sendKeyStroke(100);
    } else if (chr == '>') {
      sendKeyStroke(100, MOD_SHIFT_RIGHT);
    } else if (chr == '|') {
      sendKeyStroke(100, MOD_ALT_RIGHT);
    } else {
      sendKeyStroke(data & 0b00111111, data >> 7              ? MOD_SHIFT_RIGHT
                                       : 0 | (data << 1) >> 7 ? MOD_ALT_RIGHT
                                                              : 0);
    }

    return 1;
  }
  size_t write2(uint8_t chr) {
    if (chr == 'ı') {
      sendKeyStroke(51, KEY_I);
    } else {
      uint8_t data = pgm_read_byte_near(ascii_to_scan_code_table + (chr - 8));
      uint8_t tmpmodifiers = 0;

      if (data &
          0x80) { // it's a capital letter or other character reached with shift
        tmpmodifiers |= 0x02; // set the left shift modifier
        data &= 0x7F;
      }
      if (data & 0x40) {      // it's an altgr (ALT_RIGHT) character
        tmpmodifiers |= 0x40; // set the altgr modifier
        data &= 0x3F;
      }
      if (data == 0x03) {
        data = 0x64;
      }

      sendKeyStroke(data, tmpmodifiers);
      sendKeyStroke(data & 0b01111111, data >> 7 ? MOD_SHIFT_RIGHT : 0);
    }
    return 1;
  }

  // private: TODO: Make friend?
  uchar reportBuffer[3]; // buffer for HID reports [ 1 modifier byte + (len-1)
                         // key strokes]
  using Print::write;
};

DigiKeyboardDevice DigiKeyboard = DigiKeyboardDevice();

#ifdef __cplusplus
extern "C" {
#endif
// USB_PUBLIC uchar usbFunctionSetup
uchar usbFunctionSetup(uchar data[8]) {
  usbRequest_t *rq = (usbRequest_t *)((void *)data);

  usbMsgPtr = DigiKeyboard.reportBuffer; //
  if ((rq->bmRequestType & USBRQ_TYPE_MASK) == USBRQ_TYPE_CLASS) {
    /* class request type */

    if (rq->bRequest == USBRQ_HID_GET_REPORT) {
      /* wValue: ReportType (highbyte), ReportID (lowbyte) */

      /* we only have one report type, so don't look at wValue */
      // TODO: Ensure it's okay not to return anything here?
      return 0;

    } else if (rq->bRequest == USBRQ_HID_GET_IDLE) {
      // usbMsgPtr = &idleRate;
      // return 1;
      return 0;

    } else if (rq->bRequest == USBRQ_HID_SET_IDLE) {
      idleRate = rq->wValue.bytes[1];
    }
  } else {
    /* no vendor specific requests implemented */
  }

  return 0;
}
#ifdef __cplusplus
} // extern "C"
#endif

#endif // __DigiKeyboard_h__
