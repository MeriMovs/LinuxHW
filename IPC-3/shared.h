#ifndef SHARED_ARRAY_H
#define SHARED_ARRAY_H

#include <sys/mman.h>
// #include "shared.cpp"
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdexcept>

class shared_array {
    std::string shm_name;
    std::string sem_name;
    int fd;
    int *ptr;
    size_t count;
    size_t byte_size;
    sem_t *sem;

public:
    shared_array(const std::string &name, size_t size_elements);

    ~shared_array();

    int& operator[](size_t i);

    size_t size() const;

    sem_t* get_semaphore();
};

#endif


shared_array::~shared_array(){
    if(ptr) munmap(ptr, byte_size);
    if(fd != -1) close(fd);
    if(sem) sem_close(sem);
    shm_unlink(shm_name.c_str());
}

int& shared_array::operator[](size_t i){
    if(i >= count) throw std::out_of_range("index");
    return ptr[i];
}

size_t shared_array::size() const { return count; }

shared_array::shared_array(const std::string &name, size_t size_elements){
    fd = -1;
    ptr = nullptr;
    count = size_elements;
    sem = nullptr;

    if (size_elements > 1000000000 || size_elements < 1) {
        throw std::runtime_error("wrong size");
    }
    shm_name = "/" + name;
    sem_name = "/sem_" + name;

    byte_size = size_elements * sizeof(int);

    fd = shm_open(shm_name.c_str(), O_RDWR | O_CREAT | O_EXCL, 0664);
    if(fd != -1) {
        if(ftruncate(fd, byte_size) == -1) {
            close(fd);
            shm_unlink(shm_name.c_str());
            throw std::runtime_error("failed");
        }
    } else {
        fd = shm_open(shm_name.c_str(), O_RDWR, 0664);
        if(fd == -1) throw std::runtime_error("failed");
    }
    void *map = mmap(0, byte_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(map == MAP_FAILED) {
        close(fd);
        shm_unlink(shm_name.c_str());
        throw std::runtime_error("failed");
    }

    ptr = (int*)map;

    sem = sem_open(sem_name.c_str(), O_CREAT, 0664, 1);
    if(sem == SEM_FAILED) {
        munmap(ptr, byte_size);
        close(fd);
        throw std::runtime_error("failed");
    }
}

sem_t* shared_array::get_semaphore() { return sem; }