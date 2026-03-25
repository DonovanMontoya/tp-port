/**
 * src/JSystem/JKernel/JKRDisposer.cpp
 * PC port stub for JKRDisposer.
 */
#include "JSystem/JKernel/JKRDisposer.h"
#include "JSystem/JKernel/JKRHeap.h"

JKRDisposer::JKRDisposer()
    : mHeap(JKRHeap::sCurrentHeap), mLink(this)
{
    // Register with the owning heap's disposer list if it has one
    // (on PC we skip the actual list management since JKRHeap is a stub)
}

JKRDisposer::~JKRDisposer() {
    // On GC, this removes the object from the heap's disposer list.
    // On PC, JSUPtrLink's destructor handles removal automatically.
}
