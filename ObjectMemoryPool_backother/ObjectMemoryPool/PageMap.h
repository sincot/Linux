#pragma once

#include "Common.h"
#include "ObjectPool.h"


// 使用基数树来优化

// Single-level array   一层的基数树
// 在 32 位平台下，BITS = 32 - PAGE_SHIFT     2^32 / 2^13 = 2^19  array_ 的大小      使用的就是一层基数树
template <int BITS>
class TCMalloc_PageMap1 {
private:
    static const int LENGTH = 1 << BITS;
    void** array_;      // 占用内存 2^19 * 4 = 2^21

public:
    typedef uintptr_t Number;

    /*explicit TCMalloc_PageMap1(void* (allocator)(size_t)) {
        array_ = reinterpret_cast<void**>((*allocator)(sizeof(void*) << BITS));
        memset(array_, 0, sizeof(void*) << BITS);
    }*/

    explicit TCMalloc_PageMap1() {
        size_t size = sizeof(void*) << BITS;    
        size_t alignSize = SizeClass::_RoundUp(size, 1 << PAGE_SHIFT);    // 计算对齐数
        array_ = (void**)SystemAlloc(alignSize >> PAGE_SHIFT);
        memset(array_, 0, sizeof(void*) << BITS);
    }

    // Return the current value for KEY. Returns NULL if not yet set,
    // or if k is out of range.
    void* get(Number k) const {
        if ((k >> BITS) > 0) {
            return NULL;
        }
        return array_[k];
    }

    // REQUIRES "k" is in range "[0,2^BITS-1]".
    // REQUIRES "k" has been ensured before.
    // Sets the value 'v' for key 'k'.
    void set(Number k, void* v)
    {
        if ((k >> BITS) > 0) { assert(false); return; }
        array_[k] = v;
    }

    void erase(Number k)
    {
        if ((k >> BITS) > 0) {  return;  }  // 越界，直接返回
        array_[k] = nullptr;  // 将对应位置置空
    }
};

// Two-level radix tree     两层基数树
// 在 64 位平台下，BITS = 64 - PAGE_SHIFT     2^64 / 2^13 = 2^51
// 在 32 位平台下，BITS = 32 - PAGE_SHIFT     2^32 / 2^13 = 2^19  
template <int BITS>
class TCMalloc_PageMap2 {
private:
    // Put 32 entries in the root and (2^BITS)/32 entries in each leaf.
    static const int ROOT_BITS = 5;     
    static const int ROOT_LENGTH = 1 << ROOT_BITS;      // 第一层只有 2^5 = 32 个槽位

    static const int LEAF_BITS = BITS - ROOT_BITS;      // 19 - 5 = 14
    static const int LEAF_LENGTH = 1 << LEAF_BITS;      // 第二层有 2^14 个槽位

    // 怎么计算位置？高13位全是0，低19位存储页号（低19位的前5位，决定在第一层的第几个槽位，后14位决定在第二层的第几个槽位）

    // Leaf node
    struct Leaf {
        void* values[LEAF_LENGTH];
    };

    Leaf* root_[ROOT_LENGTH];              // Pointers to 32 child nodes
    // void* (*allocator_)(size_t);           // Memory allocator

public:
    typedef uintptr_t Number;

    /*explicit TCMalloc_PageMap2(void* (*allocator)(size_t)) {
        allocator_ = allocator;
        memset(root_, 0, sizeof(root_));
    }*/

    explicit TCMalloc_PageMap2() {
        memset(root_, 0, sizeof(root_));
    }

    void* get(Number k) const {
        /*if ((k >> BITS) > 0) {
            printf("ERROR: PageMap2::get out of range! k=%llu\n", (unsigned long long)k);
            return NULL;
        }*/
        const Number i1 = k >> LEAF_BITS;
        const Number i2 = k & (LEAF_LENGTH - 1);
        if ((k >> BITS) > 0 || root_[i1] == NULL) {
            return NULL;
        }
        // if (root_[i1] == NULL) return NULL;
        return root_[i1]->values[i2];
    }

    void set(Number k, void* v) {
        const Number i1 = k >> LEAF_BITS;
        const Number i2 = k & (LEAF_LENGTH - 1);
        assert(i1 < ROOT_LENGTH);
        
        Ensure(k, 1);
        root_[i1]->values[i2] = v;
    }

