#include <iostream>
#include <signal.h>
#include <pwd.h>

void handler(int sig, siginfo_t* info, void* context) {

    struct passwd *pw = getpwuid(info->si_uid);
    const char* username = pw ? pw->pw_name : "unknown";

    std::string res = "Received a SIGUSR1 signal from process [" + std::to_string(info->si_pid) + "] executed by [" 
              + std::to_string(info->si_uid) + "] (" + username + ").\n";

    ucontext_t *uc = (ucontext_t*)context;
    unsigned long eip = uc->uc_mcontext.gregs[REG_RIP];
    unsigned long eax = uc->uc_mcontext.gregs[REG_RAX];
    unsigned long ebx = uc->uc_mcontext.gregs[REG_RBX];

    res += "State of the context: EIP = [" + std::to_string(eip) + "], EAX = [" + std::to_string(eax) + "], EBX = [" 
            + std::to_string(ebx) + "]. \n";

    write(1, res.c_str(), res.size());
}

int main() {
    struct sigaction sa {};
    sa.sa_sigaction = handler;
    sa.sa_flags = SA_SIGINFO;
    sigemptyset(&sa.sa_mask);

    sigaction(SIGUSR1, &sa, nullptr);

    raise(SIGUSR1);
    
    while (true) { sleep(10); }

    return 0;
}