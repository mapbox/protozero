#ifndef PROTOZERO_BUFFER_HPP
#define PROTOZERO_BUFFER_HPP

/*****************************************************************************

protozero - Minimalistic protocol buffer decoder and encoder in C++.

This file is from https://github.com/mapbox/protozero where you can find more
documentation.

*****************************************************************************/

/**
 * @file buffer.hpp
 *
 * @brief Contains the customization points for buffer implementations
 */

#include <protozero/config.hpp>

#include <cstddef>

namespace protozero {

/**
 * Get the number of bytes currently used in the buffer.
 *
 * @param buffer Pointer to the buffer.
 * @returns number of bytes used in the buffer.
 */
template <typename TBuffer>
std::size_t buffer_size(const TBuffer* buffer) noexcept {
    return buffer->size();
}

/**
 * Append count bytes from data to the buffer.
 *
 * @param buffer Pointer to the buffer.
 * @param data Pointer to the data.
 * @param count Number of bytes to be added to the buffer.
 */
template <typename TBuffer>
void buffer_append(TBuffer* buffer, const char* data, std::size_t count) {
    buffer->append(data, count);
}

/**
 * Append count zero bytes to the buffer.
 *
 * @param buffer Pointer to the buffer.
 * @param count Number of bytes to be added to the buffer.
 */
template <typename TBuffer>
void buffer_append_zeros(TBuffer* buffer, std::size_t count) {
    buffer->append_zeros(count);
}

/**
 * Shrink the buffer to the specified size. The new size will always be smaller
 * than the current size.
 *
 * @param buffer Pointer to the buffer.
 * @param size New size of the buffer.
 *
 * @pre size < current size of buffer
 */
template <typename TBuffer>
void buffer_resize(TBuffer* buffer, std::size_t size) {
    protozero_assert(size < buffer_size(buffer));
    buffer->resize(size);
}

/**
 * Reserve an additional size bytes for use in the buffer. This is used for
 * variable-sized buffers to tell the buffer implementation that soon more
 * memory will be used. The implementation can ignore this.
 *
 * @param buffer Pointer to the buffer.
 * @param size Number of bytes to reserve.
 */
template <typename TBuffer>
void buffer_reserve_additional(TBuffer* buffer, std::size_t size) {
    buffer->reserve_additional(size);
}

/**
 * Delete data from the buffer. This must move back the data after the
 * part being deleted and resize the buffer accordingly.
 *
 * @param buffer Pointer to the buffer.
 * @param from Offset into the buffer where we want to erase from.
 * @param to Offset into the buffer one past the last byte we want to erase.
 *
 * @pre from, to <= size of the buffer, from < to
 */
template <typename TBuffer>
void buffer_erase_range(TBuffer* buffer, std::size_t from, std::size_t to) {
    buffer->erase_range(from, to);
}

/**
 * Return a pointer to the memory at the specified position in the buffer.
 *
 * @param buffer Pointer to the buffer.
 * @param pos The position in the buffer.
 * @returns pointer to the memory in the buffer at the specified position.
 *
 * @pre pos <= size of the buffer
 */
template <typename TBuffer>
char* buffer_at_pos(TBuffer* buffer, std::size_t pos) {
    return buffer->at_pos(pos);
}

} // namespace protozero

#endif // PROTOZERO_BUFFER_HPP
