#ifndef SPI_PAL_cfg_H
#define SPI_PAL_cfg_H

/*
//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
*/

// <e> SPI over LPSPI
//   <o1>Number of SPI over LPSPI instances  <1-10>
// </e>
#define SPI_LPSPI         0U
#define SPI_LPSPI_NO      1U

// <e> SPI over FlexIO
//   <o1>Number of SPI over FlexIO instances  <1-10>
// </e>
#define SPI_FLEXIO        0U
#define SPI_FLEXIO_NO     1U


/*
//-------- <<< end of configuration section >>> ------------------------------
*/


#if defined(SPI_LPSPI) && (SPI_LPSPI == 1)
  #define SPI_OVER_LPSPI
  #define NO_OF_LPSPI_INSTS_FOR_SPI       SPI_LPSPI_NO
#endif


#if defined(SPI_FLEXIO) && (SPI_FLEXIO == 1)
  #define SPI_OVER_FLEXIO
  #define NO_OF_FLEXIO_INSTS_FOR_SPI      SPI_FLEXIO_NO
#endif

#endif /* SPI_PAL_cfg_H */
