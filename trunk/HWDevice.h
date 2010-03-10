

#ifndef HWX_HWDEVICE_H
#define HWX_HWDEVICE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <windef.h>


void			WINAPI BacklightForce(BOOL);
void			WINAPI BacklightLight();
//////////////////////////////////////////////////////////////////////////
LONG			WINAPI XUE_GetIMEI(LPWSTR, LONG);
LONG			WINAPI XUE_GetDeviceID(LPWSTR, LONG);
LONG			WINAPI XUE_GetSDID(LPTSTR, LONG);
LONG			WINAPI XUE_GetMAC( LPWSTR, LONG);
//////////////////////////////////////////////////////////////////////////
// Lock
#ifndef SN_LOCK_BITMASK_DEVICELOCKED
#define SN_LOCK_BITMASK_DEVICELOCKED    0x00000001  // Device locked
#endif
#ifndef SN_LOCK_BITMASK_KEYLOCKED
#define SN_LOCK_BITMASK_KEYLOCKED       0x00000002  // Key locked
#endif
#ifndef SN_LOCK_BITMASK_SIMLOCKED
#define SN_LOCK_BITMASK_SIMLOCKED       0x00000004  // SIM locked
#endif
DWORD			WINAPI	XUE_GetLockState();
BOOL			WINAPI	XUE_IsKeyboardLocked();
//////////////////////////////////////////////////////////////////////////
//WiFi
#ifndef SN_WIFISTATEHARDWAREPRESENT_BITMASK // WiFiStateHardwarePresent
#define SN_WIFISTATEHARDWAREPRESENT_BITMASK 1
#endif
#ifndef SN_WIFISTATEPOWERON_BITMASK	// WiFiStatePowerOn
#define SN_WIFISTATEPOWERON_BITMASK 2
#endif
#ifndef SN_WIFISTATENETWORKSAVAILABLE_BITMASK // WiFiStateNetworksAvailable
#define SN_WIFISTATENETWORKSAVAILABLE_BITMASK 4
#endif
#ifndef SN_WIFISTATECONNECTING_BITMASK // WiFiStateConnecting
#define SN_WIFISTATECONNECTING_BITMASK 8
#endif
#ifndef SN_WIFISTATECONNECTED_BITMASK // WiFiStateConnected
#define SN_WIFISTATECONNECTED_BITMASK 0x10
#endif
DWORD			WINAPI	XUE_GetWiFiState();

////////////////////////////////////////////////////////////////////////////////
// BLUETHOOTH
#ifndef SN_BLUETOOTHSTATEPOWERON_BITMASK // BluetoothStatePowerOn
#define SN_BLUETOOTHSTATEPOWERON_BITMASK 1
#endif
#ifndef SN_BLUETOOTHSTATEDISCOVERABLE_BITMASK // BluetoothStateDiscoverable
#define SN_BLUETOOTHSTATEDISCOVERABLE_BITMASK 2
#endif
#ifndef SN_BLUETOOTHSTATEA2DPCONNECTED_BITMASK // BluetoothStateA2DPConnected
#define SN_BLUETOOTHSTATEA2DPCONNECTED_BITMASK 4
#endif
#ifndef SN_BLUETOOTHSTATEHARDWAREPRESENT_BITMASK // BluetoothStateHardwarePresent
#define SN_BLUETOOTHSTATEHARDWAREPRESENT_BITMASK 8
#endif
#ifndef SN_BLUETOOTHSTATEHANDSFERECONTROL_BITMASK // BluetoothStateHandsFreeControl
#define SN_BLUETOOTHSTATEHANDSFERECONTROL_BITMASK 0x10
#endif
#ifndef SN_BLUETOOTHSTATEHANDSFREEAUDIO_BITMASK // BluetoothStateHandsFreeAudio
#define SN_BLUETOOTHSTATEHANDSFREEAUDIO_BITMASK 0x20
#endif

DWORD			WINAPI	XUE_GetBluetoothState();

