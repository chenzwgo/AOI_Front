/**
*  @file    IKapBoard.h
*  @brief   IKapBoard library main header file.
*  @author  iTek OptoElectronics Ltd
*  @par     Copyright (c) iTek OptoElectronics Ltd.
*           All Rights Reserved
*  @date    2018:12:04
*  @note    This file contains the defines necessary to use the IKapBoard "C" user interface.
*  @version <1.10.1>
*/

#ifndef _IKAP_BOARD_H
#define _IKAP_BOARD_H

#if defined (_MSC_VER) || defined (__BORLANDC__)
#  ifdef IKAPBOARD_EXPORTS
#    define IKAPBOARDDLL __declspec(dllexport)
#  else
#    define IKAPBOARDDLL __declspec(dllimport)
#  endif
#  define IKAPBOARD_CC _stdcall
#  define IKAPBOARD_PACKING 8
#elif defined  (__GNUC__) && defined (__linux__)
#    define IKAPBOARDDLL
#    define IKAPBOARD_CC
#    define IKAPBOARD_PACKING 8
#else
#  error Unsupported platform
#endif

#include <windows.h>

/*
Event type
*/
#define IKEvent_GrabStart				 (0x00000000)
#define IKEvent_FrameReady				 (0x00000001)
#define IKEvent_GrabStop				 (0x00000002)
#define	IKEvent_FrameLost				 (0x00000003)
#define IKEvent_TimeOut					 (0x00000004)
#define IKEvent_PixelClock               (0x00000005)
#define IKEvent_No_PixelClock            (0x00000006)
#define IKEvent_External_Trigger_Ignored (0x00000007)
#define IKEvent_CXP_Over_Current_Protection     (0x00000008)
#define IKEvent_CXP_CRC_Check_Error      (0x00000009)
#define IKEvent_CXP_Transfer_Error       (0x0000000A)
#define IKEvent_GrabLine				 (0x00100000)
#define IKEvent_GrabLineEnd				 (0x00200000)
#define IKEvent_INPUT_FALLING_EDGE       (0x00400000)
#define IKEVENT_INPUT_RISING_EDGE        (0x00800000)
#define IKEvent_Start_Of_Frame           (0x01000000)
#define IKEvent_End_Of_Frame             (0x02000000)
#define IKEvent_End_Of_Transfer          (0x04000000)

/*
User defined callback
*/
typedef void (CALLBACK* HookFnPtr)(void* pContext);

/*
Resource Type
*/
#define	IKBoardUnknown              0xFFFFFFFF
#define	IKBoardALL                  0x00000000
#define	IKBoardUSB30                0x00000001
#define	IKBoardPCIE                 0x00000002

