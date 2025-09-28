/*!
 * \file    lib/ENode.h
 * \date    2025-09-22
 * \author  elijw
 * \license MIT
 *
 * \brief   node for a singly linked list.
 *
 * \note    very simple POD-like struct; no dynamic allocation here pog
 */

#pragma once

/*!
 * \brief    node for a singly linked list.
 *
 * \tparam T type of value stored in the node.
 *
 * \note     this is a template so the linked list can store any type.
 */
template <typename T>
struct ENode {
    T value;        //!< the stored value.
    ENode* next;    //!< pointer to the next node in the list.

    /*!
     * \brief   construct a new node.
     *
     * \param   v value to store.
     *
     * \note    uses explicit to prevent accidental implicit conversions.
     */
    explicit ENode(const T& v) : value(v), next(nullptr) {}
};

