#ifndef VOYAGER_H
#define VOYAGER_H
#include <stdint.h>
#include <stdlib.h>

typedef enum {
  VOYAGER_ERROR_NONE = 0,
  VOYAGER_ERROR_INVALID_ARGUMENT,
} voyager_error_E;

typedef enum {
  VOYAGER_STATE_IDLE = 0,
  VOYAGER_STATE_DFU_RECEIVE,
  VOYAGER_STATE_FLASH_VERIFY,
} voyager_bootloader_state_E;

// Note: if A/B partitioning is used later on, these keys will require separate
// values for each partition Propose modifying the NVM function signatures to
// specify the partition number and this partition number is required to be
// passed in to the bootloader
typedef enum {
  VOYAGER_NVM_KEY_CRC = 0,
  VOYAGER_NVM_KEY_APP_START_ADDRESS,
  VOYAGER_NVM_KEY_APP_END_ADDRESS,
  VOYAGER_NVM_KEY_APP_SIZE,
  VOYAGER_NVM_KEY_VERIFY_FLASH_BEFORE_JUMPING,
} voyager_nvm_key_E;

// Define a custom typedef for a voyager_bootloader_nvm_data member to be a
// uint32_t
typedef uint32_t voyager_bootloader_nvm_data_t;

/** Primary Bootloader Functions **/
/**
 * @brief voyager_bootloader_init Resets and initializes the bootloader.
 * @return VOYAGER_ERROR_NONE if successful, otherwise an error code
 *
 * @note should be called on startup before any other bootloader functions
 */
voyager_error_E voyager_bootloader_init(void);

/**
 * @brief voyager_bootloader_run Runs the bootloader
 * @return VOYAGER_ERROR_NONE if successful, otherwise an error code
 *
 * @note should be called on startup before any other bootloader functions
 */
voyager_error_E voyager_bootloader_run(void);

/**
 * @brief voyager_bootloader_process_receieved_packet Processes a packet
 * received from the host
 * @param data The data received from the host
 * @param length The length of the data received from the host
 * @return VOYAGER_ERROR_NONE if successful, otherwise an error code
 *
 * @note should be called when a packet from the DFU host device is received
 * with the relevant data
 * @note this function does perform error correction or verification - it
 * assumes the data link layer has already done this and the data is physically
 * valid.
 */
voyager_error_E
voyager_bootloader_process_receieved_packet(uint8_t const *const data,
                                            size_t const length);

/**
 * @brief voyager_bootloader_get_state Gets the current state of the bootloader
 * @return The current state of the bootloader
 */
voyager_bootloader_state_E voyager_bootloader_get_state(void);

/** User Implemented Functions **/

/**
 * @brief voyager_bootloader_nvm_write Writes a value to a given key in NVM
 * @param key The key to write to
 * @param data The data to write to the key
 * @return VOYAGER_ERROR_NONE if successful, otherwise an error code
 *
 * @note This function is called by the bootloader and is required to be
 * implemented by the application.
 */
voyager_error_E
voyager_bootloader_nvm_write(voyager_nvm_key_E key,
                             voyager_bootloader_nvm_data_t const *const data);

/**
 * @brief voyager_bootloader_nvm_read Reads a value from a given key in NVM
 * @param key The key to read from
 * @param data The data to read from the key
 * @return VOYAGER_ERROR_NONE if successful, otherwise an error code
 *
 * @note This function is called by the bootloader and is required to be
 * implemented by the application.
 */
voyager_error_E
voyager_bootloader_nvm_read(voyager_nvm_key_E key,
                            voyager_bootloader_nvm_data_t *const data);

/**
 * @brief voyager_bootloader_hal_erase_flash Erases the flash memory of the MCU
 * @param start_address The start address of the flash memory to erase
 * @param end_address The end address of the flash memory to erase
 * @return VOYAGER_ERROR_NONE if successful, otherwise an error code
 *
 * @note This function is called by the bootloader and is required to be
 * implemented by the application
 */
voyager_error_E
voyager_bootloader_hal_erase_flash(voyager_bootloader_nvm_data_t start_address,
                                   voyager_bootloader_nvm_data_t end_address);

/**
 * @brief voyager_bootloader_hal_write_flash Writes data to the flash memory of
 * the MCU
 * @param address The address of the flash memory to write to
 * @param data The data to write to the flash memory
 * @param length The length of the data to write to the flash memory
 * @return VOYAGER_ERROR_NONE if successful, otherwise an error code
 *
 * @note This function is called by the bootloader and is required to be
 * implemented by the application
 */
voyager_error_E
voyager_bootloader_hal_write_flash(voyager_bootloader_nvm_data_t address,
                                   uint8_t const *const data,
                                   size_t const length);

/**
 * @brief voyager_bootloader_hal_read_flash Reads data from the flash memory of
 * the MCU
 * @param address The address of the flash memory to read from
 * @param data The data to read from the flash memory
 * @param length The length of the data to read from the flash memory
 * @return VOYAGER_ERROR_NONE if successful, otherwise an error code
 *
 * @note This function is called by the bootloader and is required to be
 * implemented by the application
 */
voyager_error_E
voyager_bootloader_hal_read_flash(voyager_bootloader_nvm_data_t address,
                                  uint8_t *const data, size_t const length);

#endif // VOYAGER_H
