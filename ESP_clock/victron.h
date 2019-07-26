/*
  config.h - config the keywords for Victron.DIRECT
*/

#define MPPT_75_10
// MPPT 75 | 10
#ifdef MPPT_75_10

const byte buffsize = 32;
const byte value_bytes = 33;
const byte label_bytes = 9;
const byte num_keywords = 18;

char keywords[num_keywords][label_bytes] = {
  "PID",
  "FW",
  "SER#",
  "V",
  "I",
  "VPV",
  "PPV",
  "CS",
  "ERR",
  "LOAD",
  "IL",
  "H19",
  "H20",
  "H21",
  "H22",
  "H23",
  "HSDS",
  "Checksum"
};
// source: VE.Direct Protocol Whitepaper 3.24
#define PID 0 //Product ID
#define FW 1 //Firmware Version
#define SER 2 //Serial number (SER#)
#define V 3     // Battery Voltage in mV
#define I 4     // Battery Current in mA
#define VPV 5   // Panel Voltage in mV
#define PPV 6   // Panel Power in Watt
#define CS 7    // State of Operation (0-9: whitepaper for reference)
#define ERR 8   // Error code (0-116: whitepaper for reference)
#define LOAD 9  // Load output State (ON/OFF)
#define IL 10   // Load current in mA
#define H19 11  // Yield total (user resettable counter) in 0.01 kWh
#define H20 12 // Yield today in 0.01 kWh
#define H21 13 // Maximum Power today in W
#define H22 14 // Yield yesterday in 0.01 kWh
#define H23 15 // Maximum Power yesterday in W
#define HSDS 16 //Day sequence number (0..364)
#define CHECKSUM 17
#endif