    void erase(Number k) {
        if ((k >> BITS) > 0) return;
        const Number i1 = k >> LEAF_BITS;
        if (root_[i1] == NULL) return;
        const Number i2 = k & (LEAF_LENGTH - 1);
        root_[i1]->values[i2] = nullptr;
    }

    //bool Ensure(Number start, size_t n) {
    //    for (Number key = start; key <= start + n - 1;) {
    //        const Number i1 = key >> LEAF_BITS;

    //        // Check for overflow
    //        if (i1 >= ROOT_LENGTH)
    //            return false;

    //        // Make 2nd level node if necessary
    //        if (root_[i1] == NULL) {
    //            Leaf* leaf = reinterpret_cast<Leaf*>((*allocator_)(sizeof(Leaf)));
    //            if (leaf == NULL) return false;
    //            memset(leaf, 0, sizeof(*leaf));
    //            root_[i1] = leaf;
    //        }

    //        // Advance key past whatever is covered by this leaf node
    //        key = ((key >> LEAF_BITS) + 1) << LEAF_BITS;
    //    }
    //    return true;
    //}

    bool Ensure(Number start, size_t n) {
        for (Number key = start; key <= start + n - 1;) {
            const Number i1 = key >> LEAF_BITS;

            // Check for overflow
            if (i1 >= ROOT_LENGTH)
                return false;

            // Make 2nd level node if necessary
            if (root_[i1] == NULL) {
                static ObjectPool<Leaf> LeafPool;
                Leaf* leaf = (Leaf*)LeafPool.New();
                //Leaf* leaf = new Leaf();

                // Leaf* leaf = reinterpret_cast<Leaf*>((*allocator_)(sizeof(Leaf)));
                memset(leaf, 0, sizeof(*leaf));
                root_[i1] = leaf;
            }

            // Advance key past whatever is covered by this leaf node
            key = ((key >> LEAF_BITS) + 1) << LEAF_BITS;
        }
        return true;
    }

    void PreallocateMoreMemory() {
        // Allocate enough to keep track of all possible pages
        Ensure(0, 1 << BITS);
    }
};

// Three-level radix tree   三层基数树   64位平台下选择
template <int BITS>
class TCMalloc_PageMap3 {
private:
    // How many bits should we consume at each interior level
    static const int INTERIOR_BITS = (BITS + 2) / 3; // Round-up
    static const int INTERIOR_LENGTH = 1 << INTERIOR_BITS;

    // How many bits should we consume at leaf level
    static const int LEAF_BITS = BITS - 2 * INTERIOR_BITS;
    static const int LEAF_LENGTH = 1 << LEAF_BITS;

    // Interior node
    struct Node {
        void* ptrs[INTERIOR_LENGTH];
        // Node* ptrs[INTERIOR_LENGTH];
    };

    // Leaf node
    struct Leaf {
        void* values[LEAF_LENGTH];
    };

    Node* root_;                            // Root of radix tree
    // void* (*allocator_)(size_t);             // Memory allocator

public:
    typedef uintptr_t Number;

    explicit TCMalloc_PageMap3() {
        size_t size = sizeof(Node);
        size_t pages = (size + (1 << PAGE_SHIFT) - 1) >> PAGE_SHIFT;
        root_ = (Node*)SystemAlloc(pages);
        memset(root_, 0, size);
    }

    /*explicit TCMalloc_PageMap3(void* (*allocator)(size_t)) {
        allocator_ = allocator;
        root_ = NewNode();
    }*/

    void* get(Number k) const {
        const Number i1 = k >> (LEAF_BITS + INTERIOR_BITS);
        const Number i2 = (k >> LEAF_BITS) & (INTERIOR_LENGTH - 1);
        const Number i3 = k & (LEAF_LENGTH - 1);
        if ((k >> BITS) > 0 ||
            root_->ptrs[i1] == NULL ||
            reinterpret_cast<Node*>(root_->ptrs[i1])->ptrs[i2] == NULL) {  
            return NULL;
        }

        return reinterpret_cast<Leaf*>(reinterpret_cast<Node*>(root_->ptrs[i1])->ptrs[i2])->values[i3];  

        // return reinterpret_cast<Leaf*>(root_->ptrs[i1]->ptrs[i2])->values[i3];
    }

