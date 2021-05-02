#ifndef ROS_KUTILS_HPP
#define ROS_KUTILS_HPP

#include "stdint.hpp"
#include "stdlib.hpp"

namespace impl {
    constexpr uint64_t kalloc_begin_addr = 0x100000;
    constexpr uint64_t kalloc_end_addr = 0x200000;
    // Currently 1MB, must be paged. 

    struct __attribute__((packed)) free_slot_header_t {
        uint32_t size;
        uint64_t next; // 0x0 means tail.
    };
    static_assert(sizeof(free_slot_header_t) == sizeof(uint32_t) + sizeof(uint64_t));

    using allocated_slot_header_t = uint32_t; // contains size EXCEPT the head!

    inline void kalloc_init_pool() {
        // Just create a single huge slot, which contains the whole pool. 
        struct __attribute__((packed)) _kalloc_head {
            uint32_t magic = 0x12230823;
            free_slot_header_t free_slot_head {kalloc_end_addr - kalloc_begin_addr - sizeof(_kalloc_head), 0x0};
        } kalloc_head;
        static_assert(sizeof(kalloc_head) == sizeof(uint32_t)*2 + sizeof(uint64_t));

        memcpy((char *)kalloc_begin_addr, (const char *)&kalloc_head, sizeof(kalloc_head));
    }
}

// naive kalloc. We keep a list of free slot. 
// The return value is NULL on failure, and is a valid pointer `p` on success. 
// ((allocated_slot_header_t *)p)[-1] contains the header, which records the allocated size. 
// and ((char *)p)[0:requested_size] is available for use. 
inline void *kalloc_impl(const uint32_t requested_size) {
    const auto req_size_with_head = requested_size + sizeof(impl::allocated_slot_header_t);
    uint64_t prev_slot_addr = 0x0;
    // Locate first free slot. 
    auto curr_slot_addr = impl::kalloc_begin_addr + sizeof(impl::free_slot_header_t::size);
    while(true) {
        // Iterate all free slots. It's slow, but good enough for a toy. 
        auto curr_freeslot_header_ptr = (impl::free_slot_header_t *)curr_slot_addr;
        if(curr_freeslot_header_ptr->size >= req_size_with_head) {
            // Good. We can do the allocate now! 
            decltype(impl::free_slot_header_t::size) actually_assigned_size;
            decltype(curr_slot_addr) allocated_slot_addr;
            if(curr_freeslot_header_ptr->size < 32 + req_size_with_head) {
                // Give the current slot. 
                if(prev_slot_addr == 0x0) {
                    // The first slot must be a valid free_slot_head. It's required 
                    //   to locate the next free_slot. 
                    curr_freeslot_header_ptr->size = 0;
                    // Is this case, the space of free_slot_head is not available.
                    actually_assigned_size = curr_freeslot_header_ptr->size;
                    allocated_slot_addr = curr_slot_addr + sizeof(impl::free_slot_header_t);
                }
                else {
                    // Eliminate the current free_slot_head. 
                    curr_freeslot_header_ptr->size -= req_size_with_head;
                    auto prev_slot = (impl::free_slot_header_t *)prev_slot_addr;
                    prev_slot->next = curr_freeslot_header_ptr->next;
                    // Is this case, the space of free_slot_head is available.
                    actually_assigned_size = curr_freeslot_header_ptr->size + sizeof(impl::free_slot_header_t);
                    allocated_slot_addr = curr_slot_addr;
                }
            }
            else {
                // The slot is too large. Shrink the free_slot, and give the tail space as result.
                curr_freeslot_header_ptr->size -= req_size_with_head;
                actually_assigned_size = req_size_with_head;
                allocated_slot_addr = curr_slot_addr + sizeof(impl::free_slot_header_t) + curr_freeslot_header_ptr->size;
            }
            // Create the allocated_slot_head structure. 
            *(impl::allocated_slot_header_t *)allocated_slot_addr = actually_assigned_size - sizeof(impl::allocated_slot_header_t);
            return (char *)allocated_slot_addr + sizeof(impl::allocated_slot_header_t);
        }
        else if (curr_freeslot_header_ptr->next != 0x0){
            // Failed. Try next slot.
            prev_slot_addr = curr_slot_addr;
            curr_slot_addr = curr_freeslot_header_ptr->next;
            continue;
        }
        else {
            // We're already at the end. Return NO_MEMORY. 
            return NULL;
        }
    }
}

inline void kfree(void *memory) {

}

template <typename T>
struct kallocator {
    using value_type = T;
    kallocator() noexcept {}
    template <typename AnyType> kallocator (const kallocator &) noexcept {}
    template <typename AnyType> kallocator (kallocator &&) noexcept {}

    T *allocate(uint32_t count) {
        return (T*)kalloc_impl(sizeof(T) * count);
    }

    void deallocate (T* p, uint32_t) {
        kfree(p);
    }
};

#endif

