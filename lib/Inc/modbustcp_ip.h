/*
 * modbustcp_ip.h
 *
 *  Created on: Oct 2, 2025
 *      Author: Art laptop
 */

#ifndef INC_MODBUSTCP_IP_H_
#define INC_MODBUSTCP_IP_H_

#include "wizchip_conf.h"
#include "socket.h"
#include <string.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"



#define TASK_INTERVAL 500

#define COILS_N          16
#define DISCRETE_INPUTS_N 16
#define INPUT_REGS_N     10
#define HOLDING_REGS_N   10

typedef struct {
    SPI_HandleTypeDef *hspi;   // SPI handle
    GPIO_TypeDef *cs_port;     // CS pin port
    uint16_t cs_pin;           // CS pin
    GPIO_TypeDef *rst_port;    // Reset pin port
    uint16_t rst_pin;          // Reset pin
    uint8_t mac[6];
    uint8_t ip[4];
    uint8_t gw[4];
    uint8_t sn[4];
} Ethernet_Config_t;


int modbus_process(uint8_t *, int , uint8_t *);
void Modbus_TCPIP_init(Ethernet_Config_t *cfg);
void Modbus_TCPIP_process(void);

#endif /* INC_MODBUSTCP_IP_H_ */