    void set(Number k, void* v) {
        assert(k >> BITS == 0);
        Ensure(k, 1);
        const Number i1 = k >> (LEAF_BITS + INTERIOR_BITS);
        const Number i2 = (k >> LEAF_BITS) & (INTERIOR_LENGTH - 1);
        const Number i3 = k & (LEAF_LENGTH - 1);
        // reinterpret_cast<Leaf*>(root_->ptrs[i1]->ptrs[i2])->values[i3] = v;
        reinterpret_cast<Leaf*>(reinterpret_cast<Node*>(root_->ptrs[i1])->ptrs[i2])->values[i3] = v;
    }

    void erase(Number k) {
        if ((k >> BITS) > 0) return;
        const Number i1 = k >> (LEAF_BITS + INTERIOR_BITS);
        const Number i2 = (k >> LEAF_BITS) & (INTERIOR_LENGTH - 1);
        const Number i3 = k & (LEAF_LENGTH - 1);
        if (root_->ptrs[i1] == NULL) return;
        if (((Node*)root_->ptrs[i1])->ptrs[i2] == NULL) return;
        // ((Leaf*)((Node*)root_->ptrs[i1])->ptrs[i2])->values[i3] = nullptr;
        reinterpret_cast<Leaf*>(((Node*)root_->ptrs[i1])->ptrs[i2])->values[i3] = nullptr;
    }

    //bool Ensure(Number start, size_t n) {
    //    for (Number key = start; key <= start + n - 1;) {
    //        const Number i1 = key >> (LEAF_BITS + INTERIOR_BITS);
    //        const Number i2 = (key >> LEAF_BITS) & (INTERIOR_LENGTH - 1);

    //        // Check for overflow
    //        if (i1 >= INTERIOR_LENGTH || i2 >= INTERIOR_LENGTH)
    //            return false;

    //        // Make 2nd level node if necessary
    //        if (root_->ptrs[i1] == NULL) {
    //            Node* n = NewNode();
    //            if (n == NULL) return false;
    //            root_->ptrs[i1] = n;
    //        }

    //        // Make leaf node if necessary
    //        if (root_->ptrs[i1]->ptrs[i2] == NULL) {
    //            Leaf* leaf = reinterpret_cast<Leaf*>((*allocator_)(sizeof(Leaf)));
    //            if (leaf == NULL) return false;
    //            memset(leaf, 0, sizeof(*leaf));
    //            root_->ptrs[i1]->ptrs[i2] = reinterpret_cast<Node*>(leaf);
    //        }

    //        // Advance key past whatever is covered by this leaf node
    //        key = ((key >> LEAF_BITS) + 1) << LEAF_BITS;
    //    }
    //    return true;
    //}

    bool Ensure(Number start, size_t n) {
        for (Number key = start; key <= start + n - 1;) {
            const Number i1 = key >> (LEAF_BITS + INTERIOR_BITS);
            const Number i2 = (key >> LEAF_BITS) & (INTERIOR_LENGTH - 1);

            if (i1 >= INTERIOR_LENGTH || i2 >= INTERIOR_LENGTH)
                return false;

            if (root_->ptrs[i1] == NULL) {
                static ObjectPool<Node> _nodePool;
                Node* node = _nodePool.New();
                //Node* node = new Node();  // 临时用 new
                if (node == NULL) return false;
                memset(node, 0, sizeof(Node));
                root_->ptrs[i1] = node;
            }

            if (((Node*)root_->ptrs[i1])->ptrs[i2] == NULL) {
                static ObjectPool<Leaf> _leafPool;
                Leaf* leaf = _leafPool.New();
                // Leaf* leaf = new Leaf();  // 临时用 new
                if (leaf == NULL) return false;
                memset(leaf, 0, sizeof(Leaf));
                // ((Node*)root_->ptrs[i1])->ptrs[i2] = (void*)leaf;
                ((Node*)root_->ptrs[i1])->ptrs[i2] = reinterpret_cast<void*>(leaf);
            }

            key = ((key >> LEAF_BITS) + 1) << LEAF_BITS;
        }
        return true;
    }

    void PreallocateMoreMemory() {
        Ensure(0, 1 << BITS);
    }
};


// 一层基数树的优势：访问内存简单，页号是多少，就到基数树对应的位置查找
//void* get(Number k) const {
//    if ((k >> BITS) > 0) {
//        return NULL;
//    }
//    return array_[k];
//}

// 两层基数树的优势：
//void* get(Number k) const {
//    const Number i1 = k >> LEAF_BITS;
//    const Number i2 = k & (LEAF_LENGTH - 1);
//    if ((k >> BITS) > 0 || root_[i1] == NULL) {
//        return NULL;
//    }
//    return root_[i1]->values[i2];
//}