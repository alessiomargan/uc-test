/*
 * Fixed PDO transfer path for the F28P65x test subdevice.
 *
 * These offsets are the wire layout of 0x1600 and 0x1A00 in objectlist.c.
 * Keep this file in sync with those mappings. Unlike the generic mapper,
 * this path uses the F28 ESC HAL's native 16/32-bit PDI accesses directly.
 */

#include <soes/esc.h>
#include <soes/esc_octet.h>
#include <ecat_options.h>
#include <globals.h>

#define RXPDO_POS_REF_OFFSET       0U
#define RXPDO_VEL_REF_OFFSET       4U
#define RXPDO_TOR_REF_OFFSET       8U
#define RXPDO_GAINS_OFFSET          12U
#define RXPDO_FAULT_ACK_OFFSET      32U
#define RXPDO_TIMESTAMP_OFFSET      34U
#define RXPDO_OP_IDX_AUX_OFFSET     36U
#define RXPDO_AUX_OFFSET            38U

#define F28P65X_RXPDO_WIRE_SIZE     42U


#define TXPDO_LINK_POS_OFFSET       0U
#define TXPDO_MOTOR_POS_OFFSET      4U
#define TXPDO_LINK_VEL_OFFSET       8U
#define TXPDO_MOTOR_VEL_OFFSET      12U
#define TXPDO_TORQUE_OFFSET         16U
#define TXPDO_TEMPERATURE_OFFSET    20U
#define TXPDO_FAULT_OFFSET          22U
#define TXPDO_RTT_OFFSET            24U
#define TXPDO_OP_IDX_ACK_OFFSET     26U
#define TXPDO_AUX_OFFSET            28U

#define F28P65X_TXPDO_WIRE_SIZE     32U

typedef union
{
   uint32_t word;
   float value;
} pdo_float_t;

static esc_octet_t local_rx_wire[F28P65X_RXPDO_WIRE_SIZE];
static esc_octet_t local_tx_wire[F28P65X_TXPDO_WIRE_SIZE];

static inline float pdo_get_float(const esc_octet_t *wire, uint16_t offset)
{
   pdo_float_t value;

   value.word = esc_get_le32(wire, offset);
   return value.value;
}

static inline void pdo_put_float(esc_octet_t *wire, uint16_t offset,
                                 float source)
{
   pdo_float_t value;

   value.value = source;
   esc_put_le32(wire, offset, value.word);
}




/* Fixed PDO mapping using the proven C28 octet transfer representation. */
__attribute__((section(".TI.ramfunc")))
void f28p65x_rxpdo_override(void)
{
   uint16_t gain;

   ESC_read_octets(ESC_SM2_sma, local_rx_wire, F28P65X_RXPDO_WIRE_SIZE);
   rx_pdo.pos_ref = pdo_get_float(local_rx_wire, RXPDO_POS_REF_OFFSET);
   rx_pdo.vel_ref = pdo_get_float(local_rx_wire, RXPDO_VEL_REF_OFFSET);
   rx_pdo.tor_ref = pdo_get_float(local_rx_wire, RXPDO_TOR_REF_OFFSET);
   for (gain = 0U; gain < 5U; gain++)
   {
      rx_pdo.gains[gain] = pdo_get_float(local_rx_wire,
                                         RXPDO_GAINS_OFFSET + (gain * 4U));
   }
   rx_pdo.fault_ack = esc_get_le16(local_rx_wire, RXPDO_FAULT_ACK_OFFSET);
   rx_pdo.ts = esc_get_le16(local_rx_wire, RXPDO_TIMESTAMP_OFFSET);
   rx_pdo.op_idx_aux = esc_get_le16(local_rx_wire, RXPDO_OP_IDX_AUX_OFFSET);
   rx_pdo.aux = pdo_get_float(local_rx_wire, RXPDO_AUX_OFFSET);
}

__attribute__((section(".TI.ramfunc")))
void f28p65x_txpdo_override(void)
{
   pdo_put_float(local_tx_wire, TXPDO_LINK_POS_OFFSET, tx_pdo.link_pos);
   pdo_put_float(local_tx_wire, TXPDO_MOTOR_POS_OFFSET, tx_pdo.motor_pos);
   pdo_put_float(local_tx_wire, TXPDO_LINK_VEL_OFFSET, tx_pdo.link_vel);
   pdo_put_float(local_tx_wire, TXPDO_MOTOR_VEL_OFFSET, tx_pdo.motor_vel);
   pdo_put_float(local_tx_wire, TXPDO_TORQUE_OFFSET, tx_pdo.torque);
   esc_put_le16(local_tx_wire, TXPDO_TEMPERATURE_OFFSET, tx_pdo.temperature);
   esc_put_le16(local_tx_wire, TXPDO_FAULT_OFFSET, tx_pdo.fault);
   esc_put_le16(local_tx_wire, TXPDO_RTT_OFFSET, tx_pdo.rtt);
   esc_put_le16(local_tx_wire, TXPDO_OP_IDX_ACK_OFFSET, tx_pdo.op_idx_ack);
   pdo_put_float(local_tx_wire, TXPDO_AUX_OFFSET, tx_pdo.aux);
   ESC_write_octets(ESC_SM3_sma, local_tx_wire, F28P65X_TXPDO_WIRE_SIZE);
}
