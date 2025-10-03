/*
 * modbustcp_ip.c
 *
 *  Created on: Oct 2, 2025
 *      Author: Art laptop
 */

#include "modbustcp_ip.h"

#define MODBUS_PORT 502
#define SOCK_MODBUS 0
uint8_t rxBuf[512];
uint8_t txBuf[512];
static uint8_t sock_initialized = 0;


static SPI_HandleTypeDef *w5500_spi;
static GPIO_TypeDef *w5500_cs_port;
static uint16_t w5500_cs_pin;
static GPIO_TypeDef *w5500_rst_port;
static uint16_t w5500_rst_pin;

uint8_t coils[COILS_N] = {0};                // coil states (bits)
uint8_t discreteInputs[DISCRETE_INPUTS_N] = {0}; // discrete inputs
uint16_t inputRegs[INPUT_REGS_N] = {10,20,30,40,50,60,70,80,90,100};
uint16_t holdingRegs[HOLDING_REGS_N] = {0};

uint32_t prev_task ;


static void cs_sel(void)   { HAL_GPIO_WritePin(w5500_cs_port, w5500_cs_pin, GPIO_PIN_RESET); }
static void cs_desel(void) { HAL_GPIO_WritePin(w5500_cs_port, w5500_cs_pin, GPIO_PIN_SET);   }

/* --- Reset function --- */
static void reset_w5500(void) {
    HAL_GPIO_WritePin(w5500_rst_port, w5500_rst_pin, GPIO_PIN_RESET);
    HAL_Delay(10);
    HAL_GPIO_WritePin(w5500_rst_port, w5500_rst_pin, GPIO_PIN_SET);
    HAL_Delay(100);
}

/* --- SPI functions --- */
static uint8_t spi_rb(void) {
    uint8_t rb = 0;
    HAL_SPI_Receive(w5500_spi, &rb, 1, HAL_MAX_DELAY);
    return rb;
}
static void spi_wb(uint8_t wb) {
    HAL_SPI_Transmit(w5500_spi, &wb, 1, HAL_MAX_DELAY);
}
/* ------------------- Modbus Processor ------------------- */
int modbus_process(uint8_t *req, int len, uint8_t *resp) {
    if (len < 8) return 0;

    uint16_t protoId = (req[2] << 8) | req[3];
    uint8_t unitId = req[6];
    uint8_t func   = req[7];

    if (protoId != 0) return 0;

    /* ---- 0x01: Read Coils ---- */
    if (func == 1 && len >= 12) {
        uint16_t startAddr = (req[8] << 8) | req[9];
        uint16_t quantity  = (req[10] << 8) | req[11];
        if (quantity > COILS_N) quantity = COILS_N;

        uint8_t byteCount = (quantity + 7) / 8;
        resp[0]=req[0]; resp[1]=req[1];
        resp[2]=0; resp[3]=0;
        resp[4]=0; resp[5]=3+byteCount;
        resp[6]=unitId; resp[7]=func;
        resp[8]=byteCount;

        memset(&resp[9], 0, byteCount);
        for (int i=0;i<quantity;i++) {
            if (coils[startAddr+i]) resp[9+i/8] |= (1 << (i%8));
        }
        return 9+byteCount;
    }

    /* ---- 0x02: Read Discrete Inputs ---- */
    if (func == 2 && len >= 12) {
        uint16_t startAddr = (req[8] << 8) | req[9];
        uint16_t quantity  = (req[10] << 8) | req[11];
        if (quantity > DISCRETE_INPUTS_N) quantity = DISCRETE_INPUTS_N;

        uint8_t byteCount = (quantity+7)/8;
        resp[0]=req[0]; resp[1]=req[1];
        resp[2]=0; resp[3]=0;
        resp[4]=0; resp[5]=3+byteCount;
        resp[6]=unitId; resp[7]=func;
        resp[8]=byteCount;

        memset(&resp[9], 0, byteCount);
        for (int i=0;i<quantity;i++) {
            if (discreteInputs[startAddr+i]) resp[9+i/8] |= (1<<(i%8));
        }
        return 9+byteCount;
    }

    /* ---- 0x03: Read Holding Registers ---- */
    if (func == 3 && len >= 12) {
        uint16_t startAddr = (req[8] << 8) | req[9];
        uint16_t quantity  = (req[10] << 8) | req[11];
        if (quantity > HOLDING_REGS_N) quantity = HOLDING_REGS_N;

        resp[0]=req[0]; resp[1]=req[1];
        resp[2]=0; resp[3]=0;
        resp[4]=0; resp[5]=3+quantity*2;
        resp[6]=unitId; resp[7]=func;
        resp[8]=quantity*2;
        for (int i=0;i<quantity;i++) {
            uint16_t val = holdingRegs[startAddr+i];
            resp[9+i*2]  = val>>8;
            resp[10+i*2] = val&0xFF;
        }
        return 9+quantity*2;
    }

    /* ---- 0x04: Read Input Registers ---- */
    if (func == 4 && len >= 12) {
        uint16_t startAddr = (req[8] << 8) | req[9];
        uint16_t quantity  = (req[10] << 8) | req[11];
        if (quantity > INPUT_REGS_N) quantity = INPUT_REGS_N;

        resp[0]=req[0]; resp[1]=req[1];
        resp[2]=0; resp[3]=0;
        resp[4]=0; resp[5]=3+quantity*2;
        resp[6]=unitId; resp[7]=func;
        resp[8]=quantity*2;
        for (int i=0;i<quantity;i++) {
            uint16_t val = inputRegs[startAddr+i];
            resp[9+i*2]  = val>>8;
            resp[10+i*2] = val&0xFF;
        }
        return 9+quantity*2;
    }

    /* ---- 0x05: Write Single Coil ---- */
    if (func == 5 && len >= 12) {
        uint16_t coilAddr = (req[8]<<8)|req[9];
        uint16_t value    = (req[10]<<8)|req[11];
        if (coilAddr < COILS_N) {
            coils[coilAddr] = (value==0xFF00)?1:0;
        }
        memcpy(resp, req, 12); // echo request
        return 12;
    }

    /* ---- 0x06: Write Single Register ---- */
    if (func == 6 && len >= 12) {
        uint16_t regAddr = (req[8]<<8)|req[9];
        uint16_t value   = (req[10]<<8)|req[11];
        if (regAddr < HOLDING_REGS_N) holdingRegs[regAddr] = value;
        memcpy(resp, req, 12);
        return 12;
    }

    /* ---- 0x0F: Write Multiple Coils ---- */
    if (func == 15 && len >= 14) {
        uint16_t startAddr = (req[8]<<8)|req[9];
        uint16_t quantity  = (req[10]<<8)|req[11];
        for (int i=0;i<quantity;i++) {
            uint8_t byte = req[13+i/8];
            uint8_t bit  = (byte>>(i%8))&1;
            if ((startAddr+i)<COILS_N) coils[startAddr+i]=bit;
        }
        resp[0]=req[0]; resp[1]=req[1];
        resp[2]=0; resp[3]=0;
        resp[4]=0; resp[5]=6;
        resp[6]=unitId; resp[7]=func;
        resp[8]=req[8]; resp[9]=req[9];
        resp[10]=req[10]; resp[11]=req[11];
        return 12;
    }

    /* ---- 0x10: Write Multiple Registers ---- */
    if (func == 16 && len >= 13) {
        uint16_t startAddr = (req[8]<<8)|req[9];
        uint16_t quantity  = (req[10]<<8)|req[11];
        for (int i=0;i<quantity;i++) {
            uint16_t value = (req[13+i*2]<<8)|req[14+i*2];
            if ((startAddr+i)<HOLDING_REGS_N) holdingRegs[startAddr+i]=value;
        }
        resp[0]=req[0]; resp[1]=req[1];
        resp[2]=0; resp[3]=0;
        resp[4]=0; resp[5]=6;
        resp[6]=unitId; resp[7]=func;
        resp[8]=req[8]; resp[9]=req[9];
        resp[10]=req[10]; resp[11]=req[11];
        return 12;
    }

    /* ---- Unsupported function ---- */
    resp[0]=req[0]; resp[1]=req[1];
    resp[2]=0; resp[3]=0;
    resp[4]=0; resp[5]=3;
    resp[6]=unitId;
    resp[7]=func|0x80;
    resp[8]=0x01; // illegal function
    return 9;
}


