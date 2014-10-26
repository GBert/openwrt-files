/*
 * Copyright (C) 2005-2014 Darron Broad
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name `Darron Broad' nor the names of any contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*******************************************************************************
 *
 * dsPIC33FJ128GP802 Audio DAC Function Generator for the R-PI.
 *
 * R-PI kermit config:
 *	$HOME/.kermrc:
 *	set modem type none
 *	set line /dev/ttyAMA0
 *	set carrier-watch off
 *	set speed 38400
 *	set flow none
 *	connect
 *
 * Control:
 * 	$ kermit
 *
 * Parameter entry:
 *	0..9	Hexadecimal nibble.
 *	A..F	    "	      "
 *	a..f	    "	      "
 *
 *	As each nibble is entered, the currently accumulated value is shifted
 *	left 4 bits and the new nibble added.
 *
 *	The accumulator is 16-bits wide.
 *
 * Command entry:
 *	~	The accumulated low nibble is transferred to the voice
 *		function selection byte.
 *	=	The accumulated value is transferred to the voice control
 *		word.
 *	/	The accumulated value is transferred to the voice volume
 *		word.
 *	%	The accumulated value is transferred to the voice square
 *		wave duty word.
 *	@	The accumulated value is evaluated as the voice ADSR
 *		envelope.
 *	!	Trigger the voice ADSR envelope.
 *	>	The accumulated low nibble is transferred to the voice
 *              selection byte.
 *	\	The accumulated value is transferred to the master volume
 *		word.
 *	#	The accumulated low byte is evaluated as a midi note and
 *		used to update the voice control word.
 * Misc.:
 * 	CR	CRLF
 *
 * Examples:
 * 	$ kermit
 * 	1~	Select sine waveform.
 *	2~	Select triangle waveform.
 *	3~	Select saw-tooth waveform.
 *	4~	Select square waveform.
 *	5~	Select noise.
 *	0~	Select nothing (OFF).
 * 	0200=	Set frequency to approx. 304Hz (39000 / 65536 * 0x200).
 * 	0690=	Set frequency to 1KHz.
 * 	7FFF/	Set max volume.
 *	8000%	50% duty cycle.
 *	AAAA@	Set ADSR.
 *	!	Trigger ADSR.
 *
 * 	To calculate the control word for a given frequency (Fosc), use
 * 	this formula where Fs is the sample rate of 39,000 samples/sec.
 *
 *	Control word = 65536 / Fs * Fosc
 *
 *	This will then need to be entered in hexadecimal.
 *
 *	Eg. (Fosc = 1000)
 *
 *	$ printf "%04x=\n" `echo "scale=10;65536 / 39000 * 1000" | bc`
 *	0690=
 *
 *******************************************************************************
 *
 * Pinout
 * ------
 * !MCLR                        1-----28 AVDD 3V3
 * AN0 VREF+ CN2 RA0            2     27 AVSS GND
 * AN1 VREF- CN3 RA1            3     26 AN9 RP15 CN11 PMCS1 RB15      DAC1LN
 * PGD1 AN2 C2IN- RP0 CN4 RB0   4     25 AN10 RTCC RP14 CN12 PMWR RB14 DAC1LP
 * PGC1 AN3 C2IN+ RP1 CN5 RB1   5     24 AN11 RP13 CN13 PMRD RB13      DAC1RN
 * AN4 C1IN- RP2 CN6 RB2        6     23 AN12 RP12 CN14 PMD0 RB12      DAC1RP
 * AN5 C1IN+ RP3 CN7 RB3        7     22 PGC2 TMS RP11 CN15 PMD1 RB11
 * VSS GND                      8     21 PGD2 TDI RP10 CN16 PMD2 RB10
 * OSC1 CLKI CN30 RA2           9     20 VCAP
 * OSC2 CLKO CN29 PMA0 RA3      10    19 VSS
 * SOSCI RP4 CN1 PMBE RB4       11    18 TDO SDA1 RP9 CN21 PMD3 RB9
 * SOSCO T1CK CN0 PMA1 RA4      12    17 TCK SCL1 RP8 CN22 PMD4 RB8
 * VDD 3V3                      13    16 INT0 RP7 CN23 PMD5 RB7
 * PGD3 ASDA1 RP5 CN27 PMD7 RB5 14----15 PGC3 ASCL1 RP6 CN24 PMD6 RB6
 *
 * ICSP
 * ----
 * !MCLR        PROGRAMMING ENABLE      1
 * PGC1         PRIMARY CLOCK           5
 * PGD1         PRIMARY DATA            4
 *
 * PPS
 * ---
 * RP2		TX1			6
 * RP3		RX1			7
 */

