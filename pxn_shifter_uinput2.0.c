#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <linux/input.h>
#include <linux/uinput.h>

#define REAL_EVENT "/dev/input/by-id/usb-PXN_PXN-SF_U-event-if00"
#define NUM_BTNS 8

static void send_syn(int fd)
{
    struct input_event syn;
    memset(&syn, 0, sizeof(syn));
    syn.type = EV_SYN;
    syn.code = SYN_REPORT;
    syn.value = 0;
    write(fd, &syn, sizeof(syn));
}

int main(void)
{
    int fd_real, fd_ui;
    struct input_event ev, out;
    struct uinput_user_dev uidev;

    int current_gear = -1;

    fd_real = open(REAL_EVENT, O_RDONLY);
    if (fd_real < 0) {
        perror("open real device");
        return 1;
    }

    fd_ui = open("/dev/uinput", O_WRONLY | O_NONBLOCK);
    if (fd_ui < 0) {
        perror("open uinput");
        return 1;
    }

    /* joystick capabilities */
    ioctl(fd_ui, UI_SET_EVBIT, EV_KEY);
    ioctl(fd_ui, UI_SET_EVBIT, EV_ABS);
    ioctl(fd_ui, UI_SET_EVBIT, EV_SYN);

    for (int i = 0; i < NUM_BTNS; i++)
        ioctl(fd_ui, UI_SET_KEYBIT, BTN_JOYSTICK + i);

    ioctl(fd_ui, UI_SET_ABSBIT, ABS_X);

    memset(&uidev, 0, sizeof(uidev));
    snprintf(uidev.name, UINPUT_MAX_NAME_SIZE, "PXN-SF U Shifter");
    uidev.id.bustype = BUS_USB;
    uidev.id.vendor  = 0x1234;
    uidev.id.product = 0x5678;

    uidev.absmin[ABS_X] = -1;
    uidev.absmax[ABS_X] = 1;

    write(fd_ui, &uidev, sizeof(uidev));
    ioctl(fd_ui, UI_DEV_CREATE);

    /* eixo neutro */
    out.type = EV_ABS;
    out.code = ABS_X;
    out.value = 0;
    write(fd_ui, &out, sizeof(out));
    send_syn(fd_ui);

    while (1) {

        if (read(fd_real, &ev, sizeof(ev)) != sizeof(ev))
            continue;

        if (ev.type == EV_KEY) {

            int new_gear = -1;

            /* Ré */
            if (ev.code == 266 && ev.value)
                new_gear = 7;

            /* Marchas 1..6 */
            if (ev.code >= BTN_0 && ev.code <= BTN_6 && ev.value)
                new_gear = ev.code - BTN_0;

            /* troca de marcha exclusiva */
            if (new_gear != -1 && new_gear != current_gear) {

                /* solta marcha anterior */
                if (current_gear != -1) {
                    out.type  = EV_KEY;
                    out.code  = BTN_JOYSTICK + current_gear;
                    out.value = 0;
                    write(fd_ui, &out, sizeof(out));
                }

                /* ativa nova marcha */
                out.type  = EV_KEY;
                out.code  = BTN_JOYSTICK + new_gear;
                out.value = 1;
                write(fd_ui, &out, sizeof(out));

                send_syn(fd_ui);

                current_gear = new_gear;
            }

            /* soltou marcha atual */
            if (ev.value == 0 && current_gear != -1) {
                out.type  = EV_KEY;
                out.code  = BTN_JOYSTICK + current_gear;
                out.value = 0;
                write(fd_ui, &out, sizeof(out));
                send_syn(fd_ui);

                current_gear = -1;
            }
        }
    }

    ioctl(fd_ui, UI_DEV_DESTROY);
    close(fd_ui);
    close(fd_real);

    return 0;
}
