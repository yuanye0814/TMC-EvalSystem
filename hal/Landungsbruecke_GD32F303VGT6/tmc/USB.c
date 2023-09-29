#include "gd32f30x.h"
#include "usb/usbd_lld_int.h"
#include "usb/usbd_hw.h"
#include "usb/cdc_acm_core.h"
#include "hal/HAL.h"

#define BUFFER_SIZE 2048 // KEEP THIS SIZE AS IT MATCHES BUFFERSIZE OF usbd_cdc_core.c

// Specific functions
static void USBSendData(uint8_t *Buffer, uint32_t Size);
static uint32_t USBGetData(uint8_t *Buffer, size_t amount);
static uint8_t GetUSBCmd(uint8_t *Cmd);
static void InitUSB(void);
static void DetachUSB(void);

// Interface functions
static void init(void);
static void deInit(void);
static void tx(uint8_t ch);
static uint8_t rx(uint8_t *ch);
static void txN(uint8_t *str, unsigned char number);
static uint8_t rxN(uint8_t *str, unsigned char number);
static void clearBuffers(void);
static uint32_t bytesAvailable(void);

static usb_dev cdc_acm;
static uint8_t USBDataTxBuffer[256];
static volatile uint32_t available = 0;

// static RXTXBufferingTypeDef buffers =
// {
// 	.rx =
// 	{
// 		.read    = 0,
// 		.wrote   = 0,
// 		.buffer  = rxBuffer
// 	},

// 	.tx =
// 	{
// 		.read    = 0,
// 		.wrote   = 0,
// 		.buffer  = APP_Rx_Buffer
// 	}
// };

RXTXTypeDef USB =
	{
		.init = init,
		.deInit = deInit,
		.rx = NULL,
		.tx = NULL,
		.rxN = rxN,
		.txN = txN,
		.clearBuffers = clearBuffers,
		.baudRate = 115200,
		.bytesAvailable = bytesAvailable};

// void usb_timer_irq(void);

// void TIMER6_IRQHandler(void)
// {
// 	usb_timer_irq();
// }

void USBD_LP_CAN0_RX0_IRQHandler(void)
{
	usbd_isr();
}

static void InitUSB(void)
{
	usb_rcu_config();
	usb_gpio_config();

	// usb_timer_init();
	//  usbd_init(&cdc_acm, USB_CORE_ENUM_FS, &cdc_desc, &cdc_class);

	/* USB device configuration */
	usbd_init(&cdc_acm, &cdc_desc, &cdc_class);


	usb_nvic_config();
	/* enable the USB low priority interrupt */
	//nvic_irq_enable((uint8_t)USBD_LP_CAN0_RX0_IRQn, 7U, 0U);

	/* enable the USB low priority interrupt */
	//nvic_irq_enable((uint8_t)USBD_HP_CAN0_TX_IRQn, 8U, 0U);

	/* enabled USB pull-up */
	usbd_connect(&cdc_acm);

}

static void DetachUSB(void)
{
	usb_dev_stop(&cdc_acm);
}

/*******************************************************************
   Funktion: USBSendData()
   Parameter: Buffer: Array mit den zu sendenden Daten
			  Size: Anzahl der zu sendenden Bytes

   R�ckgabewert:  ---

   Zweck: Senden von Daten �ber USB.
********************************************************************/
static void USBSendData(uint8_t *Buffer, uint32_t Size)
{
	uint32_t i;

	for (i = 0; i < Size; i++)
		USBDataTxBuffer[i] = Buffer[i];

	//usbd_ep_send((usb_dev *)&cdc_acm, CDC_DATA_IN_EP, USBDataTxBuffer, Size);
	usbd_ep_send((usb_dev *)&cdc_acm, CDC_IN_EP, USBDataTxBuffer, Size);

}

