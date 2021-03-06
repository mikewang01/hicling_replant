/**
 * \addtogroup BSP
 * \{
 * \addtogroup ADAPTERS
 * \{
 * \addtogroup I2C_ADAPTER
 *
 * \brief I2C adapter
 *
 * \{
 */

/**
 *****************************************************************************************
 *
 * @file ad_i2c.h
 *
 * @brief I2C device access API
 *
 * Copyright (C) 2015. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd. All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <black.orca.support@diasemi.com> and contributors.
 *
 *****************************************************************************************
 */

#ifndef AD_I2C_H_
#define AD_I2C_H_

#if dg_configI2C_ADAPTER

#include <osal.h>
#include <resmgmt.h>
#include <hw_i2c.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \def CONFIG_I2C_ONE_DEVICE_ON_BUS
 *
 * \brief Macro to configure only one device on the I2C bus
 *
 * Set this macro to 1 if only one I2C device exists on the bus to reduce code size and
 * imporove performance.
 */
#ifndef CONFIG_I2C_ONE_DEVICE_ON_BUS
# define CONFIG_I2C_ONE_DEVICE_ON_BUS   (0)
#endif

/**
 * \def CONFIG_I2C_EXCLUSIVE_OPEN
 *
 * \brief Macro to configure exclusive use of devices
 *
 * Set this macro to 1 in order to enable preventing multiple tasks opening the
 * same device. When set to 1, ad_i2c_device_acquire() and ad_i2c_device_release()
 * are no longer necessary.
 */
#ifndef CONFIG_I2C_EXCLUSIVE_OPEN
# define CONFIG_I2C_EXCLUSIVE_OPEN      (0)
#endif

/**
 * \def CONFIG_I2C_RESOURCE_STATIC_ID
 *
 * \brief Macro to configure resource ID assignement for each device
 *
 * Set this macro to 1 to enable unique resource ID asignment per device.
 */
#ifndef CONFIG_I2C_RESOURCE_STATIC_ID
# define CONFIG_I2C_RESOURCE_STATIC_ID  (0)
#endif

/**
 * \brief Device pointer, handle to use with ad_i2c_read(), ad_i2c_write() etc.
 *
 */
typedef void *i2c_device;

/**
 * \brief Device id, those are created by I2C_SLAVE_DEVICE or I2C_SLAVE_TO_EXT_MASTER macro in
 * platform_devices.h
 *
 */
typedef const void *i2c_device_id;

#ifndef I2C_SLAVE_DEVICE

/**
 * \brief Entry for slave device
 *
 * \param [in] bus_id value must match I2C_BUS() argument valid values: I2C1, I2C2
 * \param [in] name name that will be used later to open device
 * \param [in] addr slave device address
 * \param [in] addr_mode slave device address mode from HW_I2C_ADDRESSING enum
 * \param [in] speed I2C clock speed from HW_I2C_SPEED enum
 *
 */
#define I2C_SLAVE_DEVICE(bus_id, name, addr, addr_mode, speed) \
                                I2C_SLAVE_DEVICE_DMA(bus_id, name, addr, addr_mode, speed, -1)

/**
 * \brief Entry for slave device with DMA
 *
 * \param [in] bus_id value must match I2C_BUS() argument valid values: I2C1, I2C2
 * \param [in] name name that will be used later to open device
 * \param [in] addr slave device address
 * \param [in] addr_mode slave device address mode from HW_I2C_ADDRESSING enum
 * \param [in] speed I2C clock speed from HW_I2C_SPEED enum
 * \param [in] dma_channel DMA channel to use, -1 for no DMA
 *
 */
#define I2C_SLAVE_DEVICE_DMA(bus_id, name, addr, addr_mode, speed, dma_channel) \
        extern const void *const name;

/**
 * \brief Entry for device representing I2C controller in slave mode
 *
 * \param [in] bus_id value must match I2C_BUS() argument valid values: I2C1, I2C2
 * \param [in] name name that will be used later to open device
 * \param [in] addr slave device address
 * \param [in] addr_mode slave device address mode from HW_I2C_ADDRESSING enum
 * \param [in] speed I2C clock speed from HW_I2C_SPEED enum
 *
 */
#define I2C_SLAVE_TO_EXT_MASTER(bus_id, name, addr, addr_mode, speed) \
        extern const void *const name;

/**
 * \brief Starting entry for I2C bus devices
 *
 * \param [in] bus_id identifies I2C bus I2C1 or I2C2
 *
 */
