#include <unistd.h>
#include <linux/reboot.h>
#include <sys/reboot.h>

int main() {
    sync();
    reboot(LINUX_REBOOT_CMD_RESTART);
    return 0;
}