////////////////////////////////////////////////////////////////////////////////
//Phone
#ifndef SN_PHONESIMFULL_BITMASK // PhoneSimFull
#define SN_PHONESIMFULL_BITMASK 1
#endif
#ifndef SN_PHONENOSIM_BITMASK // PhoneNoSim
#define SN_PHONENOSIM_BITMASK 2
#endif
#ifndef SN_PHONEINVALIDSIM_BITMASK // PhoneInvalidSim
#define SN_PHONEINVALIDSIM_BITMASK 4
#endif
#ifndef SN_PHONEBLOCKEDSIM_BITMASK // PhoneBlockedSim
#define SN_PHONEBLOCKEDSIM_BITMASK 8
#endif
#ifndef SN_PHONERADIOOFF_BITMASK // PhoneRadioOff
#define SN_PHONERADIOOFF_BITMASK 0x10
#endif
#ifndef SN_PHONERADIOPRESENT_BITMASK // PhoneRadioPresent
#define SN_PHONERADIOPRESENT_BITMASK 0x20
#endif
#ifndef SN_PHONERINGEROFF_BITMASK// PhoneRingerOff
#define SN_PHONERINGEROFF_BITMASK 0x40
#endif
#ifndef SN_PHONELINE1SELECTED_BITMASK // PhoneLine1Selected
#define SN_PHONELINE1SELECTED_BITMASK 0x80
#endif
#ifdef SN_PHONELINE2SELECTED_BITMASK// PhoneLine2Selected
#define SN_PHONELINE2SELECTED_BITMASK 0x100
#endif
#ifndef SN_PHONEROAMING_BITMASK// PhoneRoaming
#define SN_PHONEROAMING_BITMASK 0x200
#endif
#ifndef SN_PHONECALLFORWARDINGONLINE1_BITMASK// PhoneCallForwardingOnLine1
#define SN_PHONECALLFORWARDINGONLINE1_BITMASK 0x400
#endif
#ifndef SN_PHONEMISSEDCALL_BITMASK // PhoneMissedCall
#define SN_PHONEMISSEDCALL_BITMASK 0x800
#endif
#ifdef SN_PHONEACTIVEDATACALL_BITMASK // PhoneActiveDataCall
#define SN_PHONEACTIVEDATACALL_BITMASK 0x1000
#endif
#ifdef SN_PHONECALLBARRING_BITMASK // PhoneCallBarring
#define SN_PHONECALLBARRING_BITMASK 0x2000
#endif
#ifdef SN_PHONECALLONHOLD_BITMASK// PhoneCallOnHold
#define SN_PHONECALLONHOLD_BITMASK 0x4000
#endif
#ifdef SN_PHONECONFERENCECALL_BITMASK// PhoneConferenceCall
#define SN_PHONECONFERENCECALL_BITMASK 0x8000
#endif 
#ifndef SN_PHONEINCOMINGCALL_BITMASK// PhoneIncomingCall
#define SN_PHONEINCOMINGCALL_BITMASK 0x10000
#endif
#ifndef SN_PHONECALLCALLING_BITMASK// PhoneCallCalling
#define SN_PHONECALLCALLING_BITMASK 0x20000
#endif
#ifndef SN_PHONEGPRSCOVERAGE_BITMASK// PhoneGprsCoverage
#define SN_PHONEGPRSCOVERAGE_BITMASK 0x100000
#endif
#ifndef SN_PHONENOSERVICE_BITMASK// PhoneNoService
#define SN_PHONENOSERVICE_BITMASK 0x200000
#endif
#ifndef SN_PHONESEARCHINGFORSERVICE_BITMASK // PhoneSearchingForService
#define SN_PHONESEARCHINGFORSERVICE_BITMASK 0x400000
#endif
#ifndef SN_PHONEHOMESERVICE_BITMASK// PhoneHomeService
#define SN_PHONEHOMESERVICE_BITMASK 0x800000
#endif
#ifndef SN_PHONE1XRTTCOVERAGE_BITMASK// Phone1xRttCoverage
#define SN_PHONE1XRTTCOVERAGE_BITMASK 0x10000000
#endif
#ifndef SN_PHONECALLTALKING_BITMASK// PhoneCallTalking
#define SN_PHONECALLTALKING_BITMASK 0x20000000
#endif
#ifndef SN_PHONECALLTALKING_BITMASK// PhoneCallForwardingOnLine2
#define SN_PHONECALLFORWARDINGONLINE2_BITMASK 0x40000000
#endif

DWORD			WINAPI	XUE_GetPhoneState();
BOOL			WINAPI	XUE_IsFlightModeOn();


BOOL WINAPI XUE_GetSDID2(UINT& uSDID);
BOOL WINAPI XUE_GetSDID3(UINT& uSDID);
#endif //HWX_HWDEVICE_H