#include "dac.h"

#include "midi.h"

_FBS(RBS_NO_BOOT_RAM & BSS_NO_BOOT_CODE & BWRP_WRPROTECT_OFF)
_FSS(RSS_NO_SEC_RAM & SSS_NO_SEC_CODE & SWRP_WRPROTECT_OFF)
_FGS(GCP_OFF & GWRP_OFF)
_FOSCSEL(IESO_OFF & FNOSC_LPRCDIVN)
_FOSC(FCKSM_CSECMD & IOL1WAY_OFF & OSCIOFNC_ON & POSCMD_NONE)
_FWDT(FWDTEN_OFF & WINDIS_OFF & WDTPRE_PR128 & WDTPOST_PS32768)
_FPOR(ALTI2C_OFF & FPWRT_PWR1)
_FICD(JTAGEN_OFF & ICS_PGD1)

/* DAC FIFO */
DAC dac = {{0, 0, 0, 0}, Q15(0.5)};

/* Voices */
static VOICE voice0, voice1, voice2, voice3;
VOICE *voice[VOICE_SIZE] = {&voice0, &voice1, &voice2, &voice3};

/* Sine wave table */
const fractional sinetable[0x0100] = {
     0,   201,   402,   603,   804,  1005,  1206,  1407,
  1608,  1809,  2009,  2210,  2410,  2611,  2811,  3012,
  3212,  3412,  3612,  3811,  4011,  4210,  4410,  4609,
  4808,  5007,  5205,  5404,  5602,  5800,  5998,  6195,
  6393,  6590,  6786,  6983,  7179,  7375,  7571,  7767,
  7962,  8157,  8351,  8545,  8739,  8933,  9126,  9319,
  9512,  9704,  9896, 10087, 10278, 10469, 10659, 10849,
 11039, 11228, 11417, 11605, 11793, 11980, 12167, 12353,
 12539, 12725, 12910, 13094, 13279, 13462, 13645, 13828,
 14010, 14191, 14372, 14553, 14732, 14912, 15090, 15269,
 15446, 15623, 15800, 15976, 16151, 16325, 16499, 16673,
 16846, 17018, 17189, 17360, 17530, 17700, 17869, 18037,
 18204, 18371, 18537, 18703, 18868, 19032, 19195, 19357,
 19519, 19680, 19841, 20000, 20159, 20317, 20475, 20631,
 20787, 20942, 21096, 21250, 21403, 21554, 21705, 21856,
 22005, 22154, 22301, 22448, 22594, 22739, 22884, 23027,
 23170, 23311, 23452, 23592, 23731, 23870, 24007, 24143,
 24279, 24413, 24547, 24680, 24811, 24942, 25072, 25201,
 25329, 25456, 25582, 25708, 25832, 25955, 26077, 26198,
 26319, 26438, 26556, 26674, 26790, 26905, 27019, 27133,
 27245, 27356, 27466, 27575, 27683, 27790, 27896, 28001,
 28105, 28208, 28310, 28411, 28510, 28609, 28706, 28803,
 28898, 28992, 29085, 29177, 29268, 29358, 29447, 29534,
 29621, 29706, 29791, 29874, 29956, 30037, 30117, 30195,
 30273, 30349, 30424, 30498, 30571, 30643, 30714, 30783,
 30852, 30919, 30985, 31050, 31113, 31176, 31237, 31297,
 31356, 31414, 31470, 31526, 31580, 31633, 31685, 31736,
 31785, 31833, 31880, 31926, 31971, 32014, 32057, 32098,
 32137, 32176, 32213, 32250, 32285, 32318, 32351, 32382,
 32412, 32441, 32469, 32495, 32521, 32545, 32567, 32589,
 32609, 32628, 32646, 32663, 32678, 32692, 32705, 32717,
 32728, 32737, 32745, 32752, 32757, 32761, 32765, 32766,
};

/* Sine wave look-up */
static inline fractional
sinelookup(uint16_t a)
{
	register uint16_t x, y;

	x = a >> 6;
	y = x & 0x00FF;

	if (x < 0x0100)
		return sinetable[y];
	if (x < 0x0200)
		return sinetable[0x00FF - y];
	if (x < 0x0300)
		return -sinetable[y];
	return -sinetable[0x00FF - y];
}