#define I2C_BUS(bus_id) \
        extern i2c_bus_dynamic_data dynamic_##bus_id;

/**
 * \brief This ends I2C bus device list started with I2C_BUS()
 *
 */
#define I2C_BUS_END

/**
 * \brief Initialization of I2C bus variables
 *
 * This macro must be invoked somewhere during system startup to initialize variables needed to
 * manage I2C bus. It will create some OS specific synchronization primitives.
 * Each bus created with I2C_BUS() must have corresponding call to this macro.
 * If both I2Cs are used somewhere in initialization sequence must be:
 *      I2C_BUS_INIT(I2C1);
 *      I2C_BUS_INIT(I2C2);
 *
 * \param [in] bus_id identifies I2C bus I2C or I2C
 *
 */
#define I2C_BUS_INIT(bus_id) ad_i2c_bus_init(&dynamic_##bus_id)

/**
 * \brief Initialization of I2C bus devices
 *
 * This macro must be invoked somewhere during system startup to initialize variables needed to
 * manage access to I2C devices.
 * This is important if CONFIG_I2C_RESOURCE_STATIC_ID is defined as 0, because resource
 * id required for device access needs to be created at some point.
 * Each device created with I2C_SLAVE_DEVICE() and I2C_SLAVE_TO_EXT_MASTER() must have
 * corresponding call to this macro.
 * Example:
 * \code{.c}
 *      I2C_BUS_INIT(I2C1);
 *      I2C_DEVICE_INIT(MEM_24LC256);
 * \endcode
 *
 * \param [in] name identifies I2C device connected to bus
 *
 */
#define I2C_DEVICE_INIT(name) ad_i2c_device_init(name)

#endif

/**
 * \brief Asynchronous callback function
 *
 */
typedef void (*ad_i2c_user_cb)(void *user_data, HW_I2C_ABORT_SOURCE error);

/**
 * \brief Initialize adapter
 *
 */
void ad_i2c_init(void);

/**
 * \brief Initialize bus variables
 *
 * \warning Don't call this function directly use I2C_BUS_INIT macro.
 *
 */
void ad_i2c_bus_init(void *bus_data);

/**
 * \brief Initialize device variables
 *
 * \warning Don't call this function directly use I2C_DEVICE_INIT macro.
 *
 */
void ad_i2c_device_init(const i2c_device_id id);

/**
 * \brief Open device connected to I2C bus
 *
 * If system is configured for single device on each I2C bus
 * (defined CONFIG_I2C_ONE_DEVICE_ON_BUS as 1)
 * This function will setup configuration parameters to I2C controller.
 * If bus is not dedicated to single device only, configuration is performed during call to
 * ad_i2c_bus_acquire().
 *
 * \param [in] dev_id identifier of device connected to I2C bus (name should match entries defined
 *             by I2C_SLAVE_DEVICE or I2C_SLAVE_TO_EXT_MASTER
 *
 * \return device handle that can be used with other functions
 */
i2c_device ad_i2c_open(i2c_device_id dev_id);

/**
 * \brief Close I2C device
 *
 * \param [in] device handle to device opened with ad_i2c_open()
 *
 * \sa ad_i2c_open()
 *
 */
void ad_i2c_close(i2c_device device);

/**
 * \brief Perform typical I2C transaction
 *
 * This function performs most typical transaction on I2C bus.
 * First buffer is sent over I2C bus.
 * Then it changes to read mode and reads data from connected device.
 *
 * This function is blocking. It can wait first for bus access, then it will wait till transaction
 * is completed.
 *
 * Typical usage:
 * \code{.c}
 * {
 *   i2c_device dev = ad_i2c_open(PRESSURE_SENSOR);
 *   while (1) {
 *     // No CS necessary will be activated when needed
 *     ad_i2c_transact(dev, command, sizeof(command), response, sizeof(response));
 *     ...
 *   }
 * }
 * \endcode
 *
 * \param [in] dev handle to I2C device
 * \param [in] wbuf pointer to data to be sent to device
 * \param [in] wlen size of data to be sent to device
 * \param [out] rbuf pointer to data to be read from device
 * \param [in] rlen size of buffer pointed by rbuf
 *
 * \return 0 on success value from HW_I2C_ABORT_SOURCE enum on failure
 *
 * \sa ad_i2c_open()
 * \sa ad_i2c_close()
 *
 */
int ad_i2c_transact(i2c_device dev, const uint8_t *wbuf, size_t wlen, uint8_t *rbuf,
                                                                                size_t rlen);

