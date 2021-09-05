/**
 * @license    BSD 3-Clause
 * @copyright  Pawel Okas
 * @version    $Id$
 * @brief
 *
 * @authors    Pawel Okas
 * created on: 16-04-2014
 * last modification: 20-03-2018
 *
 * @copyright Copyright (c) 2014-2018, Pawel Okas
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef STM32F4DISCOVERY_H_
#define STM32F4DISCOVERY_H_

#include "i2c.h"
#include "microhal.h"

namespace bsp {
void init();
namespace bmp180 {
static microhal::I2C &i2c = microhal::stm32f4xx::I2C::i2c2;
}
static microhal::SerialPort &debugPort = microhal::stm32f4xx::SerialPort::Serial3;
}  // namespace bsp

constexpr microhal::IOPin Led3(microhal::stm32f4xx::GPIO::Port::PortD, 13);
constexpr microhal::IOPin Led4(microhal::stm32f4xx::GPIO::Port::PortD, 12);
constexpr microhal::IOPin Led5(microhal::stm32f4xx::GPIO::Port::PortD, 14);
constexpr microhal::IOPin Led6(microhal::stm32f4xx::GPIO::Port::PortD, 15);

constexpr microhal::IOPin Sw1(microhal::stm32f4xx::GPIO::Port::PortA, 0);

constexpr microhal::IOPin GreenLed = Led4;
constexpr microhal::IOPin RedLed = Led3;

#endif /* STM32F4DISCOVERY_H_ */