/*
Parameter information
*/
#define IKP_IMAGE_WIDTH             (0x10000001)
#define IKP_IMAGE_HEIGHT            (0x10000002)
#define IKP_DATA_FORMAT             (0x10000003)
#define IKP_BOARD_BIT               (0x10000004)
#define IKP_TIME_OUT                (0x10000005)
#define IKP_SCAN_TYPE               (0x10000006)
#define IKP_FPGA_VERSION            (0x10000007)
#define IKP_INTERNEL_BUFFER_SIZE    (0x10000008)
#define IKP_FRAME_SIZE              (0x10000009)
#define IKP_IMAGE_TYPE              (0x1000000a)
#define IKP_FRAME_COUNT             (0x1000000b)
#define IKP_FRAME_TRANSFER_MODE     (0x1000000c)
#define IKP_FRAME_AUTO_CLEAR        (0x1000000d)
#define IKP_GRAB_MODE               (0x1000000e)
#define	IKP_FRAME_TIME_STAMP_LOW    (0x1000000f)
#define IKP_FRAME_TIME_STAMP_HIGH   (0x10000010)
#define IKP_BLOCK_TIME_STAMP_LOW    (0x10000011)
#define IKP_BLOCK_TIME_STAMP_HIGH   (0x10000012)
#define IKP_TAP_NUMBER              (0x10000013)
#define IKP_TAP_ARRANGEMENT         (0x10000014)
#define IKP_BAYER_PATTERN           (0x10000015)
#define IKP_PIXEL_CLOCK             (0x10000016)
#define IKP_DATA_VALID_ENABLE       (0x10000017)
#define IKP_CC1_SOURCE                      (0x10000018)
#define IKP_CC2_SOURCE                      (0x10000019)
#define IKP_CC3_SOURCE                      (0x1000001a)
#define IKP_CC4_SOURCE                      (0x1000001b)
#define IKP_BOARD_TRIGGER_MODE              (0x1000001c)
#define IKP_BOARD_TRIGGER_SOURCE            (0x1000001d)
#define IKP_GENERAL_INPUT1_SAMPLE_MODE      (0x1000001e)
#define IKP_GENERAL_INPUT1_PROTECT_MODE     (0x1000001f)
#define IKP_GENERAL_INPUT1_MINIMUM_INTERVAL (0x10000020)
#define IKP_GENERAL_INPUT2_SAMPLE_MODE      (0x10000021)
#define IKP_GENERAL_INPUT2_PROTECT_MODE     (0x10000022)
#define IKP_GENERAL_INPUT2_MINIMUM_INTERVAL (0x10000023)
#define IKP_SHAFT_ENCODER1_PULSE_DROP       (0x10000024)
#define IKP_SHAFT_ENCODER1_PROTECT_MODE     (0x10000025)
#define IKP_SHAFT_ENCODER1_MINIMUM_INTERVAL (0x10000026)
#define IKP_INTEGRATION_TRIGGER_SOURCE      (0x10000027)
#define IKP_INTEGRATION_TRIGGER_FREQUENCY   (0x10000028)
#define IKP_STROBE_TRIGGER_SOURCE           (0x10000029)
#define IKP_BOARD_SYNC_OUTPUT1_SOURCE       (0x1000002a)
#define IKP_BOARD_SYNC_OUTPUT2_SOURCE       (0x1000002b)
#define IKP_GENERAL_OUTPUT1_SOURCE          (0x1000002c)
#define IKP_GENERAL_OUTPUT2_SOURCE          (0x1000002d)
#define IKP_INTEGRATION_METHOD              (0x1000002e)
#define IKP_INTEGRATION_PARAM1              (0x1000002f)
#define IKP_INTEGRATION_PARAM2              (0x10000030)
#define IKP_INTEGRATION_PARAM3              (0x10000031)
#define IKP_INTEGRATION_PARAM4              (0x10000032)
#define IKP_INTEGRATION_POLARITY1           (0x10000033)
#define IKP_INTEGRATION_POLARITY2           (0x10000034)
#define IKP_STROBE_METHOD                   (0x10000035)
#define IKP_STROBE_PARAM1                   (0x10000036)
#define IKP_STROBE_PARAM2                   (0x10000037)
#define IKP_STROBE_PARAM3                   (0x10000038)
#define IKP_STROBE_PARAM4                   (0x10000039)
#define IKP_STROBE_POLARITY                 (0x1000003a)
#define IKP_GENERAL_OUTPUT1_POLARITY        (0x1000003b)
#define IKP_GENERAL_OUTPUT1_DELAY           (0x1000003c)
#define IKP_GENERAL_OUTPUT2_POLARITY        (0x1000003d)
#define IKP_GENERAL_OUTPUT2_DELAY           (0x1000003e)
#define IKP_GENERAL_INPUT1_TRIGGER_MODE     (0x1000003f)
#define IKP_GENERAL_INPUT2_TRIGGER_MODE     (0x10000040)
#define IKP_BOARD_SYNC1_TRIGGER_MODE        (0x10000041)
#define IKP_BOARD_SYNC2_TRIGGER_MODE        (0x10000042)
#define IKP_SHAFT_ENCODER1_CHANNEL          (0x10000043)
#define IKP_SHAFT_ENCODER1_MULTIPlY_FACTOR  (0x10000044)
#define IKP_PCB_VERSION                     (0x10000045)
#define IKP_LVAL_FILTER                     (0x10000046)
#define IKP_FRAME_TRANSFER_PERIOD           (0x10000047)
#define IKP_LINE_TRANSFER_PERIOD            (0x10000048)
#define IKP_FPGA_EXTERNAL_TRIGGER_TIMEOUT   (0x10000049)
#define IKP_IMAGE_OFFSET_X                  (0x10000050)
#define IKP_GENERAL_INPUT1_POLARITY         (0x10000051)
#define IKP_GENERAL_INPUT1_MIN_WIDTH        (0x10000052)   
#define IKP_GENERAL_INPUT2_POLARITY         (0x10000053)
#define IKP_GENERAL_INPUT2_MIN_WIDTH        (0x10000054)
#define IKP_CAMERA_PIXEL_CLOCK              (0x10000055)
#define IKP_PCIE_KERNAL_BLOCK_SIZE          (0x10000056)
#define IKP_SOFTWARE_TRIGGER_WIDTH          (0x10000057)
#define IKP_SOFTWARE_TRIGGER_PERIOD         (0x10000058)
#define IKP_SOFTWARE_TRIGGER_COUNT          (0x10000059)
#define IKP_SOFTWARE_TRIGGER_START          (0x10000060)
#define IKP_SOFTWARE_TRIGGER_DELAY          (0x10000061)
#define IKP_SFOTWARE_TRIGGER_POLARITY       (0x10000062)
#define IKP_GRAB_STATUS                     (0x10000063)
#define IKP_CHECK_FRAME_VALID_SIGNAL        (0x10000064)  
#define IKP_PIXEL_CLOCK_POLLING_TIME        (0x10000065)
#define IKP_SOFTWARE_TRIGGER_SYNC_MODE      (0x10000066)
#define IKP_HARDWARE_TRIGGER_GENERAL_INPUT1_DELAY          (0x10000067)
#define IKP_HARDWARE_TRIGGER_GENERAL_INPUT2_DELAY          (0x10000068)
#define IKP_IMAGE_ROI_X                     (0x10000069) 
#define IKP_SHAFT_ENCODER1_MIN_WIDTH        (0x10000070)
#define IKP_SHAFT_ENCODER1_VALID_DIRECTION  (0x10000071)
#define IKP_SHAFT_ENCODER1_REVERSE_COMPENSATION            (0x10000072)
#define IKP_FRAME_SIZE_64_LOW               (0x10000073)
#define IKP_FRAME_SIZE_64_HIGH              (0x10000074)
#define IKP_BOARD_TRIGGER_OUTTER_MODE_FRAME_COUNT          (0x10000075)
#define IKP_SHAFT_ENCODER1_QUAD_FREQUENCY_SOURCE_TYPE      (0x10000076)
#define IKP_FPGA_SERIAL_NUMBER              (0x20000001)
#define IKP_PCIE_LINK_STATE                 (0x20000002)
#define IKP_PCIE_SPEED_MISS_REQUIREMENT     (0x20000003)
#define IKP_PCI_CONFIGURATION               (0x20000004)
#define IKP_HARDWARE_TRIGGER_GENERAL_INPUT_DELAY_MODE           (0x20000005)
#define IKP_HARDWARE_TRIGGER_GENERAL_INPUT1_DELAY_IN_LINES      (0x20000006)
#define IKP_HARDWARE_TRIGGER_GENERAL_INPUT2_DELAY_IN_LINES      (0x20000007)
#define IKP_EVENT_INPUT_INTERNAL_COUNT                          (0x20000008)
#define IKP_EVENT_INPUT_GENERAL_1_COUNT                         (0x20000009)
#define IKP_EVENT_INPUT_GENERAL_2_COUNT                         (0x2000000a)
#define IKP_EVENT_INPUT_SHAFT_ENCODER_A_COUNT                   (0x2000000b)
#define IKP_EVENT_INPUT_SHAFT_ENCODER_B_COUNT                   (0x2000000c)
#define IKP_EVENT_INPUT_BOARD_SYNC_IN_1_COUNT                   (0x2000000d)
#define IKP_EVENT_INPUT_INTEGRATION_SIG_1_COUNT                 (0x2000000e)
#define IKP_EVENT_INPUT_INTEGRATION_SIG_2_COUNT                 (0x2000000f)
#define IKP_CXP_TEST_IMAGE                  (0x30000001)
#define IKP_CXP_TRIGGER_OUTPUT_SELECTOR     (0x30000002)
#define IKP_LAST_FRAME_INDEX                (0x30000003)
#define IKP_CXP_VOLTAGE_SUPPLY_STATUS       (0x30000004)
#define IKP_CXP_POWER_SWITCH                (0x30000005)
#define IKP_CXP_POWER_STATUS                (0x30000006)
#define IKP_CXP_SENSE_CURRENT               (0x30000007)
#define IKP_CXP_BUS_VOLTAGE                 (0x30000008)
#define IKP_CXP_RESET_OCP                   (0x30000009)
#define IKP_CXP_SENSE_VOLTAGE_HIGH          (0x3000000a)
#define IKP_CXP_SENSE_VOLTAGE_LOW           (0x3000000b)
#define IKP_CXP_SUPPLY_VOLTAGE_LOW          (0x3000000c)
#define IKP_CXP_SUPPLY_CURRENT_LOW          (0x3000000d)
#define IKP_CXP_FPGA_FRAME_TIMEOUT          (0x3000000e)
#define IKP_CXP_FPGA_FRAME_TIMEOUT_MULTIPLE (0x3000000f)
#define IKP_CXP_CRC_ERROR_COUNT             (0x30000010)
#define IKP_CXP_PoCXP_CHANNEL               (0x30000011)
#define IKP_CXP_PoCXP_LOCKED_VOLTAGE        (0x30000012)
#define IKP_CXP_SHAFT_ENCODER_DEBOUNCE      (0x30000013)
#define IKP_CXP_SHAFT_ENCODER_TICK_MODE     (0x30000014)
#define IKP_CXP_SHAFT_ENCODER_TICK_MAX      (0x30000015)
#define IKP_CXP_SHAFT_ENCODER_TICK_RESET    (0x30000016)
#define IKP_CXP_SHAFT_ENCODER_TICK_COUNT    (0x30000017)
#define IKP_CXP_SHAFT_ENCODER_REVERSE_MODE  (0x30000018)
#define IKP_CXP_SHAFT_ENCODER_REVERSE_MAX   (0x30000019)
#define IKP_CXP_SHAFT_ENCODER_REVERSE_RESET (0x30000020)
#define IKP_CXP_SHAFT_ENCODER_REVERSE_COUNT (0x30000021)
#define IKP_CXP_GENERAL_OUTPUT1_THRESHOLD   (0x30000022)
#define IKP_CXP_GENERAL_OUTPUT2_THRESHOLD   (0x30000023)
#define IKP_CXP_FIRMWARE_TYPE               (0x30000024)
#define IKP_CXP_TEMPERATURE                 (0x30000025)
#define IKP_CXP_GENERAL_INPUT_THRESHOLD     (0x30000026)
#define IKP_CXP_GENERAL_OUTPUT1_SOURCE_CHANNEL (0x30000027)
#define IKP_CXP_GENERAL_OUTPUT2_SOURCE_CHANNEL (0x30000028)
#define IKP_CXP_GENERAL_INPUT1_TYPE            (0x30000029)
#define IKP_CXP_GENERAL_INPUT2_TYPE            (0x3000002a)
#define IKP_CXP_TRANSFER_CHANNEL_SELECTOR      (0x3000002b)
#define IKP_CXP_CHANNEL_LOST_COUNT             (0x3000002c)
#define IKP_CXP_CHANNEL_ERROR_COUNT            (0x3000002d)
#define IKP_TRIGGER_FRAME_ACTIVE_MODE          (0x3000002e)
#define IKP_JPEG_COMPRESS_ENABLE               (0x3000002f)
#define IKP_JPEG_COMPRESS_QUALITY              (0x30000030)