/*******************************************************************
   Funktion: USBGetData()
   Parameter: Array in das die Daten kopiert werden (max. 64 Bytes)

   R�ckgabewert:  Anzahl der empfangenen Bytes

   Zweck: Abholen empfangener Daten von USB (maximal 64 Bytes).
********************************************************************/
static uint32_t USBGetData(uint8_t *Buffer, size_t amount)
{
	uint32_t i;
	bool flag = FALSE;
	//usb_cdc_handler *cdc = (usb_cdc_handler *)(&cdc_acm)->dev.class_data[CDC_COM_INTERFACE];
    usb_cdc_handler *cdc = (usb_cdc_handler *)(&cdc_acm)->class_data[CDC_COM_INTERFACE];

	i = 0;
	if (USBD_CONFIGURED == cdc_acm.cur_status)
	{
		if (cdc->packet_receive)
		{
			if (cdc->receive_length >= amount)
			{
				for (i = 0; i < amount; i++)
					Buffer[i] = cdc->data[i];
				flag = TRUE;
			}
			cdc->packet_receive = 0;
			//usbd_ep_recev((usb_dev *)&cdc_acm, CDC_DATA_OUT_EP, (uint8_t *)(cdc->data), USB_CDC_DATA_PACKET_SIZE);
    		usbd_ep_recev((usb_dev *)&cdc_acm, CDC_OUT_EP, (uint8_t*)(cdc->data), USB_CDC_RX_LEN);

		}
	}
	return flag;
}

/*******************************************************************
   Funktion: GetUSBCmd()
   Parameter: Cmd: Array (9 Bytes) f�r den TMCL-Befehl.

   R�ckgabewert:  TRUE bei Erfolg
				  FALSE wenn kein Befehl vorhanden

   Zweck: Abholen eines TMCL-Befehls �ber USB.
********************************************************************/
static uint8_t GetUSBCmd(uint8_t *Cmd)
{
	uint8_t flag;
	uint32_t i;
	usb_cdc_handler *cdc = (usb_cdc_handler *)(&cdc_acm)->class_data[CDC_COM_INTERFACE];

	flag = FALSE;
	if (USBD_CONFIGURED == cdc_acm.cur_status)
	{
		if (cdc->packet_receive)
		{
			if (cdc->receive_length >= 9)
			{
				for (i = 0; i < 9; i++)
					Cmd[i] = cdc->data[i];
				flag = TRUE;
			}
			cdc->packet_receive = 0;
			//usbd_ep_recev((usb_dev *)&cdc_acm, CDC_DATA_OUT_EP, (uint8_t *)(cdc->data), USB_CDC_DATA_PACKET_SIZE);
			usbd_ep_recev((usb_dev *)&cdc_acm, CDC_OUT_EP, (uint8_t *)(cdc->data), USB_CDC_RX_LEN);
		}
	}
	return flag;
}

static void init(void)
{
	//HAL.IOs->config->reset(&HAL.IOs->pins->USB_V_DM);
	//HAL.IOs->config->reset(&HAL.IOs->pins->USB_V_DP);

	InitUSB();
}

static void tx(uint8_t ch)
{
	// buffers.tx.buffer[buffers.tx.wrote] = ch;
	// buffers.tx.wrote = (buffers.tx.wrote + 1) % BUFFER_SIZE;
	//usbd_ep_send((usb_dev *)&cdc_acm, CDC_DATA_IN_EP, &ch, 1);
	usbd_ep_send((usb_dev *)&cdc_acm, CDC_IN_EP, &ch, 1);
}

static uint8_t rx(uint8_t *ch)
{
	uint8_t data = 0;
	uint8_t i = 0;
	usb_cdc_handler *cdc = (usb_cdc_handler *)(&cdc_acm)->class_data[CDC_COM_INTERFACE];

	if (USBD_CONFIGURED == cdc_acm.cur_status)
	{
		if (cdc->packet_receive)
		{
			if (cdc->receive_length > 0)
			{
				data = cdc->data[0];
				i = 1;
			}
			cdc->packet_receive--;
			//usbd_ep_recev((usb_dev *)&cdc_acm, CDC_DATA_OUT_EP, (uint8_t *)(cdc->data), USB_CDC_DATA_PACKET_SIZE);
			usbd_ep_recev((usb_dev *)&cdc_acm, CDC_OUT_EP, (uint8_t *)(cdc->data), USB_CDC_RX_LEN);
		}
	}

	return i;
}

static void txN(uint8_t *str, unsigned char number)
{
	USBSendData(str, number);
}

static uint8_t rxN(uint8_t *str, unsigned char number)
{
	return USBGetData(str, number);
}

static void clearBuffers(void)
{
	// __disable_irq();
	// available         = 0;
	// buffers.rx.read   = 0;
	// buffers.rx.wrote  = 0;

	// buffers.tx.read   = 0;
	// buffers.tx.wrote  = 0;
	// __enable_irq();
}

static uint32_t bytesAvailable(void)
{
	return available;
	usb_cdc_handler *cdc = (usb_cdc_handler *)(&cdc_acm)->class_data[CDC_COM_INTERFACE];
	return cdc->receive_length;
}

static void deInit(void)
{
}
