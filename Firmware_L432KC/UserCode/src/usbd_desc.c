/**
  ******************************************************************************
  * @file    USB_Device/HID_Standalone_BCD/Src/usbd_desc.c
  * @author  MCD Application Team
  * @brief   This file provides the USBD descriptors and string formating method.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without
  * modification, are permitted, provided that the following conditions are met:
  *
  * 1. Redistribution of source code must retain the above copyright notice,
  *    this list of conditions and the following disclaimer.
  * 2. Redistributions in binary form must reproduce the above copyright notice,
  *    this list of conditions and the following disclaimer in the documentation
  *    and/or other materials provided with the distribution.
  * 3. Neither the name of STMicroelectronics nor the names of other
  *    contributors to this software may be used to endorse or promote products
  *    derived from this software without specific written permission.
  * 4. This software, including modifications and/or derivative works of this
  *    software, must execute solely and exclusively on microcontroller or
  *    microprocessor devices manufactured by or for STMicroelectronics.
  * 5. Redistribution and use of this software other than as permitted under
  *    this license is void and will automatically terminate your rights under
  *    this license.
  *
  * THIS SOFTWARE IS PROVIDED BY STMICROELECTRONICS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS, IMPLIED OR STATUTORY WARRANTIES, INCLUDING, BUT NOT
  * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
  * PARTICULAR PURPOSE AND NON-INFRINGEMENT OF THIRD PARTY INTELLECTUAL PROPERTY
  * RIGHTS ARE DISCLAIMED TO THE FULLEST EXTENT PERMITTED BY LAW. IN NO EVENT
  * SHALL STMICROELECTRONICS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
  * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
  * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
  * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
  * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/



#define USBD_VCP_VID     1155
#define USBD_VCP_PID     0x5740
#define USBD_VCP_LANGID_STRING     1033
#define USBD_MANUFACTURER_STRING     "STMicroelectronics"

#define USBD_VCP_PRODUCT_STRING_FS     "STM32 Virtual ComPort"
#define USBD_VCP_CONFIGURATION_STRING_FS     "CDC Config"
#define USBD_VCP_INTERFACE_STRING_FS     "CDC Interface"

#define USB_SIZ_BOS_DESC            0x0C
#define USBD_VID                      0xDC04
#define USBD_PID                      0xCD04
#define USBD_LANGID_STRING            0x409
#define USBD_MANUFACTURER_STRING      "STMicroelectronics"
#define USBD_PRODUCT_FS_STRING        "HID Joystick in FS Mode"
#define USBD_CONFIGURATION_FS_STRING  "HID Config"
#define USBD_INTERFACE_FS_STRING      "HID Interface"


#if (USBD_LPM_ENABLED == 1)
    uint8_t* USBD_USR_BOSDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);
#endif

static void Get_SerialNum (void);
static void IntToUnicode (uint32_t value, uint8_t* pbuf, uint8_t len);


uint8_t* USBD_FS_DeviceDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_FS_LangIDStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_FS_ManufacturerStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_FS_ProductStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_FS_SerialStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_FS_ConfigStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_FS_InterfaceStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);

uint8_t* USBD_HID_DeviceDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_HID_LangIDStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_HID_ManufacturerStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_HID_ProductStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_HID_SerialStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_HID_ConfigStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);
uint8_t* USBD_HID_InterfaceStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);

#ifdef USBD_SUPPORT_USER_STRING_DESC
    uint8_t* USBD_FS_USRStringDesc (USBD_SpeedTypeDef speed, uint8_t idx, uint16_t* length);
    uint8_t* USBD_HID_USRStringDesc (USBD_SpeedTypeDef speed, uint8_t idx, uint16_t* length);
#endif /* USBD_SUPPORT_USER_STRING_DESC */

#if (USBD_LPM_ENABLED == 1)
    uint8_t* USBD_FS_USR_BOSDescriptor (USBD_SpeedTypeDef speed, uint16_t* length);
#endif /* (USBD_LPM_ENABLED == 1) */

