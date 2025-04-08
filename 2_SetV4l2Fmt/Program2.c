/*
    Video for Linux Version 2 (V4L2)
    * A program which shows user V4L2 device formats desc. and set last 
        video format

*/

#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>


static const char DEVICE[] = "/dev/video0";

int
print_and_set_format(int fd)
{
    // These will correspond to the flgas V4L2+FMT_FLAG_COMPRESSED 
    // and V4L2_FMT_FLAG_EMULATED
    char c, e;

    struct v4l2_fmtdesc fmtdesc = {0};
    fmtdesc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    while(0 == ioctl(fd, VIDIOC_ENUM_FMT, &fmtdesc))
    {
        c = fmtdesc.flags & 1 ? 'C' : ' ';
        e = fmtdesc.flags & 2  ? 'E' : ' ';

        printf("%c%c %s\n", c, e, fmtdesc.description);
        fmtdesc.index++;
    }   

    printf("\n Using format: %s\n", fmtdesc.description);

    // Set the device format
    struct v4l2_format fmt = {0};
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

    fmt.fmt.pix.width = 320;
    fmt.fmt.pix.height = 240;
    fmt.fmt.pix.pixelformat = fmtdesc.pixelformat; // Last discovered pxl fmt
    fmt.fmt.pix.field = V4L2_FIELD_NONE;

    if(-1 == ioctl(fd, VIDIOC_S_FMT, &fmt))
    {
        perror("Could not set format description");
        return -1;
    }

    char format_code[5];
    strncpy(format_code, (char*)&fmt.fmt.pix.pixelformat, 5);
    printf("Set format:\n"
        " Width: %d\n"
        " Height: %d\n"
        " Pixel format: %s\n"
        " Field: %d\n",
        fmt.fmt.pix.width,
        fmt.fmt.pix.height,
        format_code,
        fmt.fmt.pix.field);

        return 0;

}

int main(void)
{
    int fd;

    fd = open(DEVICE, O_RDWR);
    if(fd < 0)
    {
        perror(DEVICE);
        return errno;
    }

    print_and_set_format(fd);

    close(fd);
    return 0;
}
