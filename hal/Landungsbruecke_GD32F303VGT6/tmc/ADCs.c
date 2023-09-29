#include <math.h>
#include "hal/HAL.h"
#include "hal/ADCs.h"

#define ADC1_DR_ADDRESS  ((uint32_t)0x4001204C)

static void init(void);
static void deInit(void);

ADCTypeDef ADCs =
{
	.AIN0    = &ADCValue[0],
	.AIN1    = &ADCValue[1],
	.AIN2    = &ADCValue[2],
	.DIO4    = &ADCValue[3],
	.DIO5    = &ADCValue[4],
	.VM      = &ADCValue[5],
	.AIN_EXT = &ADCValue[6],
	.init    = init,
	.deInit  = deInit,
};

void adc0_dma_config(void)
{
    /* ADC_DMA_channel configuration */
    dma_parameter_struct dma_data_parameter;

    /* ADC_DMA_channel deinit */
    dma_deinit(DMA0, DMA_CH0);

    /* initialize DMA single data mode */
    dma_data_parameter.periph_addr = (uint32_t)(&ADC_RDATA(ADC0));
    dma_data_parameter.periph_inc = DMA_PERIPH_INCREASE_DISABLE;
    dma_data_parameter.memory_addr = (uint32_t)&ADCValue[0];
    dma_data_parameter.memory_inc = DMA_MEMORY_INCREASE_ENABLE;
    dma_data_parameter.periph_width = DMA_PERIPHERAL_WIDTH_16BIT;
    dma_data_parameter.memory_width = DMA_MEMORY_WIDTH_16BIT;
    dma_data_parameter.direction = DMA_PERIPHERAL_TO_MEMORY;
    dma_data_parameter.number = N_O_ADC_CHANNELS;
    dma_data_parameter.priority = DMA_PRIORITY_HIGH;
    dma_init(DMA0, DMA_CH0, &dma_data_parameter);

    dma_circulation_enable(DMA0, DMA_CH0);

    /* enable DMA channel */
    dma_channel_enable(DMA0, DMA_CH0);
}


void init(void)
{
	adc_deinit(ADC0);

	rcu_periph_clock_enable(RCU_DMA0);
	rcu_periph_clock_enable(RCU_ADC0);

	HAL.IOs->config->reset(&HAL.IOs->pins->AIN0);
	HAL.IOs->config->reset(&HAL.IOs->pins->AIN1);
	HAL.IOs->config->reset(&HAL.IOs->pins->AIN2);
	HAL.IOs->config->reset(&HAL.IOs->pins->ADC_VM);

	// F303 ADC0 DMA0-Channel0 //TODO yy
	adc0_dma_config();
	
	/* config ADC clock */
    rcu_adc_clock_config(RCU_CKADC_CKAPB2_DIV4);
	//adc_sync_mode_config(ADC_SYNC_MODE_INDEPENDENT);
	adc_mode_config(ADC_MODE_FREE);

	//adc_sync_delay_config(ADC_SYNC_DELAY_5CYCLE);
	adc_resolution_config(ADC0, ADC_RESOLUTION_12B);
	adc_special_function_config(ADC0, ADC_SCAN_MODE, ENABLE);
	adc_special_function_config(ADC0, ADC_CONTINUOUS_MODE, ENABLE);

	adc_external_trigger_config(ADC0, ADC_REGULAR_CHANNEL, DISABLE);

	adc_data_alignment_config(ADC0, ADC_DATAALIGN_RIGHT);
	adc_channel_length_config(ADC0, ADC_REGULAR_CHANNEL, N_O_ADC_CHANNELS);
	adc_dma_mode_enable(ADC0);

	adc_regular_channel_config(ADC0, 0, ADC_CHANNEL_14, ADC_SAMPLETIME_13POINT5);
	adc_regular_channel_config(ADC0, 1, ADC_CHANNEL_15, ADC_SAMPLETIME_13POINT5);
	adc_regular_channel_config(ADC0, 2, ADC_CHANNEL_8, ADC_SAMPLETIME_13POINT5);
	adc_regular_channel_config(ADC0, 3, ADC_CHANNEL_0, ADC_SAMPLETIME_13POINT5);
	adc_regular_channel_config(ADC0, 4, ADC_CHANNEL_1, ADC_SAMPLETIME_13POINT5);
	adc_regular_channel_config(ADC0, 5, ADC_CHANNEL_3, ADC_SAMPLETIME_13POINT5);
	adc_regular_channel_config(ADC0, 6, ADC_CHANNEL_2, ADC_SAMPLETIME_13POINT5);

	//adc_dma_request_after_last_enable(ADC0);

	adc_enable(ADC0);

	adc_calibration_enable(ADC0);

	adc_software_trigger_enable(ADC0, ADC_REGULAR_CHANNEL);
}

static void deInit(void)
{
	adc_deinit(ADC0);
}
