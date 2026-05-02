#include "main.h"
#include <stdint.h>

// polling paradigm to test peripheral
static uint8_t spi_transfer(uint8_t tx) {
  while(!LL_SPI_IsActiveFlag_TXE(SPI2));

  LL_SPI_TransmitData8(SPI2, tx);

  // RXNE: receive data register not empty
  // set when a new byte is received
  while(!LL_SPI_IsActiveFlag_RXNE(SPI2));

  return LL_SPI_ReceiveData8(SPI2);
}

uint8_t imu_read_whoami(void) {
  // SPI: active-low chip-select pin to select target device
  LL_GPIO_ResetOutputPin(IMU_CS_GPIO_Port, IMU_CS_Pin);

  // set the read bit on the WHO_AM_REGISTER (0x75)
  spi_transfer(0x75 | (1 << 7));
  
  // to receive any payload the SCLK must be clocked 8 more times
  // executes a MOSI and MISO full-duplex transfer
  uint8_t res = spi_transfer(0x00);

  // disengage connection with target device
  LL_GPIO_SetOutputPin(IMU_CS_GPIO_Port, IMU_CS_Pin);
  return res;
}
