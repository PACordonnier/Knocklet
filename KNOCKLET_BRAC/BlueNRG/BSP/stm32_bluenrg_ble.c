/**
  ******************************************************************************
  * @file    stm32_bluenrg_ble.c
  * @author  CL
  * @version V1.0.0
  * @date    04-July-2014
  * @brief   
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "stm32_bluenrg_ble.h"
#include "gp_timer.h"
#include "debug.h"

extern volatile uint32_t ms_counter;

/** @addtogroup BSP
 *  @{
 */

/** @defgroup X-NUCLEO-IDB04A1
 *  @{
 */
 
/** @defgroup STM32_BLUENRG_BLE
 *  @{
 */

/** @defgroup STM32_BLUENRG_BLE_Private_Defines 
 * @{
 */ 

#define HEADER_SIZE 5
#define MAX_BUFFER_SIZE 255
#define TIMEOUT_DURATION 15

/**
 * @}
 */

/** @defgroup STM32_BLUENRG_BLE_Private_Variables
 * @{
 */

SPI_HandleTypeDef SpiHandle;

/**
 * @}
 */

/** @defgroup STM32_BLUENRG_BLE_Private_Function_Prototypes 
 *  @{
 */

/* Private function prototypes -----------------------------------------------*/
static void us150Delay(void);
void set_irq_as_output(void);
void set_irq_as_input(void);

/**
 * @}
 */ 

/** @defgroup STM32_BLUENRG_BLE_Exported_Functions 
 * @{
 */ 

/**
 * @brief  This function is a utility to print the log time
*          in the format HH:MM:SS:MSS (DK GUI time format)
 * @param  None
 * @retval None
 */
void print_csv_time(void){
  uint32_t ms = ms_counter;
  PRINT_CSV("%02d:%02d:%02d.%03d", ms/(60*60*1000)%24, ms/(60*1000)%60, (ms/1000)%60, ms%1000);
}

/**
 * @brief  Writes data to a serial interface.
 * @param  data1   :  1st buffer
 * @param  data2   :  2nd buffer
 * @param  n_bytes1: number of bytes in 1st buffer
 * @param  n_bytes2: number of bytes in 2nd buffer
 * @retval None
 */
void Hal_Write_Serial(const void* data1, const void* data2, int32_t n_bytes1,
                      int32_t n_bytes2)
{
#ifdef OPTIMIZED_SPI /* used by the server (L0 and F4, not L4) for the throughput test */
  struct timer t;
  int ret;
  uint8_t data2_offset = 0;
  
  Timer_Set(&t, CLOCK_SECOND/10);
  
  Disable_SPI_IRQ();
  
  while(1){
    ret = BlueNRG_SPI_Write(&SpiHandle, (uint8_t *)data1,(uint8_t *)data2 + data2_offset, n_bytes1, n_bytes2);
    
    if(ret >= 0){      
      n_bytes1 = 0;
      n_bytes2 -= ret;
      data2_offset += ret;
      if(n_bytes2==0)
        break;
    }
    
    if(Timer_Expired(&t)){
      break;
    }
  }
  
  Enable_SPI_IRQ();
  
#else /* not OPTIMIZED_SPI */
  struct timer t;

  Timer_Set(&t, CLOCK_SECOND/10);

#ifdef PRINT_CSV_FORMAT
  print_csv_time();
  for (int i=0; i<n_bytes1; i++) {
    PRINT_CSV(" %02x", ((uint8_t *)data1)[i]);
	 }
  for (int i=0; i<n_bytes2; i++) {
    PRINT_CSV(" %02x", ((uint8_t *)data2)[i]);
	 }
  PRINT_CSV("\n");
#endif

  while(1){
    if(BlueNRG_SPI_Write(&SpiHandle, (uint8_t *)data1,(uint8_t *)data2, n_bytes1, n_bytes2)==0) break;
    if(Timer_Expired(&t)){
      break;
    }
  }
#endif /* OPTIMIZED_SPI */
}

/**
 * @brief  Initializes the SPI communication with the BlueNRG
 *         Expansion Board.
 * @param  None
 * @retval None
 */
