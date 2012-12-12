/*
 * -----------------------------------------------------------------------------
 * File Name	: ns73m.h
 * Title		: NS73M driver
 * Description  : Implements communication between NS73M using a SPI-ish 3-wire
 *                protocol(IIC pin = 0). 
 * Author		: Andri Mar Jónsson.
 * License      : LGPL
 * Created		: 14/01/2008
 * Revised		: dd/mm/yyyy
 * Version		: 1.0
 * Target MCU	: Atmel AVR
 * Editor Tabs	: 4
 *
 * NOTE: IIC pin on the NS73M is assumed to be hardwired to 0(ground)
 * -----------------------------------------------------------------------------
 */


#ifndef NS73M_H
#define NS73M_H
#include "config.h"

#include <avr/io.h>
#include <util/delay.h>

#include "utils/macros.h"

/* Used Register Addresses */
#define NS73M_REG0 0x00
#define NS73M_REG1 0x01
#define NS73M_REG2 0x02
#define NS73M_REG3 0x03
#define NS73M_REG4 0x04
#define NS73M_REG5 0x05
#define NS73M_REG6 0x06
#define NS73M_REG7 0x07
#define NS73M_REG8 0x08
#define NS73M_REG14 0x0E

/* Bit mapping within above registers.
 * All listed for completenes sake and to make it easier to modify given the
 * fact that the datasheet available is only a perliminary version
 */

/* Register 0 */
#define NS73M_PE        0
#define NS73M_PDX       1
#define NS73M_MUTE      2
#define NS73M_S32K      3   //For test only
#define NS73M_EM        4
#define NS73M_EMS       5
#define NS73M_AG0       6
#define NS73M_AG1       7

/* Register 1 */
#define NS73M_DEV0      0   //For test only
#define NS73M_DEV1      1   //For test only
#define NS73M_DEV2      2   //For test only
#define NS73M_PLT       3
#define NS73M_R1_4      4   //For test only - No name in datasheet
#define NS73M_R1_5      5   //For test only - No name in datasheet
#define NS73M_SUBC      6
#define NS73M_HPF       7   //For test only

/* Register 2 */
#define NS73M_PL0       0
#define NS73M_PL1       1
#define NS73M_ULD       2
#define NS73M_RAMWEB    3   //For test only
#define NS73M_TMOD0     4   //For test only
#define NS73M_TMOD1     5   //For test only
#define NS73M_TMOD2     6   //For test only
#define NS73M_TMOD3     7   //For test only

/* Register 3 */
#define NS73M_P0        0   // Note P0-P13 probably never used directly
#define NS73M_P1        1   // 'cause it's cumbersome. More rational to
#define NS73M_P2        2   // calculate the 16 bit value and insert
#define NS73M_P3        3   // directly
#define NS73M_P4        4
#define NS73M_P5        5
#define NS73M_P6        6
#define NS73M_P7        7

/* Register 4 */
#define NS73M_P8        0
#define NS73M_P9        1
#define NS73M_P10       2
#define NS73M_P11       3
#define NS73M_P12       4
#define NS73M_P13       5
#define NS73M_FR0       6   //For test only
#define NS73M_FR1       7   //For test only

/* Register 5 */
#define NS73M_ID        0   //For test only
#define NS73M_TST0      1   //For test only
#define NS73M_TST1      2   //For test only
#define NS73M_TST2      3   //For test only
#define NS73M_TSTREG    4   //For test only
#define NS73M_MBBO      5   //For test only
#define NS73M_TEN       6   //For test only
#define NS73M_PEAU      7   //For test only

/* Register 6*/
#define NS73M_POLA      0   //For test only
#define NS73M_CIA0      1
#define NS73M_CIA1      2
#define NS73M_CIB0      3
#define NS73M_CIB1      4
#define NS73M_POLB      5   //For test only
#define NS73M_TESC      6   //For test only
#define NS73M_TESCC     7   //For test only

/* Register 7 */
#define NS73M_TEOS0     0   //For test only
#define NS73M_TEOS1     1   //For test only
#define NS73M_PDTEST    2   //For test only
#define NS73M_CPO0      3   //For test only
#define NS73M_CPO1      4   //For test only
#define NS73M_SMTES     5   //For test only
#define NS73M_PESY      6   //For test only
#define NS73M_PERF      7   //For test only

/* Register 8 */
#define NS73M_CEX0      0
#define NS73M_CEX1      1
#define NS73M_FMG0      2   //For test only
#define NS73M_FMG1      3   //For test only
#define NS73M_FA0       4   //For test only
#define NS73M_FA1       5   //For test only
#define NS73M_TAS0      6   //For test only
#define NS73M_TAS1      7   //For test only

/* Register 9 */
#define NS73M_PAB       0   //For Test only
#define NS73M_MPTEST0   1   //For Test only
#define NS73M_MPTEST1   2   //For Test only
#define NS73M_MPTEST2   3   //For Test only
#define NS73M_MPTEST3   4   //For Test only
#define NS73M_MPTEST4   5   //For Test only
#define NS73M_MPTEST5   6   //For Test only
#define NS73M_MPTEST6   7   //For Test only

/* Register 10 */
#define NS73M_MPTEST7   0   //For Test only
#define NS73M_MPTEST8   1   //For Test only
#define NS73M_MPTEST9   2   //For Test only
#define NS73M_MPTEST10  3   //For Test only
#define NS73M_MPTEST11  4   //For Test only
#define NS73M_MPTEST12  5   //For Test only
#define NS73M_MPTEST13  6   //For Test only
#define NS73M_MPTEST14  7   //For Test only

/* Register 11 */
#define NS73M_SEL_TEST0 0   //For Test only
#define NS73M_SEL_TEST1 1   //For Test only
#define NS73M_SEL_DATA0 2   //For Test only
#define NS73M_SEL_DATA1 3   //For Test only
#define NS73M_SEL_DATA2 4   //For Test only
#define NS73M_R10_5     5   //For Test only - Not even listed in datasheet
#define NS73M_R10_6     6   //For Test only - Not even listed in datasheet
#define NS73M_R10_7     7   //For Test only - Not even listed in datasheet

/* Register 14(0x0E) */
/* 
 * Nothing is listed in register 14.
   All we know from datasheet is that
   to do a software reset one writes
   0x05 to this register.
 */

/*Register defaults, accoding to a perliminary datasheet.*/
#define NS73M_REG0_DEFAULT 0x00
#define NS73M_REG1_DEFAULT 0x00
#define NS73M_REG2_DEFAULT 0x00
#define NS73M_REG3_DEFAULT 0x00
#define NS73M_REG4_DEFAULT 0x00
#define NS73M_REG5_DEFAULT 0x00
#define NS73M_REG6_DEFAULT 0x00
#define NS73M_REG7_DEFAULT 0x00
#define NS73M_REG8_DEFAULT 0x00


/*More logical "shortcuts"*/
#define NS73M_FREQ_LOWER    NS73M_REG3
#define NS73M_FREQ_UPPER    NS73M_REG4

/* Commands */
#define NS73M_SOFT_RES      0x05    //Software reset. Register 14

#define NS73M_BAND0         0x18    //Prefix: 000110xx, default for Reg. 8
#define NS73M_BAND1         0x19
#define NS73M_BAND2         0x1A
#define NS73M_BAND3         0x1B

extern void ns73m_init( void );
extern void ns73m_send( uint8_t _address, uint8_t _data );
extern uint16_t ns73m_calc_freq( uint16_t _freq );
extern uint8_t ns73m_get_band( uint16_t _calc_freq );
#endif /* NS73M_H */