/************************************************************************/
/*Data Format
/************************************************************************/
#define IKP_DATA_FORMAT_VAL_8Bit				(8)
#define IKP_DATA_FORMAT_VAL_10Bit				(10)
#define IKP_DATA_FORMAT_VAL_12Bit				(12)
#define IKP_DATA_FORMAT_VAL_14Bit				(14)
#define IKP_DATA_FORMAT_VAL_16Bit				(16)

/************************************************************************/
/*Image Type
/************************************************************************/
#define IKP_IMAGE_TYPE_VAL_MONOCHROME			(0)
#define IKP_IMAGE_TYPE_VAL_COLORFUL				(1)
#define IKP_IMAGE_TYPE_VAL_RGB                  (1)
#define IKP_IMAGE_TYPE_VAL_RGBC                 (2)
#define IKP_IMAGE_TYPE_VAL_BGR                  (3)
#define IKP_IMAGE_TYPE_VAL_BGRC                 (4)

/************************************************************************/
/*Scan Type
/************************************************************************/
#define IKP_SCAN_TYPE_VAL_LINEAR				(0)
#define IKP_SCAN_TYPE_VAL_AREA					(1)

/************************************************************************/
/*Grab Start Mode
/************************************************************************/
#define	IKP_GRAB_BLOCK  						(0)
#define	IKP_GRAB_NON_BLOCK						(1)

/************************************************************************/
/*Frame Transfer Mode
/************************************************************************/
#define	IKP_FRAME_TRANSFER_SYNCHRONOUS							(0)
#define	IKP_FRAME_TRANSFER_ASYNCHRONOUS							(1)
#define	IKP_FRAME_TRANSFER_SYNCHRONOUS_NEXT_EMPTY_WITH_PROTECT	(2)

/************************************************************************/
/*Frame Auto Clear
/************************************************************************/
#define IKP_FRAME_AUTO_CLEAR_VAL_DISABLE						(0)
#define IKP_FRAME_AUTO_CLEAR_VAL_ENABLE							(1)

/************************************************************************/
/*Bayer Pattern
/************************************************************************/
#define	IKP_BAYER_PATTERN_VAL_NULL								(0)
#define	IKP_BAYER_PATTERN_VAL_BGGR								(1)
#define IKP_BAYER_PATTERN_VAL_RGGB								(2)
#define IKP_BAYER_PATTERN_VAL_GBRG								(3)
#define IKP_BAYER_PATTERN_VAL_GRBG								(4)

/************************************************************************/
/*CC<i> Source
/************************************************************************/
#define IKP_CC_SOURCE_VAL_NOT_USE								(0)
#define IKP_CC_SOURCE_VAL_INTEGRATION_SIGNAL1					(1)
#define IKP_CC_SOURCE_VAL_INTEGRATION_SIGNAL2					(2)
#define IKP_CC_SOURCE_VAL_LOW									(3)
#define IKP_CC_SOURCE_VAL_HIGH									(4)
#define IKP_CC_SOURCE_VAL_SOFTWARE                              (5)
#define IKP_CC_SOURCE_VAL_GENERAL_INPUT1                        (6)
#define IKP_CC_SOURCE_VAL_GENERAL_INPUT2                        (7)
#define IKP_CC_SOURCE_VAL_BOARD_SYNC1                           (8)
#define IKP_CC_SOURCE_VAL_BOARD_SYNC2                           (9)
#define IKP_CC_SOURCE_VAL_STROBE                                (10)

/************************************************************************/
/*Board Trigger Mode
/************************************************************************/
#define IKP_BOARD_TRIGGER_MODE_VAL_INNER						(0)
#define IKP_BOARD_TRIGGER_MODE_VAL_OUTTER						(1)

/************************************************************************/
/*Board Trigger Source
/************************************************************************/
#define IKP_BOARD_TRIGGER_SOURCE_VAL_GENERAL_INPUT1				(0)
#define IKP_BOARD_TRIGGER_SOURCE_VAL_GENERAL_INPUT2				(1)
#define IKP_BOARD_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1				(2)
#define IKP_BOARD_TRIGGER_SOURCE_VAL_BOARD_SYNC1				(3)
#define IKP_BOARD_TRIGGER_SOURCE_VAL_BOARD_SYNC2				(4)
#define IKP_BOARD_TRIGGER_SOURCE_VAL_INNER_TRIGGER              (5)
#define IKP_BOARD_TRIGGER_SOURCE_VAL_SOFTWARE                   (6)

/************************************************************************/
/*General Input<i> Sample Mode
/************************************************************************/
#define IKP_GENERAL_INPUT_SAMPLE_MODE_VAL_ACTIVE_HIGH			(0)
#define IKP_GENERAL_INPUT_SAMPLE_MODE_VAL_ACTIVE_LOW			(1)
#define IKP_GENERAL_INPUT_SAMPLE_MODE_VAL_RISING_EDGE			(2)
#define IKP_GENERAL_INPUT_SAMPLE_MODE_VAL_FALLING_EDGE			(3)

/************************************************************************/
/*General Input<i> Protect Mode
/************************************************************************/
#define IKP_GENERAL_INPUT_PROTECT_MODE_VAL_NOT_USE				(0)
#define IKP_GENERAL_INPUT_PROTECT_MODE_VAL_DELETE				(1)
#define IKP_GENERAL_INPUT_PROTECT_MODE_VAL_MEMORIZE				(2)

