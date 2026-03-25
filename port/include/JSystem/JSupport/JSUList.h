/**
 * port/include/JSystem/JSupport/JSUList.h
 * PC port stub — replaces GC JSUList with std::list-based equivalents.
 */
#pragma once
#include "port/types.h"
#include <cstddef>

// -----------------------------------------------------------------------
// JSUPtrList / JSUPtrLink  (type-erased doubly-linked list)
// -----------------------------------------------------------------------
class JSUPtrList;

class JSUPtrLink {
public:
    explicit JSUPtrLink(void* object) : mObject(object), mList(nullptr), mNext(nullptr), mPrev(nullptr) {}
    ~JSUPtrLink() { if (mList) remove(); }

    void*        getObjectPtr() const { return mObject; }
    JSUPtrList*  getList()      const { return mList; }
    JSUPtrLink*  getNext()      const { return mNext; }
    JSUPtrLink*  getPrev()      const { return mPrev; }

private:
    void remove();
    void* mObject;
    JSUPtrList* mList;
    JSUPtrLink* mNext;
    JSUPtrLink* mPrev;
    friend class JSUPtrList;
};

class JSUPtrList {
public:
    JSUPtrList(bool initLinks = false) : mHead(nullptr), mTail(nullptr), mCount(0) { (void)initLinks; }
    ~JSUPtrList() {}

    void     append(JSUPtrLink* link);
    void     prepend(JSUPtrLink* link);
    void     insert(JSUPtrLink* prev, JSUPtrLink* link);
    bool     remove(JSUPtrLink* link);

    JSUPtrLink* getFirst() const { return mHead; }
    JSUPtrLink* getLast()  const { return mTail; }
    u32          getNumLinks() const { return mCount; }
    bool         isEmpty()     const { return mCount == 0; }

private:
    JSUPtrLink* mHead;
    JSUPtrLink* mTail;
    u32         mCount;
};

// -----------------------------------------------------------------------
// JSUList<T> / JSULink<T>  (typed wrappers)
// -----------------------------------------------------------------------
template<typename T>
class JSUList : public JSUPtrList {
public:
    JSUList(bool initLinks = false) : JSUPtrList(initLinks) {}
};

template<typename T>
class JSULink : public JSUPtrLink {
public:
    explicit JSULink(T* obj) : JSUPtrLink(static_cast<void*>(obj)) {}
    T* getObject() const { return static_cast<T*>(getObjectPtr()); }
};

// -----------------------------------------------------------------------
// JSUListIterator<T>
// -----------------------------------------------------------------------
template<typename T>
class JSUListIterator {
public:
    JSUListIterator() : mLink(nullptr) {}
    explicit JSUListIterator(JSULink<T>* link) : mLink(link) {}

    T*  getObject()  const { return mLink ? mLink->getObject() : nullptr; }
    bool isEnd()     const { return mLink == nullptr; }
    JSUListIterator& operator++() {
        if (mLink) mLink = static_cast<JSULink<T>*>(mLink->getNext());
        return *this;
    }
    T* operator->() const { return getObject(); }
    T& operator*()  const { return *getObject(); }

private:
    JSULink<T>* mLink;
};