USBD_DescriptorsTypeDef FS_Desc =
{
    USBD_FS_DeviceDescriptor
    , USBD_FS_LangIDStrDescriptor
    , USBD_FS_ManufacturerStrDescriptor
    , USBD_FS_ProductStrDescriptor
    , USBD_FS_SerialStrDescriptor
    , USBD_FS_ConfigStrDescriptor
    , USBD_FS_InterfaceStrDescriptor
    #if (USBD_LPM_ENABLED == 1)
    , USBD_FS_USR_BOSDescriptor
    #endif /* (USBD_LPM_ENABLED == 1) */
};

USBD_DescriptorsTypeDef HID_Desc =
{
    USBD_HID_DeviceDescriptor,
    USBD_HID_LangIDStrDescriptor,
    USBD_HID_ManufacturerStrDescriptor,
    USBD_HID_ProductStrDescriptor,
    USBD_HID_SerialStrDescriptor,
    USBD_HID_ConfigStrDescriptor,
    USBD_HID_InterfaceStrDescriptor,
    #if (USBD_LPM_ENABLED == 1)
    USBD_USR_BOSDescriptor,
    #endif
};

#if defined ( __ICCARM__ ) /* IAR Compiler */
    #pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
/* USB Standard Device Descriptor */
__ALIGN_BEGIN uint8_t USBD_FS_DeviceDesc[USB_LEN_DEV_DESC] __ALIGN_END =
{
    0x12,                       /*bLength */
    USB_DESC_TYPE_DEVICE,       /*bDescriptorType*/
    #if (USBD_LPM_ENABLED == 1)
    0x01,                       /*bcdUSB */ /* changed to USB version 2.01
                                             in order to support LPM L1 suspend
                                             resume test of USBCV3.0*/
    #else
    0x00,                       /*bcdUSB */
    #endif /* (USBD_LPM_ENABLED == 1) */
    0x02,
    0x02,                       /*bDeviceClass*/
    0x02,                       /*bDeviceSubClass*/
    0x00,                       /*bDeviceProtocol*/
    USB_MAX_EP0_SIZE,           /*bMaxPacketSize*/
    LOBYTE (USBD_VCP_VID),          /*idVendor*/
    HIBYTE (USBD_VCP_VID),          /*idVendor*/
    LOBYTE (USBD_VCP_PID),       /*idProduct*/
    HIBYTE (USBD_VCP_PID),       /*idProduct*/
    0x00,                       /*bcdDevice rel. 2.00*/
    0x02,
    USBD_IDX_MFC_STR,           /*Index of manufacturer  string*/
    USBD_IDX_PRODUCT_STR,       /*Index of product string*/
    USBD_IDX_SERIAL_STR,        /*Index of serial number string*/
    USBD_MAX_NUM_CONFIGURATION  /*bNumConfigurations*/
};

const uint8_t USBD_DeviceDesc[USB_LEN_DEV_DESC] =
{
    0x12,                       /* bLength */
    USB_DESC_TYPE_DEVICE,       /* bDescriptorType */
    #if (USBD_LPM_ENABLED == 1)
    0x01,                       /*bcdUSB */     /* changed to USB version 2.01
                                                 in order to support LPM L1 suspend
                                                resume test of USBCV3.0*/
    #else
    0x00,                       /* bcdUSB */
    #endif
    0x02,
    0x00,                       /* bDeviceClass */
    0x00,                       /* bDeviceSubClass */
    0x00,                       /* bDeviceProtocol */
    USB_MAX_EP0_SIZE,           /* bMaxPacketSize */
    LOBYTE (USBD_VID),          /* idVendor */
    HIBYTE (USBD_VID),          /* idVendor */
    LOBYTE (USBD_PID),          /* idVendor */
    HIBYTE (USBD_PID),          /* idVendor */
    0x00,                       /* bcdDevice rel. 2.00 */
    0x02,
    USBD_IDX_MFC_STR,           /* Index of manufacturer string */
    USBD_IDX_PRODUCT_STR,       /* Index of product string */
    USBD_IDX_SERIAL_STR,        /* Index of serial number string */
    USBD_MAX_NUM_CONFIGURATION  /* bNumConfigurations */
}; /* USB_DeviceDescriptor */