/************************************************************************/
/*Shaft Encoder<i> Protect Mode
/************************************************************************/
#define IKP_SHAFT_ENCODER_PROTECT_MODE_VAL_NOT_USE				(0)
#define IKP_SHAFT_ENCODER_PROTECT_MODE_VAL_DELETE				(1)
#define IKP_SHAFT_ENCODER_PROTECT_MODE_VAL_MEMORIZE				(2)

/************************************************************************/
/*Integration Trigger Source
/************************************************************************/
#define IKP_INTEGRATION_TRIGGER_SOURCE_VAL_INTERNAL				(0)
#define IKP_INTEGRATION_TRIGGER_SOURCE_VAL_GENERAL_INPUT1		(1)
#define IKP_INTEGRATION_TRIGGER_SOURCE_VAL_GENERAL_INPUT2		(2)
#define IKP_INTEGRATION_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1		(5)
#define IKP_INTEGRATION_TRIGGER_SOURCE_VAL_BOARD_SYNC1			(7)
#define IKP_INTEGRATION_TRIGGER_SOURCE_VAL_BOARD_SYNC2			(8)
#define IKP_INTEGRATION_TRIGGER_SOURCE_VAL_SOFTWARE             (9)

/************************************************************************/
/*Strobe Trigger Source
/************************************************************************/
#define IKP_STROBE_TRIGGER_SOURCE_VAL_INTERNAL                  (0)
#define IKP_STROBE_TRIGGER_SOURCE_VAL_GENERAL_INPUT1			(1)
#define IKP_STROBE_TRIGGER_SOURCE_VAL_GENERAL_INPUT2			(2)
#define IKP_STROBE_TRIGGER_SOURCE_VAL_SHAFT_ENCODER1			(5)
#define IKP_STROBE_TRIGGER_SOURCE_VAL_BOARD_SYNC1				(7)
#define IKP_STROBE_TRIGGER_SOURCE_VAL_BOARD_SYNC2				(8)
#define IKP_STROBE_TRIGGER_SOURCE_VAL_FRAME_TRANSFER_END        (9)
#define IKP_STROBE_TRIGGER_SOURCE_VAL_SOFTWARE                  (10)

/************************************************************************/
/*Board Sync Output Source
/************************************************************************/
#define IKP_BOARD_SYNC_OUTPUT_SOURCE_VAL_NOT_USE				(0)
#define IKP_BOARD_SYNC_OUTPUT_SOURCE_VAL_INTERNAL				(1)
#define IKP_BOARD_SYNC_OUTPUT_SOURCE_VAL_GENERAL_INPUT1			(2)
#define IKP_BOARD_SYNC_OUTPUT_SOURCE_VAL_GENERAL_INPUT2			(3)
#define IKP_BOARD_SYNC_OUTPUT_SOURCE_VAL_SHAFT_ENCODER1			(4)
#define IKP_BOARD_SYNC_OUTPUT_SOURCE_VAL_INTEGRATION_SIGNAL1	(5)
#define IKP_BOARD_SYNC_OUTPUT_SOURCE_VAL_INTEGRATION_SIGNAL2	(6)
#define IKP_BOARD_SYNC_OUTPUT_SOURCE_VAL_STROBLE_SIGNAL1		(7)
#define IKP_BOARD_SYNC_OUTPUT_SOURCE_VAL_BOARD_SYNC1            (8)
#define IKP_BOARD_SYNC_OUTPUT_SOURCE_VAL_BOARD_SYNC2            (9)

/************************************************************************/
/*General Output<i> Source
/************************************************************************/
#define IKP_GENERAL_OUTPUT_SOURCE_VAL_INTERNAL					(0)
#define IKP_GENERAL_OUTPUT_SOURCE_VAL_GENERAL_INPUT1			(1)
#define IKP_GENERAL_OUTPUT_SOURCE_VAL_GENERAL_INPUT2			(2)
#define IKP_GENERAL_OUTPUT_SOURCE_VAL_SHAFT_ENCODER1			(3)
#define IKP_GENERAL_OUTPUT_SOURCE_VAL_INTEGRATION_SIGNAL1		(4)
#define IKP_GENERAL_OUTPUT_SOURCE_VAL_INTEGRATION_SIGNAL2		(5)
#define IKP_GENERAL_OUTPUT_SOURCE_VAL_STROBLE_SIGNAL1			(6)
#define IKP_GENERAL_OUTPUT_SOURCE_VAL_SOFTWARE                  (7)

/************************************************************************/
/*General Output<i> Polarity
/************************************************************************/
#define IKP_GENERAL_OUTPUT_POLARITY_VAL_SAME_TO_SOURCE			(0)
#define IKP_GENERAL_OUTPUT_POLARITY_VAL_OPPOSITE_TO_SOURCE		(1)

/************************************************************************/
/*Integration Method
/************************************************************************/
#define IKP_INTEGRATION_METHOD_VAL_1							(0)
#define IKP_INTEGRATION_METHOD_VAL_2							(1)
#define IKP_INTEGRATION_METHOD_VAL_3							(2)
#define IKP_INTEGRATION_METHOD_VAL_4							(3)
#define IKP_INTEGRATION_METHOD_VAL_5                            (4)

/************************************************************************/
/*Integration Polarity
/************************************************************************/
#define IKP_INTEGRATION_POLARITY_VAL_HIGH						(0)
#define IKP_INTEGRATION_POLARITY_VAL_LOW						(1)

/************************************************************************/
/*Strobe Method
/************************************************************************/
#define IKP_STROBE_METHOD_VAL_1									(0)
#define IKP_STROBE_METHOD_VAL_2									(1)
#define IKP_STROBE_METHOD_VAL_3									(2)
#define IKP_STROBE_METHOD_VAL_4									(3)

/************************************************************************/
/*Strobe Polarity
/************************************************************************/
#define IKP_STROBE_POLARITY_VAL_HIGH							(0)
#define IKP_STROBE_POLARITY_VAL_LOW								(1)

/*
General input trigger mode
*/
#define IKP_GENERAL_INPUT_TRIGGER_MODE_VAL_EDGE				    (0)
#define IKP_GENERAL_INPUT_TRIGGER_MODE_VAL_LEVEL                (1)

/*
Board synchronization trigger mode
*/
#define IKP_BOARD_SYNC_TRIGGER_MODE_VAL_EDGE                    (0)
#define IKP_BOARD_SYNC_TRIGGER_MODE_VAL_LEVEL                   (1)

/*
Shaft encoder channel
*/
#define IKP_SHAFT_ENCODER_CHANNEL_VAL_A                         (0)
#define IKP_SHAFT_ENCODER_CHANNLE_VAL_B                         (1)

