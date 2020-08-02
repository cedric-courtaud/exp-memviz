#include "mv_profiler.h"
#include "pub_tool_mallocfree.h"
#include "pub_tool_libcprint.h"
#include "pub_tool_libcassert.h"
#include "pub_tool_vki.h"
#include "pub_tool_libcproc.h"
#include "pub_tool_options.h"

static Profiler_t * the_profiler;

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


static void profiler_at_fork(ThreadId tid) {
    Profiler_t * p = the_profiler;

    VG_(fclose)(p->out_fp);

    HChar * expanded_out_filename = VG_(expand_file_name)("--memviz-out-file", p->out_filename);
    p->out_fp = VG_(fopen)(expanded_out_filename, VKI_O_CREAT | VKI_O_WRONLY, VKI_S_IRUSR | VKI_S_IWUSR);
    if (p->out_fp == NULL) {
        VG_(tool_panic)("Error when opening file");
    }

    Int pid = VG_(getpid)();
    Int ppid = VG_(getppid)();
    ULong pac = p->access_count;

    VG_(fprintf)(p->out_fp, "F %d %d %llu\n", pid, ppid, pac);

    p->access_count = 0;
    p->buffer_size = 0;
}

void profiler_init(Profiler_t * profiler, UInt buffer_size, HChar * out_filename) {
    profiler->buffer_capacity = buffer_size;
    profiler->access_buffer = VG_(malloc)("Memviz profiler buffer allocation", profiler->buffer_capacity * sizeof(*profiler->access_buffer));
    profiler->buffer_size = 0;
    profiler->inst_since_last_access = 0;
    profiler->access_count = 0;
    profiler->current_flags = 0;
    profiler->out_filename = out_filename;
    the_profiler = profiler;

    HChar * expanded_out_filename = VG_(expand_file_name)("--memviz-out-file", profiler->out_filename);
    profiler->out_fp = VG_(fopen)(expanded_out_filename, VKI_O_CREAT | VKI_O_WRONLY, VKI_S_IRUSR | VKI_S_IWUSR);
    if (profiler->out_fp == NULL) {
        VG_(tool_panic)("Error when opening file");
    }

    VG_(atfork)(NULL, NULL, profiler_at_fork);
    
    record_checkpoint(profiler, CHECKPOINT_PROFILER_BEGIN, 0);
}

void profiler_close(Profiler_t * profiler) {
    record_checkpoint(profiler, CHECKPOINT_PROFILER_END, 0);
    VG_(fclose)(profiler->out_fp);
}