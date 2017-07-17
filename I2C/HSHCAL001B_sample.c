
/*I2C Address*/
#define	HSHCAL_SLVADR	0x30

/*--------------------------------------------------------------------------------------------------------------*/
/*	Register Address(HSHCAL001B)																				*/
/*--------------------------------------------------------------------------------------------------------------*/

/* Register for hshcal */
#define HSHCAL_STR			0x0C	/* Self test response  */
#define HSHCAL_INF1			0x0D	/* More info Version   */
#define HSHCAL_INF2			0x0E	/* More info ALPS      */
#define HSHCAL_WIA			0x0F	/* Who I am            */
#define HSHCAL_HUMI1		0x10	/* Humidity output1    */
#define HSHCAL_HUMI2		0x11	/* Humidity output2    */
#define HSHCAL_TEMP1		0x12	/* Temperature output1 */
#define HSHCAL_TEMP2		0x13	/* Temperature output2 */
#define HSHCAL_STAT			0x18	/* Status              */
#define HSHCAL_CTL1			0x1B	/* Control1            */
#define HSHCAL_RDAT1		0x20	/* Reserved data1      */
#define HSHCAL_RDAT2		0x21	/* Reserved data2      */
#define HSHCAL_RDAT3		0x22	/* Reserved data3      */
#define HSHCAL_RDAT4		0x23	/* Reserved data4      */

/* Action Command Register for hshcal */
#define HSHCAL_REST			0x30	/* Reset command         */
#define HSHCAL_DET			0x31	/* Sensor detect command */
#define HSHCAL_STST			0x32	/* Self test command     */

#define HSHCAL_CHIP_ID		0x2311

/* Self-test resiter value */
#define HSHCAL_ST_REG_DEF	0x55
#define HSHCAL_ST_REG_PASS	0xAA

/* Self-test error number */
#define HSHCAL_ST_OK		0x00
#define HSHCAL_ST_ERR_I2C	0x01
#define HSHCAL_ST_ERR_INIT	0x02
#define HSHCAL_ST_ERR_1ST	0x03
#define HSHCAL_ST_ERR_2ND	0x04

#define HSHCAL_MLT_DATA_ACCESS_NUM	4
#define HSHCAL_SIG_DATA_ACCESS_NUM	2
#define HSHCAL_INITIALL_DELAY		20

/*--------------------------------------------------------------------------------------------------------------*/
/*	RegisterÅEBit Define(HSHCAL001B)																			*/
/*--------------------------------------------------------------------------------------------------------------*/

#define		HUM_SRDY		0x80			//	The flag under starting (0:Inactive, 1:Active)
#define		HUM_DRDY		0x40			//	The flag of the end of humidity sensor measurement (0:Inactive, 1:Active)
#define		HUM_DOR			0x20			//	Detection of data overrun (0:No overrrun, 1:Detect Overrun)
#define		HUM_MEAS		0x10			//	The flag of measurement execution (0:Sleep, 1:Execute Measurement)
#define		HUM_TRDY		0x02			//	The flag of the end of temperature sensor measurement (0:Inactive, 1:Active)

/*******************************************************************************
* Function Name  : I2C_ByteWrite
* Description    : write a Byte to I2C Bus
* Input          : deviceAddr is the I2C address of the device
*                  WriteAddr is the register address you want to write to
*                  pBuffer contains bytes to write
* Output         : None
* Return         : None
*******************************************************************************/
void I2C_ByteWrite(uint8_t* pBuffer, uint8_t deviceAddress, uint8_t WriteAddr);


/*******************************************************************************
* Function Name  : I2C_BufferRead
* Description    : read a numByteToRead bytes from I2C Bus
* Input          : deviceAddr is the I2C address of the device
*                  readAddr is the register address you want to read from
*                  numByteToRead is the number of bytes to read
* Output         : pBuffer is the buffer that contains bytes read
* Return         : None
*******************************************************************************/
uint8_t I2C_BufferRead(uint8_t* pBuffer, uint8_t deviceAddr, uint8_t readAddr, uint16_t numByteToRead);



static	VO	HumInit()
{
	BOOL	ack;
	UB		buf[2];
	UB		WhoAmI;

	I2C_BufferRead(buf, HSHCAL_SLVADR, HSHCAL_REST, 1);
	pudelay_ms(100);
	
	I2C_BufferRead(buf, HSHCAL_SLVADR, HSHCAL_CTL1, 2);
	pudelay_ms(100);
	
	ack = I2C_BufferRead(&WhoAmI, HSHCAL_SLVADR, HSHCAL_WIA, 1);
	if (ack == TRUE  &&  WhoAmI == 0x49) return;
}



static	VO	MainProc(VO)
{
	unsigned char	csÅCsr;
	signed long humi_raw = 0, temp_raw = 0;
	double Humi,Temp;
	
	HumInit();	//Init only need to do one time
	
	I2C_BufferRead(&sr1, HSHCAL_SLVADR, HSHCAL_STAT, 1);
	fDRDY = ((sr & SR1_DRDY) != 0 && (sr & 0x02) != 0);
	
	cs = HSHCAL_DET;
	I2C_ByteWrite(&cs, HSHCAL_SLVADR, HSHCAL_DET);
	Delay(25);	//delay 25msÅCat least20ms
	
	if(fDRDY == 1) {
		I2C_BufferRead((void*)&humi, HSHCAL_SLVADR, HSHCAL_HUMI1, 2);
		I2C_BufferRead((void*)&temp, HSHCAL_SLVADR, HSHCAL_TEMP1, 2);
	}
	
	Humi = 0.015625 * humi_raw - 14;
	Temp = 0.02 * temp_raw - 41.92;
}
