#ifndef uart_pal_cfg_H
#define uart_pal_cfg_H

/*
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

// <e> UART over LPUART
//   <o1>Number of UART over LPLPUART instances  <1-10>
// </e>
#define UART_LPUART         0U
#define UART_LPUART_NO      1U

// <e> UART over FlexIO
//   <o1>Number of UART over FlexIO instances  <1-10>
// </e>
#define UART_FLEXIO         0U
#define UART_FLEXIO_NO      1U


/*
//-------- <<< end of configuration section >>> ------------------------------
*/


#if defined(UART_LPUART) && (UART_LPUART == 1)
  #define UART_OVER_LPUART
  #define NO_OF_LPUART_INSTS_FOR_UART       UART_LPUART_NO
#endif


#if defined(UART_FLEXIO) && (UART_FLEXIO == 1)
  #define UART_OVER_FLEXIO
  #define NO_OF_FLEXIO_INSTS_FOR_UART      UART_FLEXIO_NO
#endif

#endif /* uart_pal_cfg_H */