/*
Shaft encoder mutiply factor
*/
#define IKP_SHAFT_ENCODER_MULTIPLY_FACTOR_VAL_1                 (0)
#define IKP_SHAFT_ENCODER_MULTIPLY_FACTOR_VAL_2                 (1)
#define IKP_SHAFT_ENCODER_MULTIPLY_FACTOR_VAL_4                 (2)
#define IKP_SHAFT_ENCODER_MULTIPLY_FACTOR_VAL_8                 (3)
#define IKP_SHAFT_ENCODER_MULTIPLY_FACTOR_VAL_16                (4)
#define IKP_SHAFT_ENCODER_MULTIPLY_FACTOR_VAL_32                (5)
#define IKP_SHAFT_ENCODER_MULTIPLY_FACTOR_VAL_64                (6)
#define IKP_SHAFT_ENCODER_MULTIPLY_FACTOR_VAL_128               (7)
#define IKP_SHAFT_ENCODER_MULTIPLY_FACTOR_VAL_256               (8)
#define IKP_SHAFT_ENCODER_MULTIPLY_FACTOR_VAL_512               (9)

/*
LVAL Filter Enable
*/
#define IKP_LVAL_FILTER_DISABLE                                  (0)
#define IKP_LVAL_FILTER_ENABLE	     						     (1)

/*
FPGA External Trigger Timeout
*/
#define IKP_FPGA_EXTERNAL_TRIGGER_TIMEOUT_MIN                     (8)
#define IKP_FPGA_EXTERNAL_TRIGGER_TIMEOUT_MAX                     (0xFFFFFFFF)

/*
General input polarity
*/
#define IKP_GENERAL_INPUT_POLARITY_VAL_HIGH                       (0)
#define IKP_GENERAL_INPUT_POLARITY_VAL_LOW                        (1)

/*
General input min valid width
*/
#define IKP_GENERAL_INPUT_MIN_WIDTH_VAL_MINIMUM                   (50)

/*
PCIe Kernal Block Size
*/
#define IKP_PCIE_KERNAL_BLOCK_SIZE_VAL_4M                         (1)
#define IKP_PCIE_KERNAL_BLOCK_SIZE_VAL_8M                         (2)
#define IKP_PCIE_KERNAL_BLOCK_SIZE_VAL_12M                        (3)
#define IKP_PCIE_KERNAL_BLOCK_SIZE_VAL_16M                        (4)

/*
Software trigger polarity
*/
#define IKP_SOFTWARE_TRIGGER_POLARITY_VAL_HIGH                    (0)
#define IKP_SOFTWARE_TRIGGER_POLARITY_VAL_LOW                     (1)

/*
Software trigger sync mode
*/
#define IKP_SOFTWARE_TRIGGER_SYNC_MODE_VAL_DISABLE                (0)
#define IKP_SOFTWARE_TRIGGER_SYNC_MODE_VAL_ENABLE                 (1)

/*
CoaXPress board trigger output selector
*/
#define IKP_CXP_TRIGGER_OUTPUT_NO                                 (0)
#define IKP_CXP_TRIGGER_OUTPUT_INTEGRATION_SIGNAL1                (1)
#define IKP_CXP_TRIGGER_OUTPUT_INTEGRATION_SIGNAL2                (2)

/*
 Monitor IO type
*/
#define IKP_EVENT_INPUT_INTERNAL                            (1 << 0)
#define IKP_EVENT_INPUT_GENERAL_1                           (1 << 1)
#define IKP_EVENT_INPUT_GENERAL_2                           (1 << 2)
#define IKP_EVENT_INPUT_SHAFT_ENCODER_A                     (1 << 3)
#define IKP_EVENT_INPUT_SHFAT_ENCODER_B                     (1 << 4)
#define IKP_EVENT_INPUT_BOARD_SYNC_IN_1                     (1 << 5)
#define IKP_EVENT_INPUT_INTEGRATION_SIG_1                   (1 << 6)
#define IKP_EVENT_INPUT_INTEGRATION_SIG_2                   (1 << 7)

/*
CoaXPress test image for debug usage
*/
#define IKP_CXP_TEST_IMAGE_OFF                                    (0)
#define IKP_CXP_TEST_IMAGE_1                                      (1)
#define IKP_CXP_TEST_IMAGE_2                                      (2)

/*
Shaft encoder valid direction
*/
#define IKP_SHAFT_ENCODER1_VALID_DIR_BOTH                         (0)
#define IKP_SHAFT_ENCODER1_VALID_DIR_FORWARD                      (1)
#define IKP_SHAFT_ENCODER1_VALID_DIR_BACKWARD                     (2)

/*
Shaft encoder reverse compensation
*/
#define IKP_SHAFT_ENCODER1_REVERSE_COMPENSATION_OFF               (0)
#define IKP_SHAFT_ENCODER1_REVERSE_COMPENSATION_ON                (1)

/*
Shaft encoder debounce period
*/
#define IKP_CXP_SHAFT_ENCODER1_DEBOUNCE_PERIOD_MIN                (0)
#define IKP_CXP_SHAFT_ENCODER1_DEBOUNCE_PERIOD_MAX                (65535)

/*
Shaft encoder tick mode
*/
#define IKP_CXP_SHAFT_ENCODER_TICK_MODE_FOLLOW_DIRECTION          (0)
#define IKP_CXP_SHAFT_ENCODER_TICK_MODE_ANY_DIRECTION             (1)

/*
CoaXPress firmware type
*/
#define IKP_CXP_FIRMWARE_TYPE_1_CAMERA                            (0)
#define IKP_CXP_FIRMWARE_TYPE_2_CAMERA                            (1)

/*
CoaXPress general output source channel
*/
#define IKP_CXP_GENERAL_OUTPUT_SOURCE_CHANNEL_A                   (0)
#define IKP_CXP_GENERAL_OUTPUT_SOURCE_CHANNEL_B                   (1)
#define IKP_CXP_GENERAL_OUTPUT_SOURCE_CHANNEL_C                   (2)
#define IKP_CXP_GENERAL_OUTPUT_SOURCE_CHANNEL_D                   (3)

/*
CoaXPress general input type
*/
#define IKP_CXP_GENERAL_INPUT_TYPE_DIFFERENTIAL                   (0)
#define IKP_CXP_GENERAL_INPUT_TYPE_SINGLE_END                     (1)

/*
CoaXPress uart serial port index
*/
#define IKP_CXP_UART_PORT                                         (8)

/*
 Jpeg/Tiff/Png Image Compression flag
*/
#define IKP_DEFAULT_COMPRESSION                                   (0x0000)      //! default compression
#define IKP_JPEG_QUALITYSUPERB                                    (0x0001)      //! save with superb quality (100:1)
#define IKP_JPEG_QUALITYGOOD                                      (0x0002)      //! save with good quality (75:1)
#define IKP_JPEG_QUALITYNORMAL                                    (0x0003)      //! save with normal quality (50:1)
#define IKP_JPEG_QUALITYAVERAGE                                   (0x0004)      //! save with average quality (25:1)
#define IKP_JPEG_QUALITYBAD                                       (0x0005)      //! save with bad quality (10:1)
#define IKP_TIFF_NONE                                             (0x0100)      //! save without any compression
#define IKP_TIFF_LZW                                              (0x0101)      //! save using LZW compression
#define IKP_PNG_Z_NO_COMPRESSION                                  (0x0200)      //! save without ZLib compression
#define IKP_PNG_Z_BEST_SPEED                                      (0x0201)      //! save using ZLib level 1 compression flag (default value is 6)
#define IKP_PNG_Z_DEFAULT_COMPRESSION                             (0x0202)      //! save using ZLib level 6 compression flag (default recommended value)
#define IKP_PNG_Z_BEST_COMPRESSION                                (0x0203)      //! save using ZLib level 9 compression flag (default value is 6)