/**
 * \brief Perform write only transaction
 *
 * This function performs write only transaction on I2C bus.
 *
 * This function is blocking. It can wait first for bus access, then it will wait till transaction
 * is completed.
 *
 * \param [in] dev handle to I2C device
 * \param [in] wbuf pointer to data to be sent to device
 * \param [in] wlen size of data to be sent to device
 *
 * \return 0 on success value from HW_I2C_ABORT_SOURCE enum on failure
 *
 * \sa ad_i2c_open()
 * \sa ad_i2c_close()
 *
 */
int ad_i2c_write(i2c_device dev, const uint8_t *wbuf, size_t wlen);

/**
 * \brief Perform read only transaction
 *
 * This function performs read only transaction on I2C bus.
 *
 * This function is blocking. It can wait first for bus access, then it will wait till transaction
 * is completed.
 *
 * \param [in] dev handle to I2C device
 * \param [out] rbuf pointer to data to be read from device
 * \param [in] rlen size of buffer pointed by rbuf
 *
 * \return 0 on success value from HW_I2C_ABORT_SOURCE enum on failure
 *
 * \sa ad_i2c_open()
 * \sa ad_i2c_close()
 *
 */
int ad_i2c_read(i2c_device dev, uint8_t *rbuf, size_t rlen);

/**
 * \brief Acquire access to I2C bus
 *
 * This function waits for I2C bus to available, and locks it for \p dev's use only.
 * This function can be called several times, but number of ad_i2c_bus_release() calls must match
 * number of calls to this function.
 *
 * This function should be used if normal ad_i2c_transact(), ad_i2c_read(), ad_i2c_write() are not
 * enough and some other I2C controller calls are required. In this case typical usage for this
 * function would look like this:
 *
 * \code{.c}
 * ad_i2c_bus_acquire(dev);
 * id = ad_i2c_get_hw_i2c_id(dev);
 * ...
 * hw_i2c_set...(id, ...);
 * hw_i2c_write_buffer(id, ...)
 * ...
 * ad_i2c_bus_release(dev);
 * \endcode
 *
 * \param [in] dev handle to I2C device
 *
 */
void ad_i2c_bus_acquire(i2c_device dev);

/**
 * \brief Release access to I2C bus
 *
 * This function decrements acquire counter for this device and when it reaches 0 I2C bus is
 * released and can be used by other devices.
 *
 * \param [in] dev handle to I2C device
 *
 * \sa ad_i2c_bus_acquire
 *
 */
void ad_i2c_bus_release(i2c_device dev);

/**
 * \brief Acquire access to I2C device
 *
 * This function waits for device to be available, and locks it for current task use only.
 * This function can be called several times, but number of ad_i2c_device_release() calls must match
 * number of calls to this function.
 *
 * This function should be used if several transaction on this device should not be interrupted.
 * If writing to part of flash memory require reading and erasing, call to ad_i2c_device_acquire()
 * will reserve device for current task not necessarily blocking whole I2C bus.
 *
 * \code{.c}
 * ad_i2c_device_acquire(dev);
 * ...
 * ad_i2c_transact(id, read_page_commad...);
 * ad_i2c_write(id, write_enable_command...);
 * ad_i2c_transact(id, erase_page_command...);
 * ad_i2c_write(id, write_enable_command...);
 * ad_i2c_transact(id, write_page_command...);
 * ...
 * ad_i2c_device_release(dev);
 * \endcode
 *
 * \param [in] dev handle to I2C device
 *
 * \sa ad_i2c_device_release
 * \sa CONFIG_I2C_EXCLUSIVE_OPEN
 *
 */
void ad_i2c_device_acquire(i2c_device dev);

/**
 * \brief Release access to I2C device
 *
 * This function decrements acquire counter for this device and when it reaches 0 device is
 * released and can be used by other tasks.
 *
 * \param [in] dev handle to I2C device
 *
 * \sa ad_i2c_device_acquire
 * \sa CONFIG_I2C_EXCLUSIVE_OPEN
 *
 */
void ad_i2c_device_release(i2c_device dev);

/**
 * \brief Get I2C controller id
 *
 * This function returns id that can be used to get I2C controller id. This id is argument
 * for lower level functions starting with hw_i2c_ prefix.
 *
 * \param [in] dev handle to I2C device
 *
 * \return id that can be used with hw_i2c_... functions
 *
 */
HW_I2C_ID ad_i2c_get_hw_i2c_id(i2c_device dev);

