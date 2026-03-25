/**
 * src/JSystem/JSupport/JSUList.cpp
 * PC port implementation of JSUPtrList / JSUPtrLink.
 */
#include "JSystem/JSupport/JSUList.h"
#include <cassert>

// -----------------------------------------------------------------------
// JSUPtrLink
// -----------------------------------------------------------------------
void JSUPtrLink::remove() {
    if (!mList) return;
    mList->remove(this);
}

// -----------------------------------------------------------------------
// JSUPtrList
// -----------------------------------------------------------------------
void JSUPtrList::append(JSUPtrLink* link) {
    assert(link && !link->mList);
    link->mList = this;
    link->mPrev = mTail;
    link->mNext = nullptr;
    if (mTail) mTail->mNext = link;
    else       mHead        = link;
    mTail = link;
    mCount++;
}

void JSUPtrList::prepend(JSUPtrLink* link) {
    assert(link && !link->mList);
    link->mList = this;
    link->mNext = mHead;
    link->mPrev = nullptr;
    if (mHead) mHead->mPrev = link;
    else       mTail        = link;
    mHead = link;
    mCount++;
}

void JSUPtrList::insert(JSUPtrLink* prev, JSUPtrLink* link) {
    if (!prev)         { prepend(link); return; }
    if (!prev->mNext)  { append(link);  return; }
    assert(link && !link->mList);
    link->mList       = this;
    link->mPrev       = prev;
    link->mNext       = prev->mNext;
    prev->mNext->mPrev = link;
    prev->mNext        = link;
    mCount++;
}

bool JSUPtrList::remove(JSUPtrLink* link) {
    if (!link || link->mList != this) return false;
    if (link->mPrev) link->mPrev->mNext = link->mNext;
    else             mHead              = link->mNext;
    if (link->mNext) link->mNext->mPrev = link->mPrev;
    else             mTail              = link->mPrev;
    link->mList = nullptr;
    link->mPrev = nullptr;
    link->mNext = nullptr;
    mCount--;
    return true;
}
