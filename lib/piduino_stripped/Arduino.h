/*
  Arduino.h - Main include file for the Arduino SDK
  Copyright (c) 2005-2013 Arduino Team.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2018 by Pascal JEAN (pascal.jean@piduino.org) for piduino library
*/
#ifndef Arduino_h
#define Arduino_h

#ifdef __DOXYGEN__

// Time ------------------------------------------------------------------------
/**
 * @brief Pause pour une durée en millisecondes
 * @param ms durée de la pause en ms, -1 pour endormir le thread
 */
void delay(unsigned long ms);

/**
 * @brief Pause pour une durée en microsecondes
 * @param ms durée de la pause en us, -1 pour endormir le thread
 */
void delayMicroseconds(unsigned long us);

/**
 * @brief Nombre de millisecondes depuis le lancement du programme
 */
unsigned long millis();

/**
 * @brief Nombre de microsecondes depuis le lancement du programme
 */
unsigned long micros();

/**
* @}
*/

#else /* End of the documentation, __DOXYGEN__ not defined ------------------ */

// -----------------------------------------------------------------------------
// part C/C++

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

  /* types ==================================================================== */
  typedef unsigned int word;
  typedef uint8_t boolean;
  typedef uint8_t byte;
  typedef void (*Isr)(void);

/* constants ================================================================ */
#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105
#define EULER 2.718281828459045235360287471352

  /* macros =================================================================== */

#ifndef __cplusplus
#ifndef min
#define min(a, b) ((a) < (b) ? (a) : (b))
#endif
#ifndef max
#define max(a, b) ((a) > (b) ? (a) : (b))
#endif
#endif

#ifndef constrain
#define constrain(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#endif
#ifndef radians
#define radians(deg) ((deg)*DEG_TO_RAD)
#endif
#ifndef degrees
#define degrees(rad) ((rad)*RAD_TO_DEG)
#endif
#ifndef sq
#define sq(x) ((x) * (x))
#endif

#define lowByte(w) ((uint8_t)((w)&0xff))
#define highByte(w) ((uint8_t)((w) >> 8))

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bit(b) (1UL << (b))

#define digitalPinToInterrupt(p) (p)

#ifdef __cplusplus
} // extern "C"
#endif

#ifdef __cplusplus
// -----------------------------------------------------------------------------
// part only C++
//
#include <iostream>
#include <algorithm>
#include <piduino_string.h>

typedef Piduino::String String;
/*
#include <chrono>
#include <thread>

void delay(unsigned long ms)
{
  std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}*/

void delay(unsigned long ms);

#define EXTERN_C extern "C"

#else /* __cplusplus not defined */
// -----------------------------------------------------------------------------
// part only C
//
#include <stdbool.h>
#include <stdio.h>

#define EXTERN_C

// -----------------------------------------------------------------------------
#endif /* __cplusplus not defined */

// -----------------------------------------------------------------------------
// part C/C++
#ifdef __cplusplus
extern "C"
{
#endif
  void delayMicroseconds(unsigned long us);
  //unsigned long millis();
  unsigned long micros();

#ifdef __cplusplus
}
#endif

#endif /* __DOXYGEN__ not defined */
/* ========================================================================== */
#endif /*Arduino_h defined */
