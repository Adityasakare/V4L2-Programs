/*
    Video for Linux Version 2 (V4L2)
    * A program which shows user V4L2 device capabilites. 

*/

#include <errno.h>
#include <fcntl.h>
#include <linux/videodev2.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>


static const char DEVICE[] = "/dev/video0";

int 
print_capabilities(int fd)
{
    int ret;
    struct v4l2_capability caps = {0};

    ret = ioctl(fd, VIDIOC_QUERYCAP, &caps);
    if(-1 == ret)
    {
        perror("Querying device capabilities");
        return errno;
    }

    printf("\nDriver Caps: \n"
            "   Driver: \"%s\"\n"
            "   Card: \"%s\"\n"
            "   Bus: \"%s\"\n"
            "   Version: %u.%u.%u\n"
            "   Capabilities: %08x\n",
            caps.driver,
            caps.card,
            caps.bus_info,
            (caps.version >> 16) & 0xFF,
            (caps.version >> 8) & 0xFF,
            (caps.version) & 0xFF,
            caps.capabilities);

        return 0;
}


int print_crop_capabilities(int fd)
{
    int ret;
    struct v4l2_cropcap cropcaps = {0};

    cropcaps.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(fd, VIDIOC_CROPCAP, &cropcaps);
    if(-1 == ret)
    {
        perror("Querying device crop capabilities");
        return errno;
    }
    
    printf("\nCamera Cropping:\n"
        "  Bounds: %dx%d+%d+%d\n"
        "  Default: %dx%d+%d+%d\n"
        "  Aspect: %d/%d\n",
        cropcaps.bounds.width, cropcaps.bounds.height, cropcaps.bounds.left, cropcaps.bounds.top,
        cropcaps.defrect.width, cropcaps.defrect.height,
        cropcaps.defrect.left, cropcaps.defrect.top,
        cropcaps.pixelaspect.numerator, cropcaps.pixelaspect.denominator);  
        
        
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

    print_capabilities(fd);
    print_crop_capabilities(fd);

    close(fd);
    return 0;
}
