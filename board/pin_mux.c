/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Pins v6.0
processor: MK60DN512xxx10
package_id: MK60DN512VLL10
mcu_data: ksdk2_0
processor_version: 6.0.0
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

#include "fsl_common.h"
#include "fsl_port.h"
#include "pin_mux.h"

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitBootPins
 * Description   : Calls initialization functions.
 *
 * END ****************************************************************************************************************/
void BOARD_InitBootPins(void)
{
    BOARD_InitPins();
}

/* clang-format off */
/*
 * TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
BOARD_InitPins:
- options: {callFromInitBoot: 'true', coreID: core0, enableClock: 'true'}
- pin_list:
  - {pin_num: '33', peripheral: ENET, signal: CLKIN_1588, pin_signal: PTE26/ENET_1588_CLKIN/UART4_CTS_b/RTC_CLKOUT/USB_CLKIN}
  - {pin_num: '39', peripheral: ENET, signal: MII_RXER, pin_signal: PTA5/USB_CLKIN/FTM0_CH2/RMII0_RXER/MII0_RXER/CMP2_OUT/I2S0_TX_BCLK/JTAG_TRST_b}
  - {pin_num: '42', peripheral: ENET, signal: MII_RXD1, pin_signal: CMP2_IN0/PTA12/CAN0_TX/FTM1_CH0/RMII0_RXD1/MII0_RXD1/I2S0_TXD0/FTM1_QD_PHA}
  - {pin_num: '43', peripheral: ENET, signal: MII_RXD0, pin_signal: CMP2_IN1/PTA13/LLWU_P4/CAN0_RX/FTM1_CH1/RMII0_RXD0/MII0_RXD0/I2S0_TX_FS/FTM1_QD_PHB}
  - {pin_num: '45', peripheral: ENET, signal: mii_txen, pin_signal: PTA15/SPI0_SCK/UART0_RX/RMII0_TXEN/MII0_TXEN/I2S0_RXD0}
  - {pin_num: '46', peripheral: ENET, signal: MII_TXD0, pin_signal: PTA16/SPI0_SOUT/UART0_CTS_b/UART0_COL_b/RMII0_TXD0/MII0_TXD0/I2S0_RX_FS/I2S0_RXD1}
  - {pin_num: '47', peripheral: ENET, signal: MII_TXD1, pin_signal: ADC1_SE17/PTA17/SPI0_SIN/UART0_RTS_b/RMII0_TXD1/MII0_TXD1/I2S0_MCLK}
  - {pin_num: '53', peripheral: ENET, signal: MII_MDIO, pin_signal: ADC0_SE8/ADC1_SE8/TSI0_CH0/PTB0/LLWU_P5/I2C0_SCL/FTM1_CH0/RMII0_MDIO/MII0_MDIO/FTM1_QD_PHA}
  - {pin_num: '54', peripheral: ENET, signal: MII_MDC, pin_signal: ADC0_SE9/ADC1_SE9/TSI0_CH6/PTB1/I2C0_SDA/FTM1_CH1/RMII0_MDC/MII0_MDC/FTM1_QD_PHB}
  - {pin_num: '58', peripheral: UART3, signal: RX, pin_signal: ADC1_SE14/PTB10/SPI1_PCS0/UART3_RX/FB_AD19/FTM0_FLT1}
  - {pin_num: '59', peripheral: UART3, signal: TX, pin_signal: ADC1_SE15/PTB11/SPI1_SCK/UART3_TX/FB_AD18/FTM0_FLT2}
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS ***********
 */
/* clang-format on */

/* FUNCTION ************************************************************************************************************
 *
 * Function Name : BOARD_InitPins
 * Description   : Configures pin routing and optionally pin electrical features.
 *
 * END ****************************************************************************************************************/
void BOARD_InitPins(void)
{
    /* Port A Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortA);
    /* Port B Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortB);
    /* Port E Clock Gate Control: Clock enabled */
    CLOCK_EnableClock(kCLOCK_PortE);

    /* PORTA12 (pin 42) is configured as MII0_RXD1 */
    PORT_SetPinMux(PORTA, 12U, kPORT_MuxAlt4);

    /* PORTA13 (pin 43) is configured as MII0_RXD0 */
    PORT_SetPinMux(PORTA, 13U, kPORT_MuxAlt4);

    /* PORTA15 (pin 45) is configured as MII0_TXEN */
    PORT_SetPinMux(PORTA, 15U, kPORT_MuxAlt4);

    /* PORTA16 (pin 46) is configured as MII0_TXD0 */
    PORT_SetPinMux(PORTA, 16U, kPORT_MuxAlt4);

    /* PORTA17 (pin 47) is configured as MII0_TXD1 */
    PORT_SetPinMux(PORTA, 17U, kPORT_MuxAlt4);

    /* PORTA5 (pin 39) is configured as MII0_RXER */
    PORT_SetPinMux(PORTA, 5U, kPORT_MuxAlt4);

    /* PORTB0 (pin 53) is configured as MII0_MDIO */
    PORT_SetPinMux(PORTB, 0U, kPORT_MuxAlt4);

    /* PORTB1 (pin 54) is configured as MII0_MDC */
    PORT_SetPinMux(PORTB, 1U, kPORT_MuxAlt4);

    /* PORTB10 (pin 58) is configured as UART3_RX */
    PORT_SetPinMux(PORTB, 10U, kPORT_MuxAlt3);

    /* PORTB11 (pin 59) is configured as UART3_TX */
    PORT_SetPinMux(PORTB, 11U, kPORT_MuxAlt3);

    /* PORTE26 (pin 33) is configured as ENET_1588_CLKIN */
    PORT_SetPinMux(PORTE, 26U, kPORT_MuxAlt2);
}
/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
