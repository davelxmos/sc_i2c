// Copyright (c) 2011, XMOS Ltd, All rights reserved
// This software is freely distributable under a derivative of the
// University of Illinois/NCSA Open Source License posted in
// LICENSE.txt and at <http://github.xcore.com/>
#ifndef _i2c_slave_h_
#define _i2c_slave_h_

#ifdef __i2c_conf_h_exists__
#include "i2c_conf.h"
#endif

#include <stddef.h>
#include <stdint.h>

/** This interface is used to communication with an I2C slave component.
 *  It provides facilities for reading and writing to the bus. The I2C slave
 *  component acts a *client* to this interface. So the application must
 *  respond to these calls (i.e. the members of the interface are callbacks
 *  to the application).
 *
 */
typedef interface i2c_slave_if {
  /** Master has requested a read.
   *
   *  This function will be called by the component
   *  if the bus master requests a
   *  read from this slave device. The component will clock stretch
   *  until the buffer is filled by the application and then transmit
   *  the buffer back to the master.
   *
   *  \param data   the buffer to fill with data for the master
   *  \param n      the *maximum* number of bytes that can be set. The
   *                exact number of bytes sent back is governed by the bus
   *                master and will depend on the protocol used on the bus.
   */
  void master_requests_read(uint8_t data[n], size_t n);

  /** Master has performed a write.
   *
   *  This function will be called by the component if the bus master
   *  performes a write to this slave device. The component will clock
   *  stretch until the buffer is processed (and then return the ack).
   *
   *  \param data   the buffer filled with data by the master
   *  \param n      the number of bytes transmitted.
   */
  void master_perfomed_write(uint8_t data[n], size_t n);
};


/** I2C slave task.
 *
 *  This function instantiates an i2c_slave component.
 *
 *  \param i   the client end of the i2c_slave_if interface. The component
 *             takes the client end and will make calls on the interface when
 *             the master performs reads or writes.
 *  \param  p_scl  The SCL port of the I2C bus
 *  \param  p_sda  The SDA port of the I2C bus
 *  \param device_addr The address of the slave device
 *  \param max_transaction_size  The maximum number of bytes that will be
 *                               read or written by the master.
 *
 */
void i2c_slave(client interface i2c_slave_if i,
               port p_scl, port p_sda,
               uint8_t device_addr,
               static const size_t max_transaction_size);

#endif // _i2c_slave_h_
