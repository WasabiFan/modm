/*
 * Copyright (c) 2015-2018, Niklas Hauser
 * Copyright (c) 2017, Sascha Schade
 * Copyright (c) 2018, Antal Szabó
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_STM32_F429_DISCOVERY_HPP
#define MODM_STM32_F429_DISCOVERY_HPP

#include <modm/platform.hpp>
#include <modm/architecture/interface/clock.hpp>
#include <modm/debug/logger.hpp>

using namespace modm::platform;

/// @ingroup modm_board_disco_f429zi
#define MODM_BOARD_HAS_LOGGER

namespace Board
{
/// @ingroup modm_board_disco_f429zi
/// @{
using namespace modm::literals;

/// STM32F429 running at 168MHz from the external 8MHz crystal
struct SystemClock
{
	static constexpr uint32_t Frequency = 168_MHz;
	static constexpr uint32_t Apb1 = Frequency / 4;
	static constexpr uint32_t Apb2 = Frequency / 2;

	static constexpr uint32_t Adc = Apb2;

	static constexpr uint32_t Spi1 = Apb2;
	static constexpr uint32_t Spi2 = Apb1;
	static constexpr uint32_t Spi3 = Apb1;
	static constexpr uint32_t Spi4 = Apb2;
	static constexpr uint32_t Spi5 = Apb2;
	static constexpr uint32_t Spi6 = Apb2;

	static constexpr uint32_t Usart1 = Apb2;
	static constexpr uint32_t Usart2 = Apb1;
	static constexpr uint32_t Usart3 = Apb1;
	static constexpr uint32_t Uart4  = Apb1;
	static constexpr uint32_t Uart5  = Apb1;
	static constexpr uint32_t Usart6 = Apb2;
	static constexpr uint32_t Uart7  = Apb1;
	static constexpr uint32_t Uart8  = Apb1;

	static constexpr uint32_t Can1 = Apb1;
	static constexpr uint32_t Can2 = Apb1;

	static constexpr uint32_t I2c1 = Apb1;
	static constexpr uint32_t I2c2 = Apb1;
	static constexpr uint32_t I2c3 = Apb1;
	static constexpr uint32_t I2c4 = Apb1;

	static constexpr uint32_t Apb1Timer = Apb1 * 2;
	static constexpr uint32_t Apb2Timer = Apb2 * 2;
	static constexpr uint32_t Timer1  = Apb2Timer;
	static constexpr uint32_t Timer2  = Apb1Timer;
	static constexpr uint32_t Timer3  = Apb1Timer;
	static constexpr uint32_t Timer4  = Apb1Timer;
	static constexpr uint32_t Timer5  = Apb1Timer;
	static constexpr uint32_t Timer6  = Apb1Timer;
	static constexpr uint32_t Timer7  = Apb1Timer;
	static constexpr uint32_t Timer8  = Apb2Timer;
	static constexpr uint32_t Timer10 = Apb2Timer;
	static constexpr uint32_t Timer11 = Apb2Timer;
	static constexpr uint32_t Timer12 = Apb1Timer;
	static constexpr uint32_t Timer13 = Apb1Timer;
	static constexpr uint32_t Timer14 = Apb1Timer;

	static constexpr uint32_t Usb = 48_MHz;
	static constexpr uint32_t Iwdg = Rcc::LsiFrequency;

	static bool inline
	enable()
	{
		Rcc::enableExternalCrystal(); // 8 MHz
		const Rcc::PllFactors pllFactors{
			.pllM = 4,		// 8MHz / M -> 2MHz
			.pllN = 168,	// 2MHz * N -> 336MHz
			.pllP = 2,		// 336MHz / P -> 168MHz = F_cpu
			.pllQ = 7		// 336MHz / Q ->  48MHz = F_usb
		};
		Rcc::enablePll(Rcc::PllSource::ExternalCrystal, pllFactors);
		// Required for 180 MHz clock
		Rcc::enableOverdriveMode();
		Rcc::setFlashLatency<Frequency>();
		Rcc::enableSystemClock(Rcc::SystemClockSource::Pll);
		Rcc::setApb1Prescaler(Rcc::Apb1Prescaler::Div4);
		Rcc::setApb2Prescaler(Rcc::Apb2Prescaler::Div2);
		Rcc::updateCoreFrequency<Frequency>();

		return true;
	}
};

using Button = GpioInputA0;

using LedGreen = GpioOutputG13;	// User LED (LD3)
using LedRed   = GpioOutputG14;	// User LED (LD4)

using Leds = SoftwareGpioPort< LedRed, LedGreen >;
/// @}

namespace l3g
{
/// @ingroup modm_board_disco_f429zi
/// @{
using Int1 = GpioInputA1;	// MEMS_INT1 [L3GD20_INT1]: GPXTI0
using Int2 = GpioInputA2;	// MEMS_INT2 [L3GD20_DRDY/INT2]: GPXTI1

using Cs   = GpioOutputC1;	// CS_I2C/SPI [L3GD20_CS_I2C/SPI]
using Sck  = GpioOutputF7;	// SPI5_SCK [L3GD20_SCL/SPC]
using Mosi = GpioOutputF9;	// SPI5_MOSI [L3GD20_SDA/SDI/SDO]
using Miso = GpioInputF8;	// SPI5_MISO [L3GD20_SA0/SDO]

using SpiMaster = SpiMaster5;
/// @}
}

namespace fmc
{
/// @ingroup modm_board_disco_f429zi
/// @{
using Sdcke1 = GpioOutputB5;	// SDCKE1: FMC_SDCKE1
using Sdclk  = GpioOutputG8;	// SDCLK: FMC_SDCLK
using Sdncas = GpioOutputG15;	// SDNCAS: FMC_SDNCAS
using Sdnras = GpioOutputF11;	// SDNRAS: FMC_SDNRAS
using Sdnwe  = GpioOutputC0;	// SDNWE: FMC_SDNWE

using Cs = GpioOutputB6;	// SDNE1 [SDRAM_CS]: FMC_SDNE1
using Ldqm = GpioOutputE0;	// NBL0 [SDRAM_LDQM]: FMC_NBL0
using Udqm = GpioOutputE1;	// NBL1 [SDRAM_UDQM]: FMC_NBL1

using Ba0 = GpioOutputG4;	// BA0: FMC_A14_BA0
using Ba1 = GpioOutputG5;	// BA1: FMC_A15_BA1

using A0  = GpioOutputF0;	// A0: FMC_A0
using A1  = GpioOutputF1;	// A1: FMC_A1
using A2  = GpioOutputF2;	// A2: FMC_A2
using A3  = GpioOutputF3;	// A3: FMC_A3
using A4  = GpioOutputF4;	// A4: FMC_A4
using A5  = GpioOutputF5;	// A5: FMC_A5
using A6  = GpioOutputF12;	// A6: FMC_A6
using A7  = GpioOutputF13;	// A7: FMC_A7
using A8  = GpioOutputF14;	// A8: FMC_A8
using A9  = GpioOutputF15;	// A9: FMC_A9
using A10 = GpioOutputG0;	// A10: FMC_A10
using A11 = GpioOutputG1;	// A11: FMC_A11

using D0 = GpioOutputD14;	// D0: FMC_D0_DA0
using D1 = GpioOutputD15;	// D1: FMC_D1_DA1
using D2 = GpioOutputD0;	// D2: FMC_D2_DA2
using D3 = GpioOutputD1;	// D3: FMC_D3_DA3
using D4 = GpioOutputE7;	// D4: FMC_D4_DA4
using D5 = GpioOutputE8;	// D5: FMC_D5_DA5
using D6 = GpioOutputE9;	// D6: FMC_D6_DA6
using D7 = GpioOutputE10;	// D7: FMC_D7_DA7
using D8 = GpioOutputE11;	// D8: FMC_D8_DA8
using D9 = GpioOutputE12;	// D9: FMC_D9_DA9
using D10 = GpioOutputE13;	// D10: FMC_D10_DA10
using D11 = GpioOutputE14;	// D11: FMC_D11_DA11
using D12 = GpioOutputE15;	// D12: FMC_D12_DA12
using D13 = GpioOutputD8;	// D13: FMC_D13_DA13
using D14 = GpioOutputD9;	// D14: FMC_D14_DA14
using D15 = GpioOutputD10;	// D15: FMC_D15_DA15
/// @}
}

namespace lcd
{
/// @ingroup modm_board_disco_f429zi
/// @{
using TouchInt = GpioInputA15;	// TP_INT1 [Touch Panel]: GPXTI15
using TouchScl = GpioA8;		// I2C3_SCL [ACP/RF_SCL]: I2C3_SCL
using TouchSda = GpioC9;		// I2C3_SDA [ACP/RF_SDA]: I2C3_SDA
using TouchRst = GpioOutputA7;	// ACP_RST: GPIO_Output

using Hsync = GpioOutputC6;		// HSYNC: LTDC_HSYNC
using Vsync = GpioOutputA4;		// VSYNC: LTDC_VSYNC

using Csx = GpioOutputC2;		// CSX
using Rdx = GpioOutputD12;		// RDX
using WrxDcx = GpioOutputD13;	// WRX_DCX
using Te = GpioInputD11;		// TE

using Enable = GpioOutputF10;	// ENABLE: LTDC_DE
using DotClk = GpioOutputG7;	// DOTCLK: LTDC_CLK

using R2 = GpioOutputC10;		// R2: LTDC_R2
using R3 = GpioOutputB0;		// R3: LTDC_R3
using R4 = GpioOutputA11;		// R4: LTDC_R4
using R5 = GpioOutputA12;		// R5: LTDC_R5
using R6 = GpioOutputB1;		// R6: LTDC_R6
using R7 = GpioOutputG6;		// R7: LTDC_R7

using G2 = GpioOutputA6;		// G2: LTDC_G2
using G3 = GpioOutputG10;		// G3: LTDC_G3
using G4 = GpioOutputB10;		// G4: LTDC_G4
using G5 = GpioOutputB11;		// G5: LTDC_G5
using G6 = GpioOutputC7;		// G6: LTDC_G6
using G7 = GpioOutputD3;		// G7: LTDC_G7

using B2 = GpioOutputD6;		// B2: LTDC_B2
using B3 = GpioOutputG11;		// B3: LTDC_B3
using B4 = GpioOutputG12;		// B4: LTDC_B4
using B5 = GpioOutputA3;		// B5: LTDC_B5
using B6 = GpioOutputB8;		// B6: LTDC_B6
using B7 = GpioOutputB9;		// B7: LTDC_B7
/// @}
}

namespace usb
{
/// @ingroup modm_board_disco_f429zi
/// @{
using Dm = GpioOutputB14;		// OTG_FS_DM: USB_OTG_HS_DM
using Dp = GpioOutputB15;		// OTG_FS_DP: USB_OTG_HS_DP
using Id = GpioOutputB12;		// OTG_FS_ID: USB_OTG_HS_ID

using Overcurrent = GpioC5;		// OTG_FS_OC [OTG_FS_OverCurrent]: GPXTI5
using Power = GpioOutputC4;		// OTG_FS_PSO [OTG_FS_PowerSwitchOn]
using Vbus = GpioB13;			// VBUS_FS: USB_OTG_HS_VBUS

using Device = UsbHs;
/// @}
}

namespace stlink
{
/// @ingroup modm_board_nucleo_h743zi
/// @{
using Tx = GpioOutputA9;
using Rx = GpioInputA10;
using Uart = BufferedUart<UsartHal1, UartTxBuffer<2048>>;
/// @}
}



/// @ingroup modm_board_disco_f429zi
/// @{
using LoggerDevice = modm::IODeviceWrapper< stlink::Uart, modm::IOBuffer::BlockIfFull >;

inline void
initialize()
{
	SystemClock::enable();
	SysTickTimer::initialize<SystemClock>();

	stlink::Uart::connect<stlink::Tx::Tx, stlink::Rx::Rx>();
	stlink::Uart::initialize<SystemClock, 115200_Bd>();

	LedGreen::setOutput(modm::Gpio::Low);
	LedRed::setOutput(modm::Gpio::Low);

	Button::setInput();
}

inline void
initializeL3g()
{
	l3g::Int1::setInput();
	l3g::Int2::setInput();
	l3g::Cs::setOutput(modm::Gpio::High);

	l3g::SpiMaster::connect<l3g::Sck::Sck, l3g::Mosi::Mosi, l3g::Miso::Miso>();
	l3g::SpiMaster::initialize<SystemClock, 10.5_MHz>();
	l3g::SpiMaster::setDataMode(l3g::SpiMaster::DataMode::Mode3);
}

inline void
initializeUsbFs(uint8_t priority=3)
{
	Rcc::enable<Peripheral::Usbotgfs>();
	usb::Device::initialize<SystemClock>(priority);
	usb::Device::connect<usb::Dm::Dm, usb::Dp::Dp, usb::Id::Id>();

	usb::Overcurrent::setInput();
	usb::Vbus::setInput();
	// Enable VBUS sense (B device) via pin PA9
	USB_OTG_HS->GCCFG &= ~USB_OTG_GCCFG_NOVBUSSENS;
	USB_OTG_HS->GCCFG |= USB_OTG_GCCFG_VBUSBSEN;
}
/// @}

}

#endif	// MODM_STM32_F429_DISCOVERY_HPP
