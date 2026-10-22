/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : İTÜ Rover Takımı - Ödev 6: UART ile GPIO Kontrolü
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h" // HAL kütüphanesi ve pin etiketlerinin bulunduğu başlık dosyası

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2; // USART2 çevresel birimini yöneten HAL yapısı

/* USER CODE BEGIN PV */
// Serial terminalden gelecek 2 karakterlik mesajı saklayacak olan 2 elemanlı bayt dizisi
uint8_t rx_buffer[2];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
// Pinleri gelen mesaja göre güncelleyen yardımcı kontrol fonksiyonu
void Process_UART_Command(uint8_t *cmd)
{
    // --- 1. PIN KONTROLÜ (Dizinin 0. indeksi -> PA5 / GPIO_OUTPUT_1) ---
    switch (cmd[0]) // Gelen mesajın ilk karakteri incelenir
    {
        case '0': // Karakter '0' ise
            HAL_GPIO_WritePin(GPIOA, GPIO_OUTPUT_1_Pin, GPIO_PIN_RESET); // 1. pini LOW (0V) seviyesine çek
            break; // Anahtardan çık

        case '1': // Karakter '1' ise
            HAL_GPIO_WritePin(GPIOA, GPIO_OUTPUT_1_Pin, GPIO_PIN_SET); // 1. pini HIGH (3.3V) seviyesine çek
            break; // Anahtardan çık

        case '2': // Karakter '2' ise
            HAL_GPIO_TogglePin(GPIOA, GPIO_OUTPUT_1_Pin); // 1. pinin mevcut dijital durumunu tersle (TOGGLE)
            break; // Anahtardan çık

        default: // '0', '1' veya '2' dışında bir karakter geldiyse
            break; // Herhangi bir işlem yapmadan geç
    }

    // --- 2. PIN KONTROLÜ (Dizinin 1. indeksi -> PA6 / GPIO_OUTPUT_2) ---
    switch (cmd[1]) // Gelen mesajın ikinci karakteri incelenir
    {
        case '0': // Karakter '0' ise
            HAL_GPIO_WritePin(GPIOA, GPIO_OUTPUT_2_Pin, GPIO_PIN_RESET); // 2. pini LOW (0V) seviyesine çek
            break; // Anahtardan çık

        case '1': // Karakter '1' ise
            HAL_GPIO_WritePin(GPIOA, GPIO_OUTPUT_2_Pin, GPIO_PIN_SET); // 2. pini HIGH (3.3V) seviyesine çek
            break; // Anahtardan çık

        case '2': // Karakter '2' ise
            HAL_GPIO_TogglePin(GPIOA, GPIO_OUTPUT_2_Pin); // 2. pinin mevcut dijital durumunu tersle (TOGGLE)
            break; // Anahtardan çık

        default: // '0', '1' veya '2' dışında bir karakter geldiyse
            break; // Herhangi bir işlem yapmadan geç
    }
}
/* USER CODE END 0 */

/**
  * @brief  Uygulama ana giriş noktası
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Tüm çevresel birimleri sıfırlar, Flash arabirimini ve Systick zamanlayıcısını başlatır */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Sistem saat hızını yapılandırır */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Konfigüre edilen çevresel birimleri (GPIO ve USART2) başlatır */
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Sonsuz döngü */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    // USART2 üzerinden tam 2 baytlık veri gelene kadar sonsuz zaman aşımı (HAL_MAX_DELAY) ile bekle
    if (HAL_UART_Receive(&huart2, rx_buffer, 2, HAL_MAX_DELAY) == HAL_OK)
    {
        // Veri başarıyla alındıysa ayrıştırma ve kontrol fonksiyonunu çağır
        Process_UART_Command(rx_buffer);
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief Sistem Saat Konfigürasyonu
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART2 Başlatma Fonksiyonu
  */
static void MX_USART2_UART_Init(void)
{
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief GPIO Başlatma Fonksiyonu
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* Port saat sinyallerini aktif et */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /* Başlangıçta pin çıkış seviyelerini LOW yap */
  HAL_GPIO_WritePin(GPIOA, GPIO_OUTPUT_1_Pin|GPIO_OUTPUT_2_Pin, GPIO_PIN_RESET);

  /* B1 buton pini konfigürasyonu */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /* GPIO_OUTPUT_1 ve GPIO_OUTPUT_2 pinlerinin konfigürasyonu */
  GPIO_InitStruct.Pin = GPIO_OUTPUT_1_Pin|GPIO_OUTPUT_2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
  * @brief Hata Durumu Fonksiyonu
  */
void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