/* Triangle wave table */
const fractional triangletable[0x0100] = {
     0,   128,   256,   384,   512,   640,   768,   896,
  1024,  1152,  1280,  1408,  1536,  1664,  1792,  1920,
  2048,  2176,  2304,  2432,  2560,  2688,  2816,  2944,
  3072,  3200,  3328,  3456,  3584,  3712,  3840,  3968,
  4096,  4224,  4352,  4480,  4608,  4736,  4864,  4992,
  5120,  5248,  5376,  5504,  5632,  5760,  5888,  6016,
  6144,  6272,  6400,  6528,  6656,  6784,  6912,  7040,
  7168,  7296,  7424,  7552,  7680,  7808,  7936,  8064,
  8192,  8320,  8448,  8576,  8704,  8832,  8960,  9088,
  9216,  9344,  9472,  9600,  9728,  9856,  9984, 10112,
 10240, 10368, 10496, 10624, 10752, 10880, 11008, 11136,
 11264, 11392, 11520, 11648, 11776, 11904, 12032, 12160,
 12288, 12416, 12544, 12672, 12800, 12928, 13056, 13184,
 13312, 13440, 13568, 13696, 13824, 13952, 14080, 14208,
 14336, 14464, 14592, 14720, 14848, 14976, 15104, 15232,
 15360, 15488, 15616, 15744, 15872, 16000, 16128, 16256,
 16384, 16511, 16639, 16767, 16895, 17023, 17151, 17279,
 17407, 17535, 17663, 17791, 17919, 18047, 18175, 18303,
 18431, 18559, 18687, 18815, 18943, 19071, 19199, 19327,
 19455, 19583, 19711, 19839, 19967, 20095, 20223, 20351,
 20479, 20607, 20735, 20863, 20991, 21119, 21247, 21375,
 21503, 21631, 21759, 21887, 22015, 22143, 22271, 22399,
 22527, 22655, 22783, 22911, 23039, 23167, 23295, 23423,
 23551, 23679, 23807, 23935, 24063, 24191, 24319, 24447,
 24575, 24703, 24831, 24959, 25087, 25215, 25343, 25471,
 25599, 25727, 25855, 25983, 26111, 26239, 26367, 26495,
 26623, 26751, 26879, 27007, 27135, 27263, 27391, 27519,
 27647, 27775, 27903, 28031, 28159, 28287, 28415, 28543,
 28671, 28799, 28927, 29055, 29183, 29311, 29439, 29567,
 29695, 29823, 29951, 30079, 30207, 30335, 30463, 30591,
 30719, 30847, 30975, 31103, 31231, 31359, 31487, 31615,
 31743, 31871, 31999, 32127, 32255, 32383, 32511, 32639,
};

/* Triangle wave look-up */
static inline fractional
trianglelookup(uint16_t a)
{
	register uint16_t x, y;

	x = a >> 6;
	y = x & 0x00FF;

	if (x < 0x0100)
		return triangletable[y];
	if (x < 0x0200)
		return triangletable[0x00FF - y];
	if (x < 0x0300)
		return -triangletable[y];
	return -triangletable[0x00FF - y];
}

/* Waveform function */
fractional
function(WAVEFORM *w)
{
	register uint16_t a = (w->acc >> NSHIFT);

	switch (w->func) {
	default:break;
		
	case FUNC_SINE:
		return sinelookup(a);

	case FUNC_TRIANGLE:
		return trianglelookup(a);

	case FUNC_SAW:
		return a;

	case FUNC_SQUARE:
		return (a >= w->duty) ? (INT16_MIN) : (INT16_MAX);

	case FUNC_NOISE:
		return rand() << 1;
	}
	return 0;
}

/*
 * Envelope Generation
 */

/*
 * Attack times (ms)
 *
 * [0 .. 3]    2,   8,    16,   24
 * [4 .. 7]   38,  56,    68,   80
 * [8 .. B]  100, 250,   500,  800
 * [C .. F] 1000, 3000, 5000, 8000
 */
const int32_t attack[16] = {
	INT32_MAX / 2,    INT32_MAX / 8,    INT32_MAX / 16,   INT32_MAX / 24,
	INT32_MAX / 38,   INT32_MAX / 56,   INT32_MAX / 68,   INT32_MAX / 80,
	INT32_MAX / 100,  INT32_MAX / 250,  INT32_MAX / 500,  INT32_MAX / 800,
	INT32_MAX / 1000, INT32_MAX / 3000, INT32_MAX / 5000, INT32_MAX / 8000
};

/*
 * Decay/release times (ms)
 *
 * [0 .. 3]    6,   24,    48,    72
 * [4 .. 7]  114,  168,   204,   240
 * [8 .. B]  300,  750,  1500,  2400
 * [C .. F] 3000, 9000, 15000, 24000
 */
