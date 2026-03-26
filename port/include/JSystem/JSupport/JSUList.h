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

    JSUPtrLink* getFirst()     const { return mHead; }
    JSUPtrLink* getLast()      const { return mTail; }
    JSUPtrLink* getFirstLink() const { return mHead; }
    JSUPtrLink* getLastLink()  const { return mTail; }
    JSUPtrLink* getNthLink(u32 index) const;
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
class JSULink;

template<typename T>
class JSUList : public JSUPtrList {
public:
    JSUList(bool initLinks = false) : JSUPtrList(initLinks) {}
    JSULink<T>* getNth(u32 index) const { return static_cast<JSULink<T>*>(getNthLink(index)); }
};

template<typename T>
class JSULink : public JSUPtrLink {
public:
    explicit JSULink(T* obj) : JSUPtrLink(static_cast<void*>(obj)) {}
    T* getObject() const { return static_cast<T*>(getObjectPtr()); }
    JSULink<T>* getNextLink() const { return static_cast<JSULink<T>*>(getNext()); }
    JSULink<T>* getPrevLink() const { return static_cast<JSULink<T>*>(getPrev()); }
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

// -----------------------------------------------------------------------
// JSUTree<T> — parent-child tree node (each node is both a list and a link)
// -----------------------------------------------------------------------
template<typename T>
class JSUTree : public JSUList<T>, public JSULink<T> {
public:
    explicit JSUTree(T* owner) : JSUList<T>(), JSULink<T>(owner) {}
    ~JSUTree() {}

    bool appendChild(JSUTree<T>* child)                         { this->append(child); return child != nullptr; }
    bool prependChild(JSUTree<T>* child)                        { this->prepend(child); return child != nullptr; }
    bool removeChild(JSUTree<T>* child)                         { return this->remove(child); }
    bool insertChild(JSUTree<T>* /*before*/, JSUTree<T>* child) { this->append(child); return child != nullptr; }

    JSUTree<T>* getEndChild()   const { return nullptr; }
    JSUTree<T>* getFirstChild() const { return static_cast<JSUTree<T>*>(this->getFirstLink()); }
    JSUTree<T>* getLastChild()  const { return static_cast<JSUTree<T>*>(this->getLastLink()); }
    JSUTree<T>* getNextChild()  const { return static_cast<JSUTree<T>*>(this->getNext()); }
    JSUTree<T>* getPrevChild()  const { return static_cast<JSUTree<T>*>(this->getPrev()); }
    u32         getNumChildren() const { return this->getNumLinks(); }
    T*          getObject()      const { return static_cast<T*>(this->getObjectPtr()); }
    JSUTree<T>* getParent()      const { return static_cast<JSUTree<T>*>(this->getList()); }
};

// -----------------------------------------------------------------------
// JSUTreeIterator<T>
// -----------------------------------------------------------------------
template<typename T>
class JSUTreeIterator {
public:
    JSUTreeIterator() : mTree(nullptr) {}
    explicit JSUTreeIterator(JSUTree<T>* tree) : mTree(tree) {}

    T* getObject() const { return mTree ? mTree->getObject() : nullptr; }
    bool operator==(const JSUTree<T>* o) const { return mTree == o; }
    bool operator!=(const JSUTree<T>* o) const { return mTree != o; }
    JSUTreeIterator<T>  operator++(int) { JSUTreeIterator<T> prev = *this; if (mTree) mTree = mTree->getNextChild(); return prev; }
    JSUTreeIterator<T>& operator++()    { if (mTree) mTree = mTree->getNextChild(); return *this; }
    T*  operator*()  const { return getObject(); }
    T*  operator->() const { return getObject(); }
private:
    JSUTree<T>* mTree;
};

inline JSUPtrLink* JSUPtrList::getNthLink(u32 index) const {
    JSUPtrLink* link = mHead;
    while (link != nullptr && index-- > 0) {
        link = link->getNext();
    }
    return link;
}
