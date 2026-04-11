#pragma once
/**
 * port/include/d/d_model.h
 * PC port stub — game model classes.
 * Only referenced inside #if DEBUG blocks; forward declarations sufficient.
 */
#ifndef D_D_MODEL_H
#define D_D_MODEL_H

class dMdl_obj_c {
public:
    char _pad[0x34];
};

class dMdl_c {
public:
    char _pad[0x20];
};

class dMdl_mng_c {
public:
    static void create() {}
    static void reset() {}
    static void remove() {}
};

#endif /* D_D_MODEL_H */
