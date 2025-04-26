/**
 * Pico port of DRV8711 library https://github.com/jancumps/drv8711_lib
 */

module;

#include "hardware/gpio.h"
#include "hardware/spi.h"

import drv8711;
import drv8711_config;

export module drv8711_pico;
export namespace drv8711_pico {

class driver_pico : public drv8711::driver {
public:    
    driver_pico(spi_inst_t *spi, uint baudrate,
        uint cs, uint rx, uint tx, uint sck,
        uint n_sleep, uint reset) : drv8711::driver(),
        spi_(spi), baudrate_(baudrate),
        cs_(cs), rx_(rx), tx_(tx), sck_(sck),
        n_sleep_(n_sleep), reset_(reset) {}
    
    virtual bool init() override {
        init_gpio();
        init_spi();
        init_registers();
        return true;
    }

    virtual void enable(bool enable) override {
        gpio_put(n_sleep_, enable ? 1 : 0);
    }

private:
    void init_spi() override{
        // Enable SPI 0 at 1 MHz and connect to GPIOs
        spi_init(spi_, baudrate_);
        spi_set_format(spi_, 16, SPI_CPOL_0, SPI_CPHA_0, SPI_MSB_FIRST); // 16 bit registers
        gpio_set_function(rx_, GPIO_FUNC_SPI);
        gpio_set_pulls(rx_, true, false); // drv8711 outputs are open drain
        gpio_set_function(sck_, GPIO_FUNC_SPI);
        gpio_set_function(tx_, GPIO_FUNC_SPI);
        // CS is active-high, invert pin action
        gpio_set_function(cs_, GPIO_FUNC_SPI);
        gpio_set_outover(cs_, GPIO_OVERRIDE_INVERT);
    }

    void init_gpio() override{
        // nsleep as output
        gpio_init(n_sleep_);
        gpio_put(n_sleep_, 0);
        gpio_set_dir(n_sleep_, GPIO_OUT);
        // reset as output
        gpio_init(reset_);
        gpio_put(reset_, 0);
        gpio_set_dir(reset_, GPIO_OUT);
    }

    // initialise all registers from the defaults
    // defined in module drv8711_config
    // developer can override values before calling
    void init_registers() override{
        write(drv8711::reg_ctrl);
        write(drv8711::reg_torque);
        write(drv8711::reg_off);
        write(drv8711::reg_blank);
        write(drv8711::reg_decay);
        write(drv8711::reg_stall);
        write(drv8711::reg_drive);
        write(drv8711::reg_status);
    }

    // write to a register
    virtual void write(uint16_t reg) override {
        spi_write16_blocking(spi_, &reg, 1);
    }

private:
    spi_inst_t * spi_;
    uint baudrate_;
    uint cs_;
    uint rx_;
    uint tx_;
    uint sck_;
    uint n_sleep_;
    uint reset_;
};

} // namespace drv8711_pico