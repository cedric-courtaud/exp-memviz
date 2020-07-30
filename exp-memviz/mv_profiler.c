#include "mv_profiler.h"
#include "pub_tool_mallocfree.h"
#include "pub_tool_libcprint.h"
#include "pub_tool_libcassert.h"
#include "pub_tool_vki.h"

static inline void dump_access(Profiler_t * profiler, Access_t * access) {
    switch (access->flags) {
        case MV_ACCESS_D_READ:
            VG_(fprintf)(profiler->out_fp, "R 0x%lx 0x%lx %llu\n", access->inst_addr, access->dest_addr, access->instruction_since_last_access);
            break;
        case MV_ACCESS_D_WRITE:
            VG_(fprintf)(profiler->out_fp, "W 0x%lx 0x%lx %llu\n", access->inst_addr, access->dest_addr, access->instruction_since_last_access);
            break;
        case MV_ACCESS_I_READ:
            VG_(fprintf)(profiler->out_fp, "I 0x%lx 0x%lx %llu\n", access->inst_addr, access->dest_addr, access->instruction_since_last_access);
            break;
        default: break;
    }
}

void dump_access_buffer(Profiler_t * profiler) {
    for (int i = 0;i < profiler->buffer_size; i++) {
        dump_access(profiler, &profiler->access_buffer[i]);
    }    
}

void profiler_init(Profiler_t * profiler, UInt buffer_size, HChar * out_filename) {
    profiler->buffer_capacity = buffer_size;
    profiler->access_buffer = VG_(malloc)("Memviz profiler buffer allocation", profiler->buffer_capacity * sizeof(*profiler->access_buffer));
    profiler->buffer_size = 0;
    profiler->inst_since_last_access = 0;
    profiler->current_flags = 0;

    profiler->out_fp = VG_(fopen)(out_filename, VKI_O_CREAT | VKI_O_WRONLY, VKI_S_IRUSR | VKI_S_IWUSR);
    if (profiler->out_fp == NULL) {
        VG_(tool_panic)("Error when opening file");
    }
    
    record_checkpoint(profiler, CHECKPOINT_PROFILER_BEGIN, 0);
}

void profiler_close(Profiler_t * profiler) {
    record_checkpoint(profiler, CHECKPOINT_PROFILER_END, 0);
    VG_(fclose)(profiler->out_fp);
}