void Modbus_TCPIP_init(Ethernet_Config_t *cfg){
	 	w5500_spi      = cfg->hspi;
	    w5500_cs_port  = cfg->cs_port;
	    w5500_cs_pin   = cfg->cs_pin;
	    w5500_rst_port = cfg->rst_port;
	    w5500_rst_pin  = cfg->rst_pin;

	    // Reset and register callbacks
	    reset_w5500();
	    reg_wizchip_cs_cbfunc(cs_sel, cs_desel);
	    reg_wizchip_spi_cbfunc(spi_rb, spi_wb);

	    // Init memory sizes
	    uint8_t txsize[8] = {2,2,0,0,0,0,0,0};
	    uint8_t rxsize[8] = {2,2,0,0,0,0,0,0};
	    wizchip_init(txsize, rxsize);

	    // Set network parameters
	    setSHAR(cfg->mac);
	    setSIPR(cfg->ip);
	    setGAR(cfg->gw);
	    setSUBR(cfg->sn);
	    prev_task = HAL_GetTick();

    }


void Modbus_TCPIP_process(void){
	 uint32_t now = HAL_GetTick();

	    // --- Initialize socket once ---
	    if (!sock_initialized) {
	        if (socket(SOCK_MODBUS, Sn_MR_TCP, MODBUS_PORT, 0) != SOCK_MODBUS) {
	            close(SOCK_MODBUS);
	            return; // try again next call
	        }
	        if (listen(SOCK_MODBUS) != SOCK_OK) {
	            close(SOCK_MODBUS);
	            return;
	        }
	        sock_initialized = 1;
	    }

	    // --- Check socket status ---
	    uint8_t status = getSn_SR(SOCK_MODBUS);

	    if (status == SOCK_ESTABLISHED) {
	        int len = recv(SOCK_MODBUS, rxBuf, sizeof(rxBuf));
	        if (len > 0) {
	            int rlen = modbus_process(rxBuf, len, txBuf);
	            if (rlen > 0) send(SOCK_MODBUS, txBuf, rlen);
	        }

	        // Example periodic task
	        if ((now - prev_task) >= TASK_INTERVAL) {
	            prev_task += TASK_INTERVAL;
	            holdingRegs[0]++;
	        }
	    }
	    else if (status == SOCK_CLOSE_WAIT) {
	        disconnect(SOCK_MODBUS);
	    }
	    else if (status == SOCK_CLOSED) {
	        close(SOCK_MODBUS);
	        sock_initialized = 0; // allow re-init next call
	    }}

