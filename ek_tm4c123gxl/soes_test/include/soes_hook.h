#ifndef __SOES_HOOK_H__
#define __SOES_HOOK_H__


typedef struct {
    uint16_t    _type;
    int32_t     _value;
    uint64_t    _ts;
} __attribute__((__packed__)) rx_pdo_t;


typedef struct {
    uint8_t     ____bits;
        /*
    uint8_t     _bit_0:1;
    uint8_t     _bit_1:1;
    uint8_t     _bit_2:1;
    uint8_t     _bit_3:1;
    uint8_t     _bit_4:1;
    uint8_t     _bit_5:1;
    uint8_t     _bit_6:1;
    uint8_t     _bit_7:1; 
    */ 
    uint8_t     _bits;
    int8_t      _sint;
    uint8_t     _usint;
    int16_t     _int;
    uint16_t    _uint;
    int32_t     _dint;
    uint32_t    _udint;
    int64_t     _lint;
    uint64_t    _ulint;
    float       _real;
} __attribute__((__packed__)) tx_pdo_t;



extern tx_pdo_t tx_pdo;
extern rx_pdo_t rx_pdo;

extern int par_1;
extern int par_2;

void setup_esc_configs(void);

void ecat_process_pdo(void);

#endif 