#ifndef I2C_ASYNC_ACTIONS_SIZE
/*
 * 10 elements should be enough for normal read, read/write transactions with one callback
 * 11 is enough for read, read/write with two callbacks
 */
#define I2C_ASYNC_ACTIONS_SIZE 11
#endif

/**
 * \brief I2C data run time data
 *
 * Variables of this type are automatically generated by \sa I2C_BUS() macro.
 * Structure keeps data related to I2C controller that allows easy usage of bus by several
 * tasks and devices connected to I2C controller
 *
 */
typedef struct {
        OS_EVENT event;        /**< Event used for synchronization in accessing I2C controller */
#if !CONFIG_I2C_ONE_DEVICE_ON_BUS
        struct i2c_device_config_t *current_device; /**< This keeps track of last device that was
                                 used on I2C bus. When device changes all controller parameters must
                                 be also changed. If CONFIG_I2C_ONE_DEVICE_ON_BUS is defined as 1
                                 there is no need to check/update controller settings at every
                                 transfer so this field is not needed */
#endif
        uint8_t transaction_ix;
        uint32_t transaction[I2C_ASYNC_ACTIONS_SIZE];
} i2c_bus_dynamic_data;

#define I2C_TAG_CALLBACK0       0xFE000000
#define I2C_TAG_CALLBACK1       0xFD000000
#define I2C_TAG_SEND            0xFC000000
#define I2C_TAG_RECEIVE         0xFB000000
#define I2C_TAG_SEND_STOP       0xFA000000

/*
 * The following macros are used to construct asynchronous transactions on I2C device.
 */

/**
 * \brief Send data and wait until all data are placed in FIFO
 *
 * \sa ad_i2c_async_transact
 */
#define I2C_SND(wbuf, len)      (uint32_t) ((len) | I2C_TAG_SEND), (uint32_t) (wbuf)

/**
 * \brief Send data and wait until STOP condition is detected
 *
 * \sa ad_i2c_async_transact
 */
#define I2C_SND_ST(wbuf, len)   (uint32_t) ((len) | I2C_TAG_SEND_STOP), (uint32_t) (wbuf)

/**
 * \brief Receive data
 *
 * \sa ad_i2c_async_transact
 */
#define I2C_RCV(rbuf, len)      (uint32_t) ((len) | I2C_TAG_RECEIVE), (uint32_t) (rbuf)

/**
 * \brief Callback to call after transaction completes
 *
 * \sa ad_i2c_async_transact
 */
#define I2C_CB(cb)              (uint32_t) I2C_TAG_CALLBACK0, (uint32_t) (cb)

/**
 * \brief Callback with arguments to call after transaction completes
 *
 * \sa ad_i2c_async_transact
 */
#define I2C_CB1(cb, arg)        (uint32_t) I2C_TAG_CALLBACK1, (uint32_t) (cb), (uint32_t) (arg)

/**
 * \brief Mark end of transactions
 *
 * \sa ad_i2c_async_transact
 */
#define I2C_END                 (uint32_t) 0

/**
 * \brief Start asynchronous I2C transaction
 *
 * Arguments are actions that should be taken to perform full transaction.
 *
 * It is important to know that transaction starts from acquiring device and I2C bus and this is
 * done synchronously meaning that function can wait for device and bus access.
 * It is possible to acquire device and bus in advance and then ad_i2c_async_transact() will not
 * block.
 * Buffers passed to this function should not be reused before final callback is called.
 *
 * I2C adapter allows to create asynchronous transaction that consists of a number of reads, writes,
 * and callback calls.
 * This allows to create a time efficient way to manage all I2C related actions. Most of the actions
 * will be executed in interrupt context.
 * These are the possible asynchronous transactions, passed in a form of arguments:
 *
 *  Action                       |  Macro
 * ----------------------------- | ------------------
 *  Send data                    |   I2C_SND
 *  Send data and wait for STOP  |   I2C_SND_ST
 *  Receive data                 |   I2C_RCV
 *  Callback execution           |   I2C_CB, I2C_CB1
 *  Mark end of transactions     |   I2C_END
 *
 * Typical I2C transaction steps:
 * sending command/receiving response
 *
 * Asynchronous I2C transaction allows to build this sequence, start it and wait for final callback
 * after everything is done.
 * Typical usage:
 * \code{.c}
 * {
 *   i2c_device dev = ad_i2c_open(PRESSURE_SENSOR);
 *   while (1) {
 *     ad_i2c_async_transact(dev, I2C_SND(command, sizeof(command)),
 *                                I2C_RCV(response, sizeof(response),
 *                                I2C_CB(final_callback),
 *                                I2C_END);
 *     ...
 *     ad_i2c_async_transact(dev, I2C_SND(command, sizeof(command)),
 *                                I2C_SND_ST(data, sizeof(data),
 *                                I2C_CB(final_callback),
 *                                I2C_END);
 *     // wait here for final callback notification
 *     // do something with response
 *     ...
 *   }
 * }
 * \endcode
 *
 * \param [in] dev handle to I2C device
 *
 */