/* USB_DeviceDescriptor */
/* BOS descriptor */
#if (USBD_LPM_ENABLED == 1)
#if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
__ALIGN_BEGIN uint8_t USBD_FS_BOSDesc[USB_SIZ_BOS_DESC] __ALIGN_END =
{
    0x5,
    USB_DESC_TYPE_BOS,
    0xC,
    0x0,
    0x1,  /* 1 device capability*/
    /* device capability*/
    0x7,
    USB_DEVICE_CAPABITY_TYPE,
    0x2,
    0x2,  /* LPM capability bit set*/
    0x0,
    0x0,
    0x0
};
uint8_t USBD_BOSDesc[USB_SIZ_BOS_DESC] =
{
    0x5,
    USB_DESC_TYPE_BOS,
    0xC,
    0x0,
    0x1,  /* 1 device capability */
    /* device capability*/
    0x7,
    USB_DEVICE_CAPABITY_TYPE,
    0x2,
    0x2, /*LPM capability bit set */
    0x0,
    0x0,
    0x0
};
#endif /* (USBD_LPM_ENABLED == 1) */



#if defined ( __ICCARM__ ) /* IAR Compiler */
    #pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */

/** USB lang indentifier descriptor. */
__ALIGN_BEGIN uint8_t USBD_VCP_LangIDDesc[USB_LEN_LANGID_STR_DESC] __ALIGN_END =
{
    USB_LEN_LANGID_STR_DESC,
    USB_DESC_TYPE_STRING,
    LOBYTE (USBD_VCP_LANGID_STRING),
    HIBYTE (USBD_VCP_LANGID_STRING)
};

const uint8_t USBD_LangIDDesc[USB_LEN_LANGID_STR_DESC] =
{
    USB_LEN_LANGID_STR_DESC,
    USB_DESC_TYPE_STRING,
    LOBYTE (USBD_LANGID_STRING),
    HIBYTE (USBD_LANGID_STRING),
};

#if defined ( __ICCARM__ ) /* IAR Compiler */
    #pragma data_alignment=4
#endif /* defined ( __ICCARM__ ) */
/* Internal string descriptor. */
__ALIGN_BEGIN uint8_t USBD_StrDesc[USBD_MAX_STR_DESC_SIZ] __ALIGN_END;

#if defined ( __ICCARM__ ) /*!< IAR Compiler */
    #pragma data_alignment=4
#endif
__ALIGN_BEGIN uint8_t USBD_StringSerial[USB_SIZ_STRING_SERIAL] __ALIGN_END =
{
    USB_SIZ_STRING_SERIAL,
    USB_DESC_TYPE_STRING,
};
/**
  * @brief  Returns the device descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t* USBD_FS_DeviceDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    *length = sizeof (USBD_FS_DeviceDesc);
    return USBD_FS_DeviceDesc;
}

uint8_t* USBD_HID_DeviceDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    *length = sizeof (USBD_DeviceDesc);
    return (uint8_t*) USBD_DeviceDesc;
}

/**
  * @brief  Return the LangID string descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t* USBD_FS_LangIDStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    *length = sizeof (USBD_VCP_LangIDDesc);
    return USBD_VCP_LangIDDesc;
}
/**
  * @brief  Returns the LangID string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t* USBD_HID_LangIDStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    *length = sizeof (USBD_LangIDDesc);
    return (uint8_t*) USBD_LangIDDesc;
}

/**
  * @brief  Return the product string descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t* USBD_FS_ProductStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    if (speed == 0)
        {
            USBD_GetString ( (uint8_t*) USBD_VCP_PRODUCT_STRING_FS, USBD_StrDesc, length);
        }
    else
        {
            USBD_GetString ( (uint8_t*) USBD_VCP_PRODUCT_STRING_FS, USBD_StrDesc, length);
        }
        
    return USBD_StrDesc;
}
/**
  * @brief  Returns the product string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t* USBD_HID_ProductStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    USBD_GetString ( (uint8_t*) USBD_PRODUCT_FS_STRING, USBD_StrDesc, length);
    return USBD_StrDesc;
}

/**
  * @brief  Return the manufacturer string descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t* USBD_FS_ManufacturerStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    USBD_GetString ( (uint8_t*) USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
    return USBD_StrDesc;
}

/**
  * @brief  Returns the manufacturer string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t* USBD_HID_ManufacturerStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    USBD_GetString ( (uint8_t*) USBD_MANUFACTURER_STRING, USBD_StrDesc, length);
    return USBD_StrDesc;
}

/**
  * @brief  Return the serial number string descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t* USBD_FS_SerialStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    *length = USB_SIZ_STRING_SERIAL;
    /* Update the serial number string descriptor with the data from the unique
     * ID */
    Get_SerialNum();
    return (uint8_t*) USBD_StringSerial;
}
/**
  * @brief  Returns the serial number string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t* USBD_HID_SerialStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    *length = USB_SIZ_STRING_SERIAL;
    /* Update the serial number string descriptor with the data from the unique ID*/
    Get_SerialNum();
    return USBD_StringSerial;
}