const int32_t decay[16] = {
	INT32_MIN / 6,    INT32_MIN / 24,   INT32_MIN / 48,    INT32_MIN / 72,
	INT32_MIN / 114,  INT32_MIN / 168,  INT32_MIN / 204,   INT32_MIN / 240,
	INT32_MIN / 300,  INT32_MIN / 750,  INT32_MIN / 1500,  INT32_MIN / 2400,
	INT32_MIN / 3000, INT32_MIN / 9000, INT32_MIN / 15000, INT32_MIN / 24000
};

/*
 * LEFT CHANNEL INTERRUPT
 */
void __attribute__((interrupt, no_auto_psv)) _DAC1LInterrupt(void)
{
	/* Millisecond Counter */
	static uint8_t ticktock = 0;

	/* Output this DAC FIFO buffer */
	DAC1LDAT = dac.fifo[0];
	DAC1LDAT = dac.fifo[1];
	DAC1LDAT = dac.fifo[2];
	DAC1LDAT = dac.fifo[3];

	/* Start Update */
	LATAbits.LATA0 = 1;

	/* ~1ms */
	if (++ticktock == MILLISECOND) {
		ticktock = 0;

		/* Envelope Generator */
		eg();
	}

	/* Prepare next DAC FIFO buffer */
	fifo();

	/* End Update */
	LATAbits.LATA0 = 0;

	/* Clear DAC L Channel Interrupt Flag */
	IFS4bits.DAC1LIF = 0;
}

void
init_dsp(void)
{
	CORCONbits.US     = 0;
	CORCONbits.SATA   = 0;
	CORCONbits.SATB   = 0;
	CORCONbits.SATDW  = 1;
	CORCONbits.ACCSAT = 0;
	CORCONbits.RND    = 0;
	CORCONbits.IF     = 0;
}

void
init_io(void)
{
	/* ADC1 Digital Mode */
	AD1PCFGL = 0xFFFF;

	/* RA0 O/P */
	TRISAbits.TRISA0 = 0;
	LATAbits.LATA0 = 0;

	/* RA1 O/P */
	TRISAbits.TRISA1 = 0;
	LATAbits.LATA1 = 0;

	/* RA2 O/P */
	TRISAbits.TRISA2 = 0;
	LATAbits.LATA2 = 0;
}

void
init_uart()
{
	/*
	 * Peripheral Pin Select
	 *
	 * RP2(6) = TX1
	 * RP3(7) = RX1
	 */
	PPSUnLock;
	PPSOutput(OUT_FN_PPS_U1TX, OUT_PIN_PPS_RP2);
	PPSInput(IN_FN_PPS_U1RX, IN_PIN_PPS_RP3);
	PPSLock;

	/* Set Baud Rate */
	U1BRG = BRG1_PC2;

	/* Enable UART 1 */
	U1MODEbits.UARTEN = 1;

	/* Enable Tx */
	U1STAbits.UTXEN = 1;
}

void
init_dac(void)
{
	/* PLL output (FVCO) provides the source clock for the Auxiliary Clock Divider */
	ACLKCONbits.SELACLK = 0;
	/* Auxiliary Oscillator Disabled */
	ACLKCONbits.AOSCMD = 0;
	/* Auxiliary Oscillator is the Clock Source */
	ACLKCONbits.ASRCSEL = 0;
	/* Auxiliary Clock output divided by 1 */
	ACLKCONbits.APSTSCLR = 7;
	/* DAC Default value is the midpoint when Format = 1 */
	DAC1DFLT = 0x0000;
	/* Format is Signed integer */
	DAC1CONbits.FORM = 1;
	/* Analog Output Amplifier is enabled during Sleep Mode/Stop in Idle mode */
	DAC1CONbits.AMPON = 0;
	/* DAC Clock Divider bits */
	DAC1CONbits.DACFDIV = DAC_FDIV;

	/*
	 * LEFT CHANNEL
	 */

	/* Left Channel DAC Output Enabled */
	DAC1STATbits.LOEN = 1;

	/* Interrupt if L Channel FIFO is empty */
	DAC1STATbits.LITYPE = 1;

	/* Clear DAC L Channel Interrupt Flag */
	IFS4bits.DAC1LIF = 0;

	/* Enable DAC L Channel Interrupt */
	IEC4bits.DAC1LIE = 1;

	/*
	 * GO!
	 */

	/* DAC1 Module Enabled */
	DAC1CONbits.DACEN = 1;
}

