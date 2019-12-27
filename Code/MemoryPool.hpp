#ifndef MEMORY_POOL_HPP
#define MEMORY_POOL_HPP

#include <climits>
#include <cstddef>

template <typename T, size_t BlockSize = 4096>
class MemoryPool
{
  public:
    typedef T*              pointer;

    template <typename U> struct rebind {
      typedef MemoryPool<U> other;
    };

    MemoryPool() noexcept {
      currentBlock_ = nullptr;
      currentSlot_ = nullptr;
      lastSlot_ = nullptr;
      freeSlots_ = nullptr;
    }

    ~MemoryPool() noexcept {
      slot_pointer_ curr = currentBlock_;
      while (curr != nullptr) {
        slot_pointer_ prev = curr->next;
        operator delete(reinterpret_cast<void*>(curr));
        curr = prev;
      }
    }

    pointer allocate(size_t n = 1, const T* hint = 0) {
      if (freeSlots_ != nullptr) {
        pointer result = reinterpret_cast<pointer>(freeSlots_);
        freeSlots_ = freeSlots_->next;
        return result;
      }
      else {
        if (currentSlot_ >= lastSlot_) {
          data_pointer_ newBlock = reinterpret_cast<data_pointer_>(operator new(BlockSize));
          reinterpret_cast<slot_pointer_>(newBlock)->next = currentBlock_;
          currentBlock_ = reinterpret_cast<slot_pointer_>(newBlock);
          data_pointer_ body = newBlock + sizeof(slot_pointer_);
          uintptr_t result = reinterpret_cast<uintptr_t>(body);
          size_t bodyPadding = (alignof(slot_type_) - result) % alignof(slot_type_);
          currentSlot_ = reinterpret_cast<slot_pointer_>(body + bodyPadding);
          lastSlot_ = reinterpret_cast<slot_pointer_>(newBlock + BlockSize - sizeof(slot_type_));
        }
        return reinterpret_cast<pointer>(currentSlot_++);
      }
    }

    void deallocate(pointer p, size_t n = 1) {
      if (p != nullptr) {
        reinterpret_cast<slot_pointer_>(p)->next = freeSlots_;
        freeSlots_ = reinterpret_cast<slot_pointer_>(p);
      }
    }

    template <typename U, typename... Args>
    void construct(U* p, Args&&... args) {
      new (p) U (std::forward<Args>(args)...);
    }

    template <typename U>
    void destroy(U* p) {
      p->~U();
    }

  private:
    union Slot_ {
      T element;
      Slot_* next;
    };

    typedef char* data_pointer_;
    typedef Slot_ slot_type_;
    typedef Slot_* slot_pointer_;

    slot_pointer_ currentBlock_;
    slot_pointer_ currentSlot_;
    slot_pointer_ lastSlot_;
    slot_pointer_ freeSlots_;
    static_assert(BlockSize >= 2 * sizeof(slot_type_), "BlockSize too small.");
};

#endif