void BNRG_SPI_Init(void)
{
  SpiHandle.Instance = BNRG_SPI_INSTANCE;
  SpiHandle.Init.Mode = BNRG_SPI_MODE;
  SpiHandle.Init.Direction = BNRG_SPI_DIRECTION;
  SpiHandle.Init.DataSize = BNRG_SPI_DATASIZE;
  SpiHandle.Init.CLKPolarity = BNRG_SPI_CLKPOLARITY;
  SpiHandle.Init.CLKPhase = BNRG_SPI_CLKPHASE;
  SpiHandle.Init.NSS = BNRG_SPI_NSS;
  SpiHandle.Init.FirstBit = BNRG_SPI_FIRSTBIT;
  SpiHandle.Init.TIMode = BNRG_SPI_TIMODE;
  SpiHandle.Init.CRCPolynomial = BNRG_SPI_CRCPOLYNOMIAL;
  SpiHandle.Init.BaudRatePrescaler = BNRG_SPI_BAUDRATEPRESCALER;
  SpiHandle.Init.CRCCalculation = BNRG_SPI_CRCCALCULATION;
  
  HAL_SPI_Init(&SpiHandle);
	
#ifdef OPTIMIZED_SPI /* used by the server (L0 and F4, not L4) for the throughput test */
  /* Added HAP to enable SPI since Optimized SPI Transmit, Receive and Transmit/Receive APIs are 
     used for BlueNRG, BlueNRG-MS SPI communication in order to get the best performance in terms of 
     BLE throughput */
  __HAL_SPI_ENABLE(&SpiHandle);
#endif
}

/**
 * @brief  Resets the BlueNRG.
 * @param  None
 * @retval None
 */
void BlueNRG_RST(void)
{
  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_RESET);
  HAL_Delay(5);
  HAL_GPIO_WritePin(BNRG_SPI_RESET_PORT, BNRG_SPI_RESET_PIN, GPIO_PIN_SET);
  HAL_Delay(5);
}

/**
 * @brief  Reports if the BlueNRG has data for the host micro.
 * @param  None
 * @retval 1 if data are present, 0 otherwise
 */
// FIXME: find a better way to handle this return value (bool type? TRUE and FALSE)
uint8_t BlueNRG_DataPresent(void)
{
  if (HAL_GPIO_ReadPin(BNRG_SPI_EXTI_PORT, BNRG_SPI_EXTI_PIN) == GPIO_PIN_SET)
      return 1;
  else  
      return 0;
} /* end BlueNRG_DataPresent() */

/**
 * @brief  Activate internal bootloader using pin.
 * @param  None
 * @retval None
 */
void BlueNRG_HW_Bootloader(void)
{
  Disable_SPI_IRQ();
  set_irq_as_output();
  BlueNRG_RST();
  set_irq_as_input();
  Enable_SPI_IRQ();
}

/**
 * @brief  Reads from BlueNRG SPI buffer and store data into local buffer.
 * @param  hspi     : SPI handle
 * @param  buffer   : Buffer where data from SPI are stored
 * @param  buff_size: Buffer size
 * @retval int32_t  : Number of read bytes
 */
int32_t BlueNRG_SPI_Read_All(SPI_HandleTypeDef *hspi, uint8_t *buffer,
                             uint8_t buff_size)
{
#ifdef OPTIMIZED_SPI /* used by the server (L0 and F4, not L4) for the throughput test */
  uint16_t byte_count;
  uint8_t len = 0;
  
  const uint8_t header_master[5] = {0x0b, 0x00, 0x00, 0x00, 0x00};
  uint8_t header_slave[5];
  
  __disable_irq();
  
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_RESET);
  
  /* Read the header */
  HAL_SPI_TransmitReceive_Opt(hspi, header_master, header_slave, HEADER_SIZE);
  
  if (header_slave[0] == 0x02) {
    // device is ready
    
    byte_count = (header_slave[4]<<8)|header_slave[3];
    
    if (byte_count > 0) {
      
      // avoid to read more data that size of the buffer
      if (byte_count > buff_size)
        byte_count = buff_size;
      
      HAL_SPI_Receive_Opt(hspi, buffer, byte_count);
      
      len = byte_count;
    }    
  }
  
  // Release CS line.
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);
  
  __enable_irq();
  