/**
  * @brief  Return the configuration string descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t* USBD_FS_ConfigStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    if (speed == USBD_SPEED_HIGH)
        {
            USBD_GetString ( (uint8_t*) USBD_VCP_CONFIGURATION_STRING_FS, USBD_StrDesc, length);
        }
    else
        {
            USBD_GetString ( (uint8_t*) USBD_VCP_CONFIGURATION_STRING_FS, USBD_StrDesc, length);
        }
        
    return USBD_StrDesc;
}
/**
  * @brief  Returns the configuration string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t* USBD_HID_ConfigStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    USBD_GetString ( (uint8_t*) USBD_CONFIGURATION_FS_STRING, USBD_StrDesc, length);
    return USBD_StrDesc;
}

/**
  * @brief  Return the interface string descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t* USBD_FS_InterfaceStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    if (speed == 0)
        {
            USBD_GetString ( (uint8_t*) USBD_VCP_INTERFACE_STRING_FS, USBD_StrDesc, length);
        }
    else
        {
            USBD_GetString ( (uint8_t*) USBD_VCP_INTERFACE_STRING_FS, USBD_StrDesc, length);
        }
        
    return USBD_StrDesc;
}
/**
  * @brief  Returns the interface string descriptor.
  * @param  speed: Current device speed
  * @param  length: Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t* USBD_HID_InterfaceStrDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    USBD_GetString ( (uint8_t*) USBD_INTERFACE_FS_STRING, USBD_StrDesc, length);
    return USBD_StrDesc;
}

#if (USBD_LPM_ENABLED == 1)
/**
  * @brief  Return the BOS descriptor
  * @param  speed : Current device speed
  * @param  length : Pointer to data length variable
  * @retval Pointer to descriptor buffer
  */
uint8_t* USBD_FS_USR_BOSDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    *length = sizeof (USBD_FS_BOSDesc);
    return (uint8_t*) USBD_FS_BOSDesc;
}
#endif /* (USBD_LPM_ENABLED == 1) */

/**
  * @brief  Create the serial number string descriptor
  * @param  None
  * @retval None
  */
static void Get_SerialNum (void)
{
    uint32_t deviceserial0, deviceserial1, deviceserial2;
    deviceserial0 = * (uint32_t*) DEVICE_ID1;
    deviceserial1 = * (uint32_t*) DEVICE_ID2;
    deviceserial2 = * (uint32_t*) DEVICE_ID3;
    deviceserial0 += deviceserial2;
    
    if (deviceserial0 != 0)
        {
            IntToUnicode (deviceserial0, &USBD_StringSerial[2], 8);
            IntToUnicode (deviceserial1, &USBD_StringSerial[18], 4);
        }
}

#if (USBD_LPM_ENABLED == 1)

/**
  * @brief  USBD_USR_BOSDescriptor
  *         return the BOS descriptor
  * @param  speed : current device speed
  * @param  length : pointer to data length variable
  * @retval pointer to descriptor buffer
  */
uint8_t* USBD_USR_BOSDescriptor (USBD_SpeedTypeDef speed, uint16_t* length)
{
    *length = sizeof (USBD_BOSDesc);
    return (uint8_t*) USBD_BOSDesc;
}
#endif

/**
  * @brief  Convert Hex 32Bits value into char
  * @param  value: value to convert
  * @param  pbuf: pointer to the buffer
  * @param  len: buffer length
  * @retval None
  */
static void IntToUnicode (uint32_t value, uint8_t* pbuf, uint8_t len)
{
    uint8_t idx = 0;
    
    for (idx = 0; idx < len; idx++)
        {
            if ( ( (value >> 28)) < 0xA)
                {
                    pbuf[2 * idx] = (value >> 28) + '0';
                }
            else
                {
                    pbuf[2 * idx] = (value >> 28) + 'A' - 10;
                }
                
            value = value << 4;
            pbuf[2 * idx + 1] = 0;
        }
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