/*
General output threshold
*/
#define IKP_GENERAL_OUTPUT_THRESHOLD_3V3                          (0x0)
#define IKP_GENERAL_OUTPUT_THRESHOLD_24V                          (0x1)

/*
Shaft encoder debounce value
*/
#define IKP_SHAFT_ENCODER_DEBOUNCE_MIN                            (0)
#define IKP_SHAFT_ENCODER_DEBOUNCE_MAX                            (1000 * 1000 * 1000)

/*
CXP General input threshold
*/
#define IKP_CXP_GENERAL_INPUT_THRESHOLD_5V                        (0)
#define IKP_CXP_GENERAL_INPUT_THRESHOLD_3V                        (1)

/*
General input delay mode
*/
#define IKP_HARDWARE_TRIGGER_GENERAL_INPUT_DELAY_MODE_IN_LINES    (0)
#define IKP_HARDWARE_TRIGGER_GENERAL_INPUT_DELAY_MODE_IN_US       (1)

#define IKP_HARDWARE_TRIGGER_GENERAL_INPUT_DELAY_IN_LINES_MIN     (0)
#define IKP_HARDWARE_TRIGGER_GENERAL_INPUT_DELAY_IN_LINES_MAX     (65535)

/*
Shaft encoder quad frequency source type
*/
#define IKP_SHAFT_ENCODER1_QUAD_FREQUENCY_SOURCE_TYPE_VAL_DOUBLE_CHANNEL            (0)
#define IKP_SHAFT_ENCODER1_QUAD_FREQUENCY_SOURCE_TYPE_VAL_SINGLE_CHANNEL            (1)

/*
IKP_TRIGGER_FRAME_ACTIVE_MODE frame active mode is on or off
*/
#define IKP_TRIGGER_FRAME_ACTIVE_MODE_OFF                         (0)
#define IKP_TRIGGER_FRAME_ACTIVE_MODE_ON                          (1)

/*
IKP_JPEG_COMPRESS_ENABLE
*/
#define IKP_JPEG_COMPRESS_ENABLE_OFF                              (0)
#define IKP_JPEG_COMPRESS_ENABLE_ON                               (1)

/*
IKP_JPEG_COMPRESS_QUALITY
*/
#define IKP_JPEG_COMPRESS_QUALITY_MIN                             (1)
#define IKP_JPEG_COMPRESS_QUALITY_MAX                             (100)

/*
Status information
*/
#define IK_RTN_OK							(1)
#define IK_RTN_ERROR						(0)

#define	IKStatus_Success					0x00000001

#define	IKStatus_BoardNotFound				0x00000002
#define	IKStatus_AllocMemoryFail			0x00000003
#define	IKStatus_InvalidParameter			0x00000004
#define	IKStatus_OpenBoardFail				0x00000005
#define	IKStatus_TimeOut					0x00000006
#define	IKStatus_WinError					0x00000007
#define	IKStatus_BoardNotOpen				0x00000008
#define	IKStatus_ConfigFilePathInvalid		0x00000009
#define	IKStatus_ConfigParameterInvalid		0x0000000a
#define	IKStatus_ZLP						0x0000000b
#define	IKStatus_ThreadUnsetup				0x0000000c
#define	IKStatus_ThreadExist				0x0000000d
#define	IKStatus_CameraUnsupport			0x0000000e
#define	IKStatus_XMLFileLoadFail			0x0000000f
#define	IKStatus_NodeNotExist				0x00000010
#define	IKStatus_WriteSerialFail			0x00000011
#define	IKStatus_CommandNotExist			0x00000012
#define	IKStatus_CommandCollision			0x00000013
#define	IKStatus_CommandMissRequirement		0x00000014
#define	IKStatus_CommandNotAllow			0x00000015
#define	IKStatus_CommandSyntaxError			0x00000016
#define	IKStatus_NodeTypeDismatch			0x00000017
#define	IKStatus_FeatureUnSupport			0x00000018
#define	IKStatus_CommandResultNoExit		0x00000019
#define	IKStatus_CLRegPathNotFound			0x0000001a
#define	IKStatus_CLDLLNotFound				0x0000001b
#define	IKStatus_CameraNotFound				0x0000001c
#define	IKStatus_BufferTooSmall				0x0000001d
#define	IKStatus_BaudrateNotSupport			0x0000001e
#define	IKStatus_CameraInUse				0x0000001f
#define	IKStatus_FPGA_EraseFlashFail		0x00000020
#define	IKStatus_FPGA_CheckFail				0x00000021
#define	IKStatus_BoardNotBindingCOM			0x00000022
#define	IKStatus_ReadRegFail				0x00000023
#define	IKStatus_Invalid_Mutex				0x00000024
#define IKStatus_Mutex_Locked				0x00000025
#define IKStatus_Invalid_Handle				0x00000026
#define IKStatus_Set_Info_Error				0x00000027
#define IKStatus_Grab_Pending				0x00000028
#define IKStatus_Insufficient_Resource		0x00000029
#define IKStatus_Grab_Abort					0x0000002a
#define IKStatus_Need_Reboot                0x0000002b
#define IKStatus_Need_Restart               0x0000002c
#define IKStatus_Not_Implement              0x0000002d

/**
*  @name Buffer Status
*  @brief  This struct provides general information about the buffer status.
*/
typedef struct _ikap_buffer_status{
	unsigned uFull;                  /**< 指明缓冲区是否为满 */
	unsigned uEmpty;                 /**< 指明缓冲区是否为空 */
	unsigned uTransfer;              /**< 指明缓冲区是否正在传输 */
	unsigned uOverflow;              /**< 指明缓冲区是否越界 */
	unsigned uLineNum;               /**< 对于线扫描相机，该参数指明当前传输的图像有效行数；对于面扫描相机，该参数始终为0。 */
}IKAPBUFFERSTATUS, *PIKAPBUFFERSTATUS;

typedef struct _ikap_buffer_status_ex{
	unsigned uFull;                      /**< 指明缓冲区是否为满 */
	unsigned uEmpty;                     /**< 指明缓冲区是否为空 */
	unsigned uTransfer;                  /**< 指明缓冲区是否正在传输 */
	unsigned uOverflow;                  /**< 指明缓冲区是否越界 */
	unsigned uLineNum;                   /**< 对于线扫描相机，该参数指明当前传输的图像有效行数；对于面扫描相机，该参数始终为0。 */
	unsigned long long uCompressSize;    /**< 对于压缩图像，存储当前缓冲区中有效长度.  */
	unsigned Reserved[64];
}IKAPBUFFERSTATUSEX, *PIKAPBUFFERSTATUSEX;

