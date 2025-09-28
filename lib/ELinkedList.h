/*!
 * \file    lib/ELinkedList.h
 * \date    2025-09-22
 * \author  elijw
 * \license MIT
 *
 * \brief   super simple singly linked list.
 */

#pragma once
#include "ENode.h"

/*!
 * \brief    singly linked list implementation.
 *
 * \tparam T type of values stored in the list.
 *
 * \note     designed for EHash chaining; no size caching or iterator support.
 */
template<typename T> class ELinkedList
{
  public:
    ENode<T>* head = nullptr; //!< pointer to the first node.

    /*!
     * \brief   insert a new value at the head.
     *
     * \param   value - the value to insert.
     *
     * \note    O(1) insertion; allocates a new ENode on the heap.
     */
    void insert(const T& value)
    {
        ENode<T>* node = new ENode<T>(value);
        node->next = head;
        head = node;
    }

    /*!
     * \brief   find first element matching a predicate.
     *
     * \tparam  Pred predicate type: bool(const T&)
     * \param   pred function/lambda to test elements.
     *
     * \return  pointer to value if found, nullptr otherwise.
     *
     * \note    returns pointer so EHash can update values directly.
     */
    template<typename Pred> T* find(Pred pred)
    {
        ENode<T>* cur = head;
        while (cur)
        {
            if (pred(cur->value)) return &cur->value;
            cur = cur->next;
        }
        return nullptr;
    }

    /*!
     * \brief   remove first element matching a predicate.
     *
     * \tparam  Pred predicate type: bool(const T&)
     * \param   pred function/lambda to test elements.
     *
     * \return  true if element was removed, false otherwise.
     *
     * \note    carefully updates pointers; deletes node to avoid leaks.
     */
    template<typename Pred> bool remove(Pred pred)
    {
        ENode<T>* cur = head;
        ENode<T>* prev = nullptr;
        while (cur)
        {
            if (pred(cur->value))
            {
                if (prev) prev->next = cur->next;
                else head = cur->next;
                delete cur;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }
};

;