#ifdef PRINT_CSV_FORMAT
  if (len > 0) {
    print_csv_time();
    for (int i=0; i<len; i++) {
      PRINT_CSV(" %02x", buffer[i]);
    }
    PRINT_CSV("\n");
  }
#endif  
  
  return len;
	
#else /* not OPTIMIZED_SPI */
  uint16_t byte_count;
  uint8_t len = 0;
  uint8_t char_ff = 0xff;
  volatile uint8_t read_char;

  uint8_t header_master[HEADER_SIZE] = {0x0b, 0x00, 0x00, 0x00, 0x00};
  uint8_t header_slave[HEADER_SIZE];

  /* CS reset */
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_RESET);

  /* Read the header */  
  HAL_SPI_TransmitReceive(hspi, header_master, header_slave, HEADER_SIZE, TIMEOUT_DURATION);
  	
  if (header_slave[0] == 0x02) {
    /* device is ready */
    byte_count = (header_slave[4]<<8)|header_slave[3];
  
    if (byte_count > 0) {
  
      /* avoid to read more data that size of the buffer */
      if (byte_count > buff_size){
        byte_count = buff_size;
      }
  
      for (len = 0; len < byte_count; len++){                                               
        HAL_SPI_TransmitReceive(hspi, &char_ff, (uint8_t*)&read_char, 1, TIMEOUT_DURATION); 
        buffer[len] = read_char;                                                            
      }                                                                                     
      
    }    
  }
  /* Release CS line */
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);
  
  // Add a small delay to give time to the BlueNRG to set the IRQ pin low
  // to avoid a useless SPI read at the end of the transaction
  for(volatile int i = 0; i < 2; i++)__NOP();
  
#ifdef PRINT_CSV_FORMAT
  if (len > 0) {
    print_csv_time();
    for (int i=0; i<len; i++) {
      PRINT_CSV(" %02x", buffer[i]);
    }
    PRINT_CSV("\n");
  }
#endif /* OPTIMIZED_SPI */
  
  return len;  
#endif
}

/**
 * @brief  Writes data from local buffer to SPI.
 * @param  hspi     : SPI handle
 * @param  data1    : First data buffer to be written
 * @param  data2    : Second data buffer to be written
 * @param  Nb_bytes1: Size of first data buffer to be written
 * @param  Nb_bytes2: Size of second data buffer to be written
 * @retval Number of read bytes
 */
int32_t BlueNRG_SPI_Write(SPI_HandleTypeDef *hspi, uint8_t* data1,
                          uint8_t* data2, uint8_t Nb_bytes1, uint8_t Nb_bytes2)
{  
#ifdef OPTIMIZED_SPI /* used by the server (L0 and F4, not L4) for the throughput test */
  int16_t result = 0;
  uint16_t tx_bytes;
  uint8_t rx_bytes;
  
  const uint8_t header_master[5] = {0x0a, 0x00, 0x00, 0x00, 0x00};
  uint8_t header_slave[5]  = {0x00};
  
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_RESET);
  
  HAL_SPI_TransmitReceive_Opt(hspi, header_master, header_slave, HEADER_SIZE);
  
  if(header_slave[0] != 0x02){
    result = -1;
    goto failed; // BlueNRG not awake.
  }
  
  rx_bytes = header_slave[1];
  
  if(rx_bytes < Nb_bytes1){
    result = -2;
    goto failed; // BlueNRG .      
  }
  
  HAL_SPI_Transmit_Opt(hspi, data1, Nb_bytes1);
  
  rx_bytes -= Nb_bytes1;
  
  if(Nb_bytes2 > rx_bytes){
    tx_bytes = rx_bytes;
  }
  else{
    tx_bytes = Nb_bytes2;
  }
  
  HAL_SPI_Transmit_Opt(hspi, data2, tx_bytes);
  
  result = tx_bytes;
  
failed:
  
  // Release CS line
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);
  
  return result;
  
#else /* not OPTIMIZED_SPI */
  int32_t result = 0;  
  int32_t spi_fix_enabled = 0;
  
#ifdef ENABLE_SPI_FIX
  spi_fix_enabled = 1;
#endif //ENABLE_SPI_FIX
  
  unsigned char header_master[HEADER_SIZE] = {0x0a, 0x00, 0x00, 0x00, 0x00};
  unsigned char header_slave[HEADER_SIZE]  = {0xaa, 0x00, 0x00, 0x00, 0x00};
  
  unsigned char read_char_buf[MAX_BUFFER_SIZE];

  Disable_SPI_IRQ(); 

  /*
   If the SPI_FIX is enabled the IRQ is set in Output mode, then it is pulled
   high and, after a delay of at least 112us, the CS line is asserted and the
   header transmit/receive operations are started.
   After these transmit/receive operations the IRQ is reset in input mode.
 */
  if (spi_fix_enabled) {
    set_irq_as_output();

    /* Assert CS line after at least 112us */
    us150Delay();
  }

  /* CS reset */
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_RESET);

  /* Exchange header */  
  HAL_SPI_TransmitReceive(hspi, header_master, header_slave, HEADER_SIZE, TIMEOUT_DURATION);
  
  if (spi_fix_enabled) {
    set_irq_as_input();
  }

  if (header_slave[0] == 0x02) {
    /* SPI is ready */
    if (header_slave[1] >= (Nb_bytes1+Nb_bytes2)) {
  
      /*  Buffer is big enough */
      if (Nb_bytes1 > 0) {
        HAL_SPI_TransmitReceive(hspi, data1, read_char_buf, Nb_bytes1, TIMEOUT_DURATION);
      }
      if (Nb_bytes2 > 0) {
        HAL_SPI_TransmitReceive(hspi, data2, read_char_buf, Nb_bytes2, TIMEOUT_DURATION);
      }

    } else {
      /* Buffer is too small */
      result = -2;
    }
  } else {
    /* SPI is not ready */
    result = -1;
  }
	
  /* Release CS line */
  HAL_GPIO_WritePin(BNRG_SPI_CS_PORT, BNRG_SPI_CS_PIN, GPIO_PIN_SET);
    
  Enable_SPI_IRQ();
    
  return result;
 
#endif	/* OPTIMIZED_SPI */

}
      
/**
 * @brief  Set in Output mode the IRQ.
 * @param  None
 * @retval None
 */
void set_irq_as_output(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* Pull IRQ high */
  GPIO_InitStructure.Pin = BNRG_SPI_IRQ_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = BNRG_SPI_IRQ_SPEED;
  GPIO_InitStructure.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStructure);
  HAL_GPIO_WritePin(BNRG_SPI_IRQ_PORT, BNRG_SPI_IRQ_PIN, GPIO_PIN_SET);
}

/**
 * @brief  Set the IRQ in input mode.
 * @param  None
 * @retval None
 */
void set_irq_as_input(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* IRQ input */  
  GPIO_InitStructure.Pin = BNRG_SPI_IRQ_PIN;
  GPIO_InitStructure.Mode = BNRG_SPI_IRQ_MODE;
  GPIO_InitStructure.Pull = GPIO_PULLDOWN;
  GPIO_InitStructure.Speed = BNRG_SPI_IRQ_SPEED;
  GPIO_InitStructure.Alternate = BNRG_SPI_IRQ_ALTERNATE;    
  HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pull = BNRG_SPI_IRQ_PULL;
  HAL_GPIO_Init(BNRG_SPI_IRQ_PORT, &GPIO_InitStructure);
}

/**
 * @brief  Utility function for delay
 * @param  None
 * @retval None
 * NOTE: TODO: implement with clock-independent function.
 */