void
init_voices(void)
{
	for (uint8_t n = 0; n < VOICE_SIZE; ++n) {
		voice[n]->waveform.acc     = 0;
		voice[n]->waveform.control = CONTROL_1000HZ;
		voice[n]->waveform.func    = FUNC_SINE;
		voice[n]->waveform.duty    = DUTY_SQUARE;
		voice[n]->waveform.volume  = INT16_MAX;
		voice[n]->envelope.a       = attack[0];
		voice[n]->envelope.d       = decay [0];
		voice[n]->envelope.s       = INT16_MAX;
		voice[n]->envelope.r       = decay [0];
		voice[n]->envelope.volume  = 0;
		voice[n]->envelope.state   = STATE_NONE;
	}
	voice[0]->envelope.state = STATE_ATTACK;
}

void
uart_putchar(uint8_t b)
{
	while (U1STAbits.UTXBF)
		;

	U1TXREG = b;
}

bool
uart_getchar(uint8_t *b)
{
	if (IFS0bits.U1RXIF) {
		*b = U1RXREG;
		IFS0bits.U1RXIF = 0;

		return true;
	}
	return false;
}

void
uart_putnibble(uint8_t n)
{
	n &= 15;

	if (n < 10)
		n += 48;
	else
		n += 55;

	uart_putchar(n);
}

void
uart_putbyte(uint8_t b)
{
	uart_putnibble(b >> 4);
	uart_putnibble(b);
}

void
uart_putword(uint16_t w)
{
	uart_putbyte(w >> 8);
	uart_putbyte(w);
}

void
uart_putcrlf(void)
{
	uart_putchar('\r');
	uart_putchar('\n');
}

void
uart_putprompt(uint8_t voc)
{
	uart_putcrlf();

	uart_putword(dac.volume);
	uart_putchar('\\');
	uart_putnibble(voice[voc]->waveform.func);
	uart_putchar('~');
	uart_putnibble(voice[voc]->envelope.state);
	uart_putchar('!');
	uart_putword(voice[voc]->waveform.volume);
	uart_putchar('/');
	uart_putnibble(voc);
	uart_putchar('>');

	uart_putchar(' ');
}

void
process_char(uint8_t b)
{
	static uint16_t acc = 0;
	static uint8_t voc = 0;

	if (b >= '0' && b <= '9') {
		acc <<= 4;
		acc += (b - 48);
		uart_putchar(b);
		return;
	}
	else if (b >= 'a' && b <= 'f') {
		acc <<= 4;
		acc += (b - 87);
		uart_putchar(b);
		return;
	}
	else if (b >= 'A' && b <= 'F') {
		acc <<= 4;
		acc += (b - 55);
		uart_putchar(b);
		return;
	}
	switch (b) {
	case '=':
		voice[voc]->waveform.control = acc;
		break;
	case '~':
		voice[voc]->waveform.func = acc & 7;
		break;
	case '%':
		voice[voc]->waveform.duty = acc;
		break;
	case '/':
		voice[voc]->waveform.volume = acc;
		break;
	case '@':
		voice[voc]->envelope.a = attack[(acc & 0xF000) >> 12];
		voice[voc]->envelope.d = decay [(acc & 0x0F00) >>  8];
		voice[voc]->envelope.s =        (acc & 0x00F0) <<  7;
		voice[voc]->envelope.r = decay  [acc & 0x000F];
		break;
	case '!':
		if (voice[voc]->envelope.state == STATE_SUSTAIN) {
			voice[voc]->envelope.state = STATE_RELEASE;
		} else {
			voice[voc]->envelope.volume = 0;
			voice[voc]->envelope.state = STATE_ATTACK;
		}
		break;
	case '>':
		voc = acc & 3;
		break;
	case '\\':
		dac.volume = acc;
		break;
	case '#':
		voice[voc]->waveform.control = notetable[acc & 0x7F];
		break;
	case '\r':
		uart_putprompt(voc);
	default:return;
	}
	uart_putchar(b);
	uart_putprompt(voc);
}

int
main(void)
{
	uint8_t b;

	/* Init Clock */
	CLKDIVbits.PLLPRE = PLL_PRE;
	PLLFBD = PLL_DIV;
	CLKDIVbits.PLLPOST = PLL_POST;

	__builtin_write_OSCCONH(0x01);
	__builtin_write_OSCCONL(0x01);

	while (!OSCCONbits.COSC);
	while (!OSCCONbits.LOCK);

	/* Init DSP */
	init_dsp();

	/* Init PORT I/O */
	init_io();

	/* Init RS-232 */
	init_uart();

	/* Init Voices */
	init_voices();

	/* Init D/A */
	init_dac();

	/* I/O */
	uart_putprompt(0);
	while (true) {
		if (uart_getchar(&b)) {
			process_char(b);
		}
	}
}
