/* Copyright (c) 2002, Marek Michalkiewicz
   Copyright (c) 2005, 2007 Joerg Wunsch
   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

   * Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

   * Neither the name of the copyright holders nor the names of
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE. */

/* $Id: twi.h,v 1.2 2007/01/23 15:32:48 joerg_wunsch Exp $ */
/* copied from: Id: avr/twi.h,v 1.4 2004/11/01 21:19:54 arcanum Exp */

#ifndef _TWI_H_
#define _TWI_H_

#include <avr/io.h>


/** \name TWSR values

  Mnemonics:
  <br>TW_MT_xxx - master transmitter
  <br>TW_MR_xxx - master receiver
  <br>TW_ST_xxx - slave transmitter
  <br>TW_SR_xxx - slave receiver
 
*/

/* Master */

#define TW_START		0x08		// Start condition transmitted
#define TW_REP_START		0x10		// Repeated start condition transmitted


/* Master Transmitter */

#define TW_MT_SLA_ACK		0x18		// SLA+W transmitted, ACK received
#define TW_MT_SLA_NACK		0x20		// SLA+W transmitted, NACK received
#define TW_MT_DATA_ACK		0x28		// Data transmitted, ACK received
#define TW_MT_DATA_NACK		0x30		// Data transmitted, NACK received

#define TW_MT_ARB_LOST		0x38		// Arbitration lost in SLA+W or data
#define TW_MR_ARB_LOST		0x38		// Arbitration lost in SLA+R or NACK

#define TW_MR_SLA_ACK		0x40		// SLA+R transmitted, ACK received
#define TW_MR_SLA_NACK		0x48		// SLA+R transmitted, NACK received
#define TW_MR_DATA_ACK		0x50		// Data received, ACK returned
#define TW_MR_DATA_NACK		0x58		// Data received, NACK returned

/* Slave Transmitter*/

#define TW_ST_SLA_ACK		0xA8		// SLA+R received, ACK returned
#define TW_ST_ARB_LOST_SLA_ACK	0xB0		// Arbitration lost in SLA+RW, SLA+R received, ACK returned
#define TW_ST_DATA_ACK		0xB8		// Data transmitted, ACK received
#define TW_ST_DATA_NACK		0xC0		// Data transmitted, NACK received
#define TW_ST_LAST_DATA		0xC8		// Last data byte transmitted, ACK received


/* Slave Receiver */

#define TW_SR_SLA_ACK		0x60		// SLA+W received, ACK returned
#define TW_SR_ARB_LOST_SLA_ACK	0x68		// Arbitration lost in SLA+RW, SLA+W received, ACK returned
#define TW_SR_GCALL_ACK		0x70		// General call received, ACK returned
#define TW_SR_ARB_LOST_GCALL_ACK 0x78		// Arbitration lost in SLA+RW, general call received, ACK returned
#define TW_SR_DATA_ACK		0x80		// Data received, ACK returned
#define TW_SR_DATA_NACK		0x88		// Data received, NACK returned
#define TW_SR_GCALL_DATA_ACK	0x90		// General call data received, ACK returned
#define TW_SR_GCALL_DATA_NACK	0x98		// General call data received, NACK returned
#define TW_SR_STOP		0xA0		// Stop or repeated start condition received while selected 

/* Misc */

#define TW_NO_INFO		0xF8		// No state information available
#define TW_BUS_ERROR		0x00		// Illegal start or stop condition


/**
 * The lower 3 bits of TWSR are reserved on the ATmega163.
 * The 2 LSB carry the prescaler bits on the newer ATmegas.
 */
#define TW_STATUS_MASK		(_BV(TWS7)|_BV(TWS6)|_BV(TWS5)|_BV(TWS4)|\
				_BV(TWS3))
/**
 * TWSR, masked by TW_STATUS_MASK
 */
#define TW_STATUS		(TWSR & TW_STATUS_MASK)



/**
 * name R/~W bit in SLA+R/W address field
 */
#define TW_READ		1
#define TW_WRITE	0


#endif  /* _TWI_H_ */
