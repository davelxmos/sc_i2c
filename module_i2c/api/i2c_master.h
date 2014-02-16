// Copyright (c) 2011, XMOS Ltd, All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>
#ifndef _i2c_master_h_
#define _i2c_master_h_

#ifdef __i2c_conf_h_exists__
#include "i2c_conf.h"
#endif

#include <stddef.h>
#include <stdint.h>

/** This type is used in I2C write functions to report back on whether the
 *   write is successful or not.
 */
typedef enum {
  I2C_WRITE_ACK_SUCCEEDED, ///< The write was successful
  I2C_WRITE_ACK_FAILED     ///< The write has failed
} i2c_write_res_t;

/** This interface is used to communication with an I2C master component.
 *  It provides facilities for reading and writing to the bus.
 *
 */
typedef interface i2c_master_if {

  /** Write data to an I2C bus.
   *
   *  \param device_addr the address of the slave device to write to
   *  \param buf         the buffer containing data to write
   *  \param n           the number of bytes to write
   *
   *  \returns     whether the write succeeded
   */
  i2c_write_res_t tx(uint8_t device_addr, uint8_t buf[n], size_t n);

  /** Read data from an I2C bus.
   *
   *  \param device_addr the address of the slave device to read from
   *  \param buf         the buffer to fill with data
   *  \param n           the number of bytes to read
   */
  void rx(uint8_t device_addr, uint8_t buf[n], size_t n);


} i2c_master_if;

extends client interface i2c_master_if : {
  /** Read a register on a slave device.
   *
   *  This function reads a register from the i2c bus. The register address
   *  and data can be any number of bytes. The function reads data by
   *  transmitting the register addr and then reading the data from the slave
   *  device.
   *
   *  \param device_addr the address of the slave device to read from
   *  \param reg         the array containing the m-byte register
   *                     address
   *  \param m           the number of bytes in the register address
   *  \param data        the array to fill with data
   *  \param n           the number of bytes to read
   */
  inline void read_reg_n_m(client interface i2c_master_if i,
                           uint8_t device_addr,
                           uint8_t reg[m],
                           size_t m,
                           uint8_t data[n],
                           size_t n)
  {
    i.tx(device_addr, reg, m);
    i.rx(device_addr, data, n);
  }

  /** Read an 8-bit register on a slave device.
   *
   *  This function reads an 8-bit addressed, 8-bit register from the i2c
   *  bus. The function reads data by
   *  transmitting the register addr and then reading the data from the slave
   *  device.
   *
   *  \param device_addr the address of the slave device to read from
   *  \param reg         the address of the register to read
   *
   *  \returns           the value of the register
   */
  inline uint8_t read_reg(client interface i2c_master_if i,
                       uint8_t device_addr, uint8_t reg) {
    uint8_t a_reg[1] = {reg};
    uint8_t data[1];
    i.read_reg_n_m(device_addr, a_reg, 1, data, 1);
    return data[0];
  }

  /** Write a register on a slave device.
   *
   *  This function writes a slave device register on the i2c bus.
   *  The register address
   *  and data can be any number of bytes. The function writes data by
   *  transmitting the register addr and then
   *  transmitting the data to the slave device.
   *
   *  \param device_addr the address of the slave device to write to
   *  \param reg         the array containing the m-byte register
   *                     address
   *  \param m           the number of bytes in the register address
   *  \param data        the array of data to write
   *  \param n           the number of bytes to write
   */
  inline void write_reg_n_m(client interface i2c_master_if i,
                           uint8_t device_addr,
                           uint8_t reg[m],
                           size_t m,
                           uint8_t data[n],
                           size_t n)
  {
    i.tx(device_addr, reg, m);
    i.tx(device_addr, data, n);
  }

  /** Write an 8-bit register on a slave device.
   *
   *  This function writes a slave device register on the i2c bus.
   *  The register address
   *  and data can be any number of bytes. The function writes data by
   *  transmitting the register addr and then
   *  transmitting the data to the slave device.
   *
   *  \param device_addr the address of the slave device to write to
   *  \param reg         the address of the register to write
   *  \param data        the 8-bit value to write
   */
  inline void write_reg(client interface i2c_master_if i,
                        uint8_t device_addr, uint8_t reg, uint8_t data) {
    uint8_t a_reg[1] = {reg};
    uint8_t a_data[1] = {data};
    i.write_reg_n_m(device_addr, a_reg, 1, a_data, 1);
  }


}


enum i2c_enable_mm {
  I2C_ENABLE_MULTIMASTER,
  I2C_DISABLE_MULTIMASTER
};

/** Implements I2C on the i2c_master_if interface using two ports.
 *
 *  \param  c      An array of server interface connections for clients to
 *                 connect to
 *  \param  n      The number of clients connected
 *  \param  p_scl  The SCL port of the I2C bus
 *  \param  p_sda  The SDA port of the I2C bus
 *  \param  kbits_per_second The speed of the I2C bus
 *  \param  enable_multimaster A flag indicating whether multi-master
 *                             support should be enabled. This should be one
 *                             of the values ``I2C_ENABLE_MULTIMASTER`` or
 *                             ``I2C_DISABLE_MULTIMASTER``.
 **/
[[distributable]] void i2c_master(server interface i2c_master_if c[n],
                                  size_t n,
                                  port p_scl, port p_sda,
                                  unsigned kbits_per_second,
                                  enum i2c_enable_mm enable_multi_master);

/** Implements I2C on a single multi-bit port.
 *
 *  *Reading from the bus is not supported in this implementation*.
 *  *Multi master support is not available in this implementation*.
 *
 *  \param  c      An array of server interface connections for clients to
 *                 connect to
 *  \param  n      The number of clients connected
 *  \param  p_i2c  The multi-bit port containing both SCL and SDA.
 *                 You will need to set the relevant defines in i2c_conf.h in
 *                 you application to say which bits of the port are used
 *  \param  kbits_per_second The speed of the I2C bus
 *  \param  sda_bit_position The bit position of the SDA line on the port
 *  \param  scl_bit_position The bit position of the SCL line on the port
 *  \param  other_bits_mask  The mask for the other bits of the port to use
 *                           when driving it.  Note that, on occassions,
 *                           the other bits are left to float, so external
 *                           resistors shall be used to reinforce the default
 *                           value
 */
[[distributable]]
void i2c_master_single_port(server interface i2c_master_if c[n], size_t n,
                            port p_i2c, unsigned kbits_per_second,
                            unsigned sda_bit_position,
                            unsigned scl_bit_position,
                            unsigned other_bits_mask);




#endif // _i2c_master_h_