/**
*  @name Error Handle
*  @brief  This struct provides general information about the error code.
*/
typedef struct _ikap_error_info{
	unsigned uBoardType;            /**< 采集卡类型 */
	unsigned uBoardIndex;           /**< 采集卡索引 */
	unsigned uErrorCode;            /**< 错误代码 */
}IKAPERRORINFO, *PIKAPERRORINFO;

/**
*  @name CoaXPress Board Information
*  @brief This struct provides CoaXPress frame grabber information
*/
typedef struct _ikap_cxp_board_info{
	unsigned BoardIndex;
	unsigned MasterPort;
	unsigned SlaveCount;
	unsigned SlavePort[7];
	unsigned CameraIndex;
	char Reserved[252];
}IKAP_CXP_BOARD_INFO, *PIKAP_CXP_BOARD_INFO;

/**
*  @name Bus addressable memory
*  @brief This struct provides bus addressabel memory
*/
typedef struct _ikap_bus_memory{
    unsigned long long BusAddress;    /**< PCIez总线物理地址 */ 
    unsigned Size;                    /**< 内存大小          */ 
    void* UserPointer;                /**< 用户自定义指针    */
}IKAP_BUS_MEMORY, *PIKAP_BUS_MEMORY;

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

	/**
	*  @brief  获取当前PC连接采集卡的数量.
	*  @param[in]  resourceType : 采集卡接口类型
	*  @param[out]  resourceCount : 采集卡数量
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapGetBoardName
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapGetBoardCount(unsigned resourceType, unsigned * resourceCount);

	/**
	*  @brief  获取当前PC连接采集卡的名称.
	*  @param[in]  resourceType : 采集卡接口类型
	*  @param[in]  resourceIndex : 采集卡索引
	*  @param[out]  resourceName : 采集卡名称
	*  @param[out]  resoureceNameSize : 作为输入，该参数指明resourceName的输入长度，作为输出该参数指明resourceName的实际长度
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapGetBoardCount
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapGetBoardName(unsigned resourceType, unsigned resourceIndex, char* resourceName, unsigned * resourceNameSize);

	/**
	*  @brief  打开采集卡设备.
	*  @param[in]  resourceType : 采集卡接口类型
	*  @param[in]  resourceIndex : 采集卡索引
	*  @Return:
	*  - Handle : 采集卡设备句柄
	*
	*  @note   使用该函数可以打开CameraLink接口的采集卡设备;
	*  @see    IKapClose
	*/
	IKAPBOARDDLL HANDLE IKAPBOARD_CC
		IKapOpen(unsigned resourceType, unsigned resourceIndex);

	/**
	*  @brief  打开采集卡设备.
	*  @param[in]  resourceType : 采集卡接口类型
	*  @param[in]  resourceIndex : 采集卡索引
	*  @param[in]  info : CoaXPress采集卡信息
	*  @Return:
	*  - Handle : 采集卡设备句柄
	*
	*  @note   使用该函数可以打开CameraLink接口的采集卡设备;
	*  @see    IKapClose
	*/
	IKAPBOARDDLL HANDLE IKAPBOARD_CC
		IKapOpenCXP(unsigned resourceType, unsigned resourceIndex, IKAP_CXP_BOARD_INFO info);


	/**
	*  @brief  关闭采集卡设备.
	*  @param[in]  Handle : 采集卡句柄
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapOpen
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapClose(HANDLE hDev);

	/**
	*  @brief  导入采集卡配置文件.
	*  @param[in]  Handle : 采集卡句柄
	*  @param[in]  lpFileName : 配置文件路径
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapSaveConfigurationToFile
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapLoadConfigurationFromFile(HANDLE hDev, char* lpFileName);

	/**
	*  @brief  保存采集卡配置文件.
	*  @param[in]  Handle : 采集卡句柄
	*  @param[in]  lpFileName : 配置文件路径
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapLoadConfigurationToFile
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapSaveConfigurationToFile(HANDLE hDev, char* lpFileName);

	/**
	*  @brief  获取采集卡参数
	*  @param[in]  Handle : 采集卡句柄
	*  @param[in]  uType : 参数类型
	*  @param[out]  npValue : 参数值
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapSetInfo
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapGetInfo(HANDLE hDev, unsigned int uType, int* npValue);

	/**
	*  @brief  设置采集卡参数
	*  @param[in]  Handle : 采集卡句柄
	*  @param[in]  uType : 参数类型
	*  @param[in]  nValue : 参数值
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapGetInfo
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapSetInfo(HANDLE hDev, unsigned int uType, int nValue);   

	/**
	*  @brief  注册回调函数
	*  @param[in]  Handle : 采集卡句柄
	*  @param[in]  uEventType : 回调函数类型
	*  @param[in]  fEventFunc : 用户回调函数
	*  @param[in]  pContext : 用户上下文指针
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapUnRegisterCallback
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapRegisterCallback(HANDLE hDev, unsigned uEventType, HookFnPtr fEventFunc, void* pContext);

	/**
	*  @brief  清除回调函数
	*  @param[in]  Handle : 采集卡句柄
	*  @param[in]  uEventType : 回调函数类型
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapRegisterCallback
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapUnRegisterCallback(HANDLE hDev, unsigned int uEventType);

	/**
	*  @brief  开始连续采集
	*  @param[in]  Handle : 采集卡句柄
	*  @param[in]  nFrameCount : 采集图片数量
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapStopGrab
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapStartGrab(HANDLE hDev, int nFrameCount);

	/**
	*  @brief  停止连续采集
	*  @param[in]  Handle : 采集卡句柄
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapStartGrab
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapStopGrab(HANDLE hDev);

	/**
	*  @brief  等待连续采集的完成或采集图像超时
	*  @param[in]  Handle : 采集卡句柄
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapStartGrab
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapWaitGrab(HANDLE hDev);

	/**
	*  @brief  清空当前所有的采集状态和缓冲区状态
	*  @param[in]  Handle : 采集卡句柄
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note   该函数不会停止正在进行的采集操作，仅仅清空当前采集的缓冲区和采集状态
	*  @see    IKapStartGrab
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapClearGrab(HANDLE hDev);

	/**
	*  @brief  获取图像缓冲区地址
	*  @param[in]  Handle : 采集卡句柄
	*  @param[in]  nFrameNum : 缓冲区索引
	*  @param[out]  pAddress : 图像缓冲区地址
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapGetBufferAddress
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapGetBufferAddress(HANDLE hDev, int nFrameNum, void** pAddress);

	/**
	*  @brief  设置图像缓冲区地址
	*  @param[in]  Handle : 采集卡句柄
	*  @param[in]  nFrameNum : 缓冲区索引
	*  @param[in]  pAddress : 图像缓冲区地址
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapGetBufferAddress
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapSetBufferAddress(HANDLE hDev, int nFrameNum, void* pAddress);

    /**
    *  @brief  获取图像缓冲区物理总线映射地址
    *  @param[in]  Handle : 采集卡句柄
    *  @param[in]  nFrameNum : 缓冲区索引
    *  @param[out] pBusAddress : 物理总线地址
    *  @Return:
    *  - IK_RTN_OK
    *  - IK_RTN_ERROR
    *
    *  @note
    *  @see    IKapSetBufferBusAddress
    */
    IKAPBOARDDLL int IKAPBOARD_CC
        IKapGetBufferBusAddress(HANDLE hDev, int nFrameNum, PIKAP_BUS_MEMORY pBusAddress);

    /**
    *  @brief  设置图像缓冲区物理总线映射地址
    *  @param[in]  Handle : 采集卡句柄
    *  @param[in]  nFrameNum : 缓冲区索引
    *  @param[in] pBusAddress : 物理总线地址
    *  @Return:
    *  - IK_RTN_OK
    *  - IK_RTN_ERROR
    *
    *  @note
    *  @see    IKapGetBufferBusAddress
    */
    IKAPBOARDDLL int IKAPBOARD_CC
        IKapSetBufferBusAddress(HANDLE hDev, int nFrameNum, IKAP_BUS_MEMORY BusAddress);

	/**
	*  @brief  获取图像缓冲区状态
	*  @param[in]  Handle : 采集卡句柄
	*  @param[in]  nFrameNum : 缓冲区索引
	*  @param[out]  pIKapBufferStatus : 图像缓冲区状态
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapGetBufferAddress
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapGetBufferStatus(HANDLE hDev, int nFrameNum, PIKAPBUFFERSTATUS pIKapBufferStatus);

	/**
	*  @brief  获取更多图像缓冲区状态
	*  @param[in]  Handle : 采集卡句柄
	*  @param[in]  nFrameNum : 缓冲区索引
	*  @param[out]  pIKapBufferStatusEx : 图像缓冲区状态
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapGetBufferAddress
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapGetBufferStatusEx(HANDLE hDev, int nFrameNum, PIKAPBUFFERSTATUSEX pIKapBufferStatusEx);

	/**
	*  @brief  释放图像缓冲区
	*  @param[in]  Handle : 采集卡句柄
	*  @param[in]  nFrameNum : 缓冲区索引
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapGetBufferAddress
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapReleaseBuffer(HANDLE hDev, int nFrameNum);

	/**
	*  @brief  获取相机状态
	*  @param[in]  Handle : 采集卡句柄
	*  @param[in]  npValue : 相机状态
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see    IKapReleaseBuffer
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapGetCameraStatus(HANDLE hDev, int* npValue);

	/**
	*  @brief  获取上一次错误代码
	*  @param[in]  pIKapErrorInfo : 错误信息
	*  @param[in]  bErrorReset : 是否情况错误
	*  @Return:
	*  - void
	*
	*  @note
	*  @see
	*/
	IKAPBOARDDLL void IKAPBOARD_CC
		IKapGetLastError(PIKAPERRORINFO pIKapErrorInfo, bool bErrorReset);

	/**
	*  @brief  获取采集卡串口号
	*  @param[in]  hDev : 采集卡句柄
	*  @param[in]  npPortNumber : 串口号
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapGetSerialPort(HANDLE hDev, int* npPortNumber);

	/**
	*  @brief  获取采集帧率
	*  @param[in]  hDev : 采集卡句柄
	*  @param[in]  dpFrameRate : 采集帧率
	*  @Return:
	*  - IK_RTN_OK
	*  - IK_RTN_ERROR
	*
	*  @note
	*  @see
	*/
	IKAPBOARDDLL int IKAPBOARD_CC
		IKapGetFrameRate(HANDLE hDev, double* dpFrameRate);

    /**
    *  @brief  读取CXP串口数据
    *  @param[in]  handle : 调试串口句柄
    *  @param[in]  buffer : 缓冲区数据
    *  @param[in,out]  length : 作为输入，指示输入缓冲区的长度;作为输出，指示实际读取缓冲区的长度
    *  @Return:
    *  - Handle : CXP调试串口设备句柄
    *
    *  @note   
    *  @see    IKapClose
    */
    IKAPBOARDDLL int IKAPBOARD_CC
        IKapReadCXPUart(HANDLE hDev, char* buffer, unsigned* length);

    /**
    *  @brief  写入CXP串口数据
    *  @param[in]  handle : 调试串口句柄
    *  @param[in]  buffer : 缓冲区数据
    *  @param[in]  length : 作为输入，指示输入缓冲区的长度;
    *  @Return:
    *  - Handle : CXP调试串口设备句柄
    *
    *  @note
    *  @see    IKapClose
    */
    IKAPBOARDDLL int IKAPBOARD_CC
        IKapWriteCXPUart(HANDLE hDev, char* buffer, unsigned length);

    /**
    *  @brief  等待CXP数据有效
    *  @param[in]  handle : 调试串口句柄
    *  @param[in]  timeout : 超时时间
    *  @Return:
    *  - Handle : CXP调试串口设备句柄
    *
    *  @note
    *  @see    IKapClose
    */
    IKAPBOARDDLL int IKAPBOARD_CC
        IKapWaitCXPUart(HANDLE hDev, int timeout);

    /**
    *  @brief  保存缓冲区到指定位置
	*  @param[in]  Handle : 采集卡句柄
    *  @param[in]  nFrameNum : frame buffer index
    *  @param[in]  fileName : frame buffer file name
	*  @param[in]  nFlag : Image compression flag
    *  @Return:
    *  - IK_RTN_OK
	*  - IK_RTN_ERROR
    *
    *  @note
    *  @see    IKapLoadBuffer
    */
    IKAPBOARDDLL int IKAPBOARD_CC
		IKapSaveBuffer(HANDLE hDev, int nFrameNum, const char* fileName, int nFlag);

    /**
    *  @brief  保存缓冲区到指定位置
	*  @param[in]  Handle : 采集卡句柄
    *  @param[in]  nFrameNum : frame buffer index
    *  @param[in]  fileName :  frame buffer file name
    *  @Return:
    *  - IK_RTN_OK
    *  - IK_RTN_ERROR
    *
    *  @note
    *  @see    IKapSaveBuffer
    */
    IKAPBOARDDLL int IKAPBOARD_CC
		IKapLoadBuffer(HANDLE hDev, int nFrameNum, const char* fileName);

    /**
    *  @brief  采集卡寄存器直接写入
    *  @param[in]  Handle : 采集卡句柄
    *  @param[in]  Address : 寄存器地址
    *  @param[in]  Data : 寄存器值
    *  @Return:
    *  - IK_RTN_OK
    *  - IK_RTN_ERROR
    *
    *  @note
    *  @see    IKapSaveBuffer
    */
    IKAPBOARDDLL int IKAPBOARD_CC
        IKapWriteRegister(HANDLE hDev, unsigned addr, unsigned data);

    /**
    *  @brief  采集卡寄存器直接读取
    *  @param[in]  Handle : 采集卡句柄
    *  @param[in]  Address : 寄存器地址
    *  @param[in]  Data : 寄存器值
    *  @Return:
    *  - IK_RTN_OK
    *  - IK_RTN_ERROR
    *
    *  @note
    *  @see    IKapSaveBuffer
    */
    IKAPBOARDDLL int IKAPBOARD_CC
        IKapReadRegister(HANDLE hDev, unsigned addr, unsigned* data);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif