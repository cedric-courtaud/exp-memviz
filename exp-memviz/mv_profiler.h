#ifndef __MV_PROFILER_H__
#define __MV_PROFILER_H__

#include "pub_tool_basics.h"
#include "pub_tool_libcprint.h"
#include "pub_tool_xarray.h"

#define MV_PROFILER_DEFAULT_BUFFER_SIZE (16 << 10)

#define CHECKPOINT_PROFILER_BEGIN "memviz_begin"
#define CHECKPOINT_PROFILER_END   "memviz_end"

#define MV_ACCESS_D_READ   1
#define MV_ACCESS_D_WRITE  2
#define MV_ACCESS_I_READ   4

typedef UChar access_flags_t ;

typedef struct access {
    Addr inst_addr;
    Addr dest_addr;
    access_flags_t flags;
    ULong instruction_since_last_access;
} Access_t;

typedef struct checkpoint {
    HChar * id;
    ULong pos;
    ULong instruction_since_last_access;
} Checkpoint_t;

typedef struct {
    Access_t * access_buffer;
    UInt buffer_size;
    UInt buffer_capacity;
    ULong inst_since_last_access;
    Addr current_instruction_addr;
    access_flags_t current_flags;
    VgFile * out_fp;
} Profiler_t;

void dump_access_buffer(Profiler_t * t);

static inline void set_current_instruction_addr(Profiler_t * profiler, Addr addr) {
    profiler->current_instruction_addr = addr;
}

static inline void set_current_flags(Profiler_t * profiler, access_flags_t flags) {
    profiler->current_flags = flags;
}

static inline void record_local_instruction(Profiler_t * profiler) {
    profiler->inst_since_last_access++;
}

static inline void __record_access(Profiler_t * profiler, Addr inst_addr, Addr dest_addr, access_flags_t flags) {
    // __update_last_access_instruction_count(profiler);

    if (profiler->buffer_size == profiler->buffer_capacity) {
        dump_access_buffer(profiler);
        profiler->buffer_size = 0;
    }

    Access_t * access = &profiler->access_buffer[profiler->buffer_size];
    profiler->buffer_size++;

    access->inst_addr = inst_addr;
    access->dest_addr = dest_addr;
    access->flags = flags;
    access->instruction_since_last_access = profiler->inst_since_last_access;
    profiler->inst_since_last_access = 0;
}

static inline void record_access(Profiler_t * profiler, Addr dest_addr) {
    __record_access(profiler, profiler->current_instruction_addr, dest_addr, profiler->current_flags);
}

static inline void record_checkpoint(Profiler_t * profiler, const HChar * checkpoint, ThreadId tid) {
    if (profiler->buffer_size > 0) {
        dump_access_buffer(profiler);
    }

    VG_(fprintf)(profiler->out_fp, "C %s %llu\n", checkpoint, profiler->inst_since_last_access);
}

void profiler_init(Profiler_t * profiler, UInt buffer_size, HChar * out_filename);
void profiler_close(Profiler_t * profiler);

#endif //__MV_PROFILER_H__