static void us150Delay(void)
{
#if SYSCLK_FREQ == 4000000
  for(volatile int i = 0; i < 35; i++)__NOP();
#elif SYSCLK_FREQ == 32000000
  for(volatile int i = 0; i < 420; i++)__NOP();
#elif SYSCLK_FREQ == 80000000
  for(volatile int i = 0; i < 1072; i++)__NOP();
#elif SYSCLK_FREQ == 84000000
  for(volatile int i = 0; i < 1125; i++)__NOP();
#else
#error Implement delay function.
#endif    
}

/**
 * @brief  Enable SPI IRQ.
 * @param  None
 * @retval None
 */
void Enable_SPI_IRQ(void)
{
  HAL_NVIC_EnableIRQ(BNRG_SPI_EXTI_IRQn);  
}

/**
 * @brief  Disable SPI IRQ.
 * @param  None
 * @retval None
 */
void Disable_SPI_IRQ(void)
{ 
  HAL_NVIC_DisableIRQ(BNRG_SPI_EXTI_IRQn);
}

/**
 * @brief  Clear Pending SPI IRQ.
 * @param  None
 * @retval None
 */
void Clear_SPI_IRQ(void)
{
  HAL_NVIC_ClearPendingIRQ(BNRG_SPI_EXTI_IRQn);
}

/**
 * @brief  Clear EXTI (External Interrupt) line for SPI IRQ.
 * @param  None
 * @retval None
 */
void Clear_SPI_EXTI_Flag(void)
{  
  __HAL_GPIO_EXTI_CLEAR_IT(BNRG_SPI_EXTI_PIN);  
}

#ifdef OPTIMIZED_SPI
/* used by the server (L0 and F4, not L4) for the throughput test */
static void SPI_I2S_SendData(SPI_HandleTypeDef *hspi, uint8_t data)
{
  hspi->Instance->DR = data;
}

static  uint8_t SPI_I2S_ReceiveData(SPI_HandleTypeDef *hspi)
{
  return hspi->Instance->DR;
}

/**
  * @brief  Transmit and Receive an amount of data in blocking mode 
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for SPI module.
  * @param  pTxData: pointer to transmission data buffer
  * @param  pRxData: pointer to reception data buffer to be
  * @param  Size: amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_TransmitReceive_Opt(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t *pRxData, uint8_t Size)
{
  uint8_t i;
  
  for (i = 0; i < Size; i++) {
    SPI_I2S_SendData(hspi, *pTxData++); 
    while(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE) == RESET);
    while(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE) == RESET);
    *pRxData++ = SPI_I2S_ReceiveData(hspi);
  }
  
  return HAL_OK;
}

/**
  * @brief  Transmit an amount of data in blocking mode (optimized version)
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for SPI module.
  * @param  pData: pointer to data buffer
  * @param  Size: amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Transmit_Opt(SPI_HandleTypeDef *hspi, const uint8_t *pTxData, uint8_t Size)
{
  uint8_t i;
  
  for (i = 0; i < Size; i++) {
    SPI_I2S_SendData(hspi, *pTxData++); 
    while(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE) == RESET);
    while(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE) == RESET);
    SPI_I2S_ReceiveData(hspi);
  }
  
  return HAL_OK;
}

/**
  * @brief  Receive an amount of data in blocking mode (optimized version)
  * @param  hspi: pointer to a SPI_HandleTypeDef structure that contains
  *                the configuration information for SPI module.
  * @param  pData: pointer to data buffer
  * @param  Size: amount of data to be sent
  * @retval HAL status
  */
HAL_StatusTypeDef HAL_SPI_Receive_Opt(SPI_HandleTypeDef *hspi, uint8_t *pRxData, uint8_t Size)
{
  uint8_t i;
  
  for (i = 0; i < Size; i++) {
    SPI_I2S_SendData(hspi, 0xFF); 
    while(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_TXE) == RESET);
    while(__HAL_SPI_GET_FLAG(hspi, SPI_FLAG_RXNE) == RESET);
    *pRxData++ = SPI_I2S_ReceiveData(hspi);
  }
  
  return HAL_OK;
}
#endif /* OPTIMIZED_SPI */

/**
* @}
*/

/**
 * @}
 */

/**
 * @}
 */
   
/**
 * @}
 */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/