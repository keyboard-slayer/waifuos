#pragma once

#include <kawaii-io/traits.hpp>
#include <stdint.h>

#include "asm.hpp"

enum ComPort
{
	COM1 = 0x3F8,
	COM2 = 0x2F8,
	COM3 = 0x3E8,
	COM4 = 0x2E8
};

struct Com : public Writer
{
	uint16_t port;
	uint32_t baud;

	enum ComRegs
	{
		COM_REGS_DATA = 0,
		COM_REGS_INTERRUPT = 1,
		COM_REGS_BAUD_RATE_LOW = 0,
		COM_REGS_BAUD_RATE_HIGH = 1,
		COM_REGS_INTERRUPT_IDENTIFICATOR = 2,
		COM_REGS_FIFO_CONTROLLER = 2,
		COM_REGS_LINE_CONTROL = 3,
		COM_REGS_MODEM_CONTROL = 4,
		COM_REGS_LINE_STATUS = 5,
		COM_REGS_MODEM_STATUS = 6,
		COM_REGS_SCRATCH_REGISTER = 7,

		COM_DEFAULT_RATE = 115200
	};

	void
	write_reg(ComRegs reg, uint8_t data)
	{
		out8(port + reg, data);
	}

	uint8_t
	read_reg(ComRegs reg)
	{
		return in8(port + reg);
	}

	void
	putc(Byte c)
	{
		while ((read_reg(COM_REGS_LINE_STATUS) & 0x20) == 0)
			;
		write_reg(COM_REGS_DATA, c);
	}

	Maybe<size_t>
	puts(Bytes str)
	{
		size_t i = 0;
		for (i = 0; str[i] != '\0'; i++)
		{
			putc(str[i]);
		}

		return i;
	}

	Maybe<size_t>
	puts(char const *s)
	{
		return puts((uint8_t *) s);
	}

	constexpr Com(uint16_t _port, uint32_t _baud) : port(_port), baud(_baud)
	{
		const uint8_t div_low = (COM_DEFAULT_RATE / baud) & 0xff;
		const uint8_t div_high = (COM_DEFAULT_RATE / baud) >> 8;

		write_reg(COM_REGS_INTERRUPT, 0x00);
		write_reg(COM_REGS_LINE_CONTROL, 0x80);
		write_reg(COM_REGS_BAUD_RATE_LOW, div_low);
		write_reg(COM_REGS_BAUD_RATE_HIGH, div_high);
		write_reg(COM_REGS_LINE_CONTROL, 0x03);
		write_reg(COM_REGS_FIFO_CONTROLLER, 0xc7);
		write_reg(COM_REGS_MODEM_CONTROL, 0x0b);
	}

	constexpr Com(uint16_t _port) : Com(_port, 9600) {}
	constexpr Com() : Com(COM1, 9600) {}
};