void ad_i2c_async_transact(i2c_device dev, ...);

/**
 * \brief Start asynchronous write I2C transaction
 *
 * This is convenience macro that builds typical write only transaction and executes it.
 * After all is done and stop condition is detected user callback is executed.
 *
 * \param [in] dev handle to I2C device
 * \param [in] wbuf data to send
 * \param [in] wlen number of bytes to write
 * \param [in] cb callback to call after transaction is over (from interrupt context)
 * \param [in] ud user data to pass to \p cb
 *
 */
#define i2c_async_write(dev, wbuf, wlen, cb, ud) \
        ad_i2c_async_transact(dev, I2C_SND_ST(wbuf, wlen), I2C_CB1(cb, ud), I2C_END)

/**
 * \brief Start asynchronous read I2C transaction
 *
 * This is convenience macro that builds read transaction and executes it.
 * After all is done user callback is executed.
 *
 * \param [in] dev handle to I2C device
 * \param [out] rbuf buffer for incoming data
 * \param [in] rlen number of bytes to read
 * \param [in] cb callback to call after transaction is over (from interrupt context)
 *
 * Typical usage:
 * \code{.c}
 * {
 *   i2c_device dev = ad_i2c_open(PRESSURE_SENSOR);
 *   while (1) {
 *     i2c_async_read(dev, response, sizeof(response), final_callback, cb_arg);
 *
 *     // wait here for final callback notification
 *     // do something with response
 *     ...
 *   }
 * }
 * \endcode
 *
 */
#define i2c_async_read(dev, rbuf, rlen, cb) \
        ad_i2c_async_transact(dev, I2C_RCV(rbuf, rlen), I2C_CB(cb), I2C_END)

/**
 * \brief I2C device data run time data
 *
 * Variables of this type are automatically generated by \sa I2C_SLAVE_DEVICE() and
 * \sa I2C_SLAVE_TO_EXT_MASTER() macros.
 * Structure keeps data related to device connected to I2C controller.
 *
 */
typedef struct {
        int8_t bus_acquire_count; /**< this keeps track of number of calls to ad_i2c_bus_acquire() */

#if !CONFIG_I2C_EXCLUSIVE_OPEN
        int8_t dev_acquire_count; /**< Number of device_acquire calls */
        OS_TASK owner;            /**< Task that acquired this device */
#endif

#if !CONFIG_I2C_RESOURCE_STATIC_ID
        RES_ID dev_res_id;        /**< dynamically created (\sa resource_add()) resource ID for this
                                  device */
#endif
} i2c_dev_dynamic_data;

/**
 * \brief I2C device constant data
 *
 * Variable of this type keeps static configuration needed to access device on I2C bus.
 * Those variables are generated by \sa I2C_SLAVE_DEVICE() and \sa I2C_SLAVE_TO_EXT_MASTER() macros.
 *
 */
typedef struct i2c_device_config_t {
        HW_I2C_ID bus_id;               /**< I2C id as needed by hw_i2c_... functions */
        RES_ID bus_res_id;              /**< I2C resource ID RES_ID_I2C1 or RES_ID_I2C2 */
        int8_t dma_channel;             /**< DMA channel for I2C */
        i2c_config hw_init;             /**< I2C hardware configuration */
        i2c_bus_dynamic_data *bus_data; /**< pointer to dynamic bus data */
        i2c_dev_dynamic_data *data;     /**< pointer to dynamic device data */
#if CONFIG_I2C_RESOURCE_STATIC_ID
        RES_ID dev_res_id;              /**< If CONFIG_I2C_RESOURCE_STATIC_ID is defined, RES_ID contains
                                         value for this device and it can be stored in constant
                                         data instead of dynamic */
#endif
} i2c_device_config;

#ifdef __cplusplus
extern }
#endif

#endif /* dg_configI2C_ADAPTER */

#endif /* AD_I2C_H_ */

/**
 * \}
 * \}
 * \}
 */
