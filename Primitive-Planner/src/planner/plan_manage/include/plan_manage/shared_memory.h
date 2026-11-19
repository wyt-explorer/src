/**
 * @file shared_memory.h
 * 文件作用：封装 System V 共享内存的创建、挂接、分离和删除。
 * 调用关系：PPReplanFSM 在 USE_SHARED_MEMORY 启用时调用这些辅助函数。
 * 输入：共享内存大小、段 ID；默认通过 /etc/apt/sources.list 和项目字符生成键。
 * 输出：共享内存段 ID 或进程地址；创建、挂接等关键调用失败时退出进程。
 * 运行模式：同一 Linux 主机上的进程间通信，不负责轨迹序列化或同步。
 * 主要依赖：sys/ipc.h、sys/shm.h、C++ 标准库、traj_utils。
 */
#include <sys/ipc.h>
#include <sys/shm.h>
#include <vector>
#include <utility>
#include <iostream>
#include <cstring> // 用于memset

#include <traj_utils/plan_container.hpp>

// 附加共享内存到进程地址空间
void* attach_shared_memory(int shm_id) {
    void* shared_memory = shmat(shm_id, nullptr, 0);
    if (shared_memory == (void*)-1) {
        perror("shmat");
        exit(1);
    }
    return shared_memory;
}

// 从进程地址空间分离共享内存
void detach_shared_memory(void* shared_memory) {
    if (shmdt(shared_memory) == -1) {
        perror("shmdt");
        exit(1);
    }
}

// 删除共享内存
void remove_shared_memory(int shm_id) {
    // 检查共享内存是否存在
    struct shmid_ds shminfo;
    if (shmctl(shm_id, IPC_STAT, &shminfo) == -1) {
        if (errno == EIDRM) {
            std::cerr << "Shared memory already removed or never existed." << std::endl;
        } else {
            std::cerr << "Error checking shared memory status: " << strerror(errno) << std::endl;
        }
        return;
    }

    // 确认共享内存存在后，执行删除操作
    if (shmctl(shm_id, IPC_RMID, nullptr) == -1) {
        std::cerr << "Failed to remove shared memory: " << strerror(errno) << std::endl;
    } else {
        std::cout << "Shared memory removed successfully." << std::endl;
    }
}

int get_shared_memory_key(const char* filename, char proj_id) {
    key_t key = ftok(filename, proj_id);
    if (key == -1) {
        perror("ftok");
        exit(1);
    }
    return key;
}

int get_or_create_shared_memory(size_t size) {
    int key = get_shared_memory_key("/etc/apt/sources.list", 'a');
    int shm_id = shmget(key, 0, 0666);
    if (shm_id == -1) {
        // 如果共享内存不存在，创建它
        shm_id = shmget(key, size, IPC_CREAT | 0666);
        if (shm_id < 0) {
            perror("shmget");
            exit(1);
        }
    }
    return shm_id;
}
