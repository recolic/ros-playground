#ifndef ROS_KUTILS_HPP
#define ROS_KUTILS_HPP

#include "stdint.hpp"
#include "stdlib.hpp"

namespace impl {
    constexpr uint64_t kalloc_begin_addr = 0x100000;
    constexpr uint64_t kalloc_end_addr = 0x200000;
    // Currently 1MB, must be paged. 

    struct __attribute__((packed)) free_slot_head_t {
        uint32_t size;
        uint64_t next; // 0x0 means tail.
    };
    static_assert(sizeof(free_slot_head_t) == sizeof(uint32_t) + sizeof(uint64_t));

    using allocated_slot_head_t = uint32_t; // contains size EXCEPT the head!

    inline void kalloc_init_pool() {
        struct __attribute__((packed)) _kalloc_head {
            uint32_t magic = 0x12230823;
            free_slot_head_t free_slot_head {kalloc_end_addr - kalloc_begin_addr - sizeof(_kalloc_head), 0x0};
        } kalloc_head;
        static_assert(sizeof(kalloc_head) == sizeof(uint32_t)*2 + sizeof(uint64_t));

        memcpy((char *)kalloc_begin_addr, (const char *)&kalloc_head, sizeof(kalloc_head));
    }
}

// naive kalloc
inline void *kalloc_impl(const uint32_t requested_size) {
    const auto req_size_with_head = requested_size + sizeof(impl::allocated_slot_head_t);
    uint64_t prev_slot_addr = 0x0;
    auto curr_slot_addr = impl::kalloc_begin_addr + sizeof(impl::free_slot_head_t::size);
    while(true) {
        auto curr_slot = (impl::free_slot_head_t *)curr_slot_addr;
        if(curr_slot->size >= req_size_with_head) {
            if(curr_slot->size < 32 + req_size_with_head) {
                // Give the whole slot
                if(prev_slot_addr == 0x0) {
                    // No previous slot. I am the first one.
                    // Never give the WHOLE first slot. Try again. 
                    goto _kalloc_failed_try_again;
                }
                // Do the alloc
                else {
                    curr_slot->size -= req_size_with_head;
                    auto prev_slot = (impl::free_slot_head_t *)prev_slot_addr;
                    prev_slot->next = curr_slot->next;
                }
                const auto actually_assigned_size = sizeof(impl::free_slot_head_t) + curr_slot->size;
                *(impl::allocated_slot_head_t *)curr_slot_addr = actually_assigned_size;
                return (char *)curr_slot_addr + sizeof(impl::allocated_slot_head_t);
            }
            else {
                // Shrink the slot.
                // Do the alloc
                // TODO
                curr_slot->size -= req_size_with_head;
                return (char *)curr_slot_addr + curr_slot->size;
            }
        }
        else if (curr_slot->next != 0x0){
            // Failed. Next.
_kalloc_failed_try_again:
            prev_slot_addr = curr_slot_addr;
            curr_slot_addr = curr_slot->next;
            continue;
        }
        else {
            // No memory
            return 0x0;
        }
    }
}


inline void kfree(void *memory) {

}

#endif

