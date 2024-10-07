# Device files
- Provides an abstraction for the programmer
## /dev abstraction
- Some devices is only accesible using device files, being accessible from its corresponding device file.
- This allows the operating syten and user-space programs to communicate with HW at a higher level. The different devices are:
1. Block Devices
   - `/dev/sda`, `/dev/sdb` (Storage devices)
   - `/dev/sda1`, `/dev/sda2` (Partitions)

2. Character Devices
   - `/dev/tty` (Terminals)
   - `/dev/null` (Data sink)
   - `/dev/random`, `/dev/urandom` (Random data generator)

3. Virtual Devices
   - `/dev/loop0` (Loopback device)
   - `/dev/fuse` (FUSE interface)

4. Network Interfaces
   - `/dev/net/tun` (TUN interface)
   - `/dev/net/tap` (TAP interface)

5. Miscellaneous Devices
   - `/dev/dsp` (Digital sound device)
   - `/dev/snd/*` (ALSA sound devices)

6. Pseudodevices
   - `/dev/null` (Data sink)
   - `/dev/zero` (Null byte generator)
   - `/dev/full` (Disk full error)

7. Input/Output Devices
   - `/dev/input/mouse0` (Mouse device)
   - `/dev/input/event0` (Input events)

8. Disk and Tape Drives
   - `/dev/sr0` (CD/DVD drive)
   - `/dev/st0` (Tape drive)

9. Special Memory Devices
   - `/dev/mem` (Physical memory access)
   - `/dev/kmem` (Kernel memory access)

## fb
- Access graphics through well-defined interface
- Framebuffer is a memory device (memory-mapped) hust as /dev/mem and has the same features, can be:
    - Written to
    - Read from
    - Seek to some locatin in it
    - mmap() it (the main usage)
- Allows several ioctls on it (where the color comes in). This also allows request for:
    - information about unchangable information ab. HW, name, adress and length off screen memory
    - information about variable infrmation visible and virtual geometry, depth, color, etc.
    - get and set parts of a color map (16 bit depth)

## fb API
- The API that applications have to use in order to interact with the frame buffer
### Capabilities
- To get the capabilities of a spesific framebuffer, can be found in the fb_fix_screeninfo strucure. Here the field informs the application about what features are supported by the specific device.
- FB_CAP_FOURCC (four character code) is one example of a capability supported by some device drivers
### Types and visuals
- A macropixel (or makropixel) refers to a group of data that represents a single pixel on a screen, which may consist of multiple components like red, green, and blue (RGB) values, and possibly a transparency (alpha) value.
- Formats are given by frame buffer types and visuals. Some visuals require additional information, which are stored in the variable screen information bits_per_pixel, grayscale, red, green, blue and transp fields. Visuals describe how color information is encoded and assembled to create macropixels. Types describe how macropixels are stored in memory. The following types and visuals are supported.

    - **FB_TYPE_PACKED_PIXELS**  
      Macropixels are stored contiguously in a single plane. Padding to the lines may be present and is reported through the fixed screen information `line_length` field.
    
    - **FB_TYPE_PLANES**  
      Macropixels are split across multiple planes. The number of planes is equal to the number of bits per macropixel, with plane i’th storing i’th bit from all macropixels.  
      Planes are located contiguously in memory.
    
    - **FB_TYPE_INTERLEAVED_PLANES**  
      Macropixels are split across multiple planes. The number of planes is equal to the number of bits per macropixel, with plane i’th storing i’th bit from all macropixels.  
      Planes are interleaved in memory. The interleave factor, defined as the distance in bytes between the beginning of two consecutive interleaved blocks belonging to different planes, is stored in the fixed screen information `type_aux` field.

    - **FB_TYPE_FOURCC**  
      Macropixels are stored in memory as described by the format FOURCC identifier stored in the variable screen information `grayscale` field.

    - **FB_VISUAL_MONO01**  
      Pixels are black or white and stored on a number of bits (typically one) specified by the variable screen information `bpp` field.  
      Black pixels are represented by all bits set to 1 and white pixels by all bits set to 0.  
      When the number of bits per pixel is smaller than 8, several pixels are packed together in a byte.  
      **FB_VISUAL_MONO01** is currently used with **FB_TYPE_PACKED_PIXELS** only.

    - **FB_VISUAL_MONO10**  
      Pixels are black or white and stored on a number of bits (typically one) specified by the variable screen information `bpp` field.  
      Black pixels are represented by all bits set to 0 and white pixels by all bits set to 1.  
      When the number of bits per pixel is smaller than 8, several pixels are packed together in a byte.  
      **FB_VISUAL_MONO10** is currently used with **FB_TYPE_PACKED_PIXELS** only.

    - **FB_VISUAL_TRUECOLOR**  
      Pixels are broken into red, green, and blue components, and each component indexes a read-only lookup table for the corresponding value.  
      Lookup tables are device-dependent and provide linear or non-linear ramps.  
      Each component is stored in a macropixel according to the variable screen information `red`, `green`, `blue`, and `transp` fields.

    - **FB_VISUAL_PSEUDOCOLOR** and **FB_VISUAL_STATIC_PSEUDOCOLOR**  
      Pixel values are encoded as indices into a colormap that stores red, green, and blue components.  
      The colormap is read-only for **FB_VISUAL_STATIC_PSEUDOCOLOR** and read-write for **FB_VISUAL_PSEUDOCOLOR**.  
      Each pixel value is stored in the number of bits reported by the variable screen information `bits_per_pixel` field.

    - **FB_VISUAL_DIRECTCOLOR**  
      Pixels are broken into red, green, and blue components, and each component indexes a programmable lookup table for the corresponding value.  
      Each component is stored in a macropixel according to the variable screen information `red`, `green`, `blue`, and `transp` fields.

    - **FB_VISUAL_FOURCC**  
      Pixels are encoded and interpreted as described by the format FOURCC identifier stored in the variable screen information `grayscale` field.

### Screen information
Can be fetched by applications using FBIOGET_FSCREENINFO or FBIOGET_VSCREENINFO ioctls. These takes pointers to a fb_fix_screeninfo and fb_var_screeninfo structure, respectively. The fb_fix_screeninfo structure contains information about the fixed capabilities of the framebuffer device, while the fb_var_screeninfo structure contains information about the current state of the framebuffer device. Modifiactions of the variable screen information can be done by the application using FBIOPUT_VSCREENINFO ioctl with a pointer to a fb_var_screeninfo structure. Instead of filling the complete fb_var_screeninfo structure manually, applications should call the FBIOGET_VSCREENINFO ioctl and modify only the fields they care about.

### Format configuration
### Using the Legacy Frame Buffer API

The legacy API is the recommended approach for applications when dealing with RGB, grayscale, and legacy non-standard formats. Here's a simple guide on how to use it:

1. **Selecting Frame Buffer Format**
   - Applications can select a format by setting the `fb_var_screeninfo` struct’s `bits_per_pixel` field to the desired frame buffer depth.
   - Values up to 8 usually correspond to monochrome, grayscale, or pseudocolor visuals, though this is not required.

2. **Grayscale Formats**
   - To use grayscale formats:
     - Set the `grayscale` field to 1.
     - Set the `red`, `blue`, `green`, and `transp` fields to 0.
     - Drivers will automatically fill in the offsets for `red`, `blue`, and `green` to 0, and the lengths to match the `bits_per_pixel` value.

3. **Pseudocolor Formats**
   - For pseudocolor formats:
     - Set the `grayscale` field to 0.
     - Set the `red`, `blue`, `green`, and `transp` fields to 0.
     - Drivers will also set the offsets for `red`, `blue`, and `green` to 0, and lengths to match the `bits_per_pixel` value.

4. **Truecolor and Directcolor Formats**
   - To configure truecolor or directcolor formats:
     - Set the `grayscale` field to 0.
     - Define the layout of color components using the `red`, `green`, `blue`, and `transp` fields.
     - Each of these fields is a `struct fb_bitfield`, defined as:
     ```c
     struct fb_bitfield {
         __u32 offset;     /* Beginning of the bitfield */
         __u32 length;     /* Length of the bitfield */
         __u32 msb_right;  /* != 0 if most significant bit is on the right */
     };
     ```

5. **Handling Pixel Components**
   - Pixel values are `bits_per_pixel` wide and are split into non-overlapping red, green, blue, and alpha components.
   - The location and size of each component are specified by the `fb_bitfield`'s `offset` and `length` fields. Offsets are calculated from the right.
   
6. **Pixel Storage**
   - Pixels are stored in an integer number of bytes.
   - If the number of bits per pixel is not a multiple of 8, pixel values will be padded to the next multiple of 8 bits.

## Input subsystem
Collection of drivers that is designed to support all devices under Linux. The input subsystem is responsible for handling input devices such as keyboards, mice, touchscreens, and other devices that provide input to the system. The input subsystem is a part of the Linux kernel and is responsible for managing input devices and providing a consistent interface for applications to interact with these devices. 

The core of the input subsystem is the input module, MUST be loaded before any other module. This serves as a way for the two modules to communicate:
- Device drivers: Modules talking to the HW and provides events to the input module
- Event handlers: Get the events from the input module and pass them to various interfaces

### Event Handlers
Event handlers distribute the events from the devices to userspace and in-kernel consumers. 

#### evdev
- **Description**: Generic input event interface that passes events generated in the kernel to the program with timestamps.
- **Characteristics**:
  - Event codes are consistent across architectures and are hardware-independent.
  - Preferred interface for user-space to consume user input; all clients are encouraged to use it.
- **Minor Ranges**: 
  - Static legacy range: 64 through 95.
  - Additional evdev nodes created with minors starting from 256 if more than 32 input devices exist.

#### keyboard
- **Description**: In-kernel input handler that is part of VT code.
- **Functionality**: Consumes keyboard keystrokes and manages user input for VT consoles.

#### mousedev
- **Description**: A compatibility layer to enable legacy programs that use mouse input.
- **Functionality**: Takes events from mice or digitizers/tablets and makes a PS/2-style mouse device available to userland.
- **Special Notes**: 
  - Each mouse device is assigned to a single mouse or digitizer, except the last device (`mice`), which is shared by all.
  - Supports hotplugging USB mice and is useful for older programs that do not handle hotplugging.

#### joydev
- **Description**: Access point for connected joysticks in the `/dev/input` directory.
- **Functionality**: Joystick devices are represented as files, allowing programs to read joystick input.
- **Device Files**:
  - `js0`, `js1`, `js2`, `js3`, ..., up to `js31` in the legacy range.
  - Additional joystick devices can have minor numbers above 256 if there are more than 32 connected.
- **Special Notes**: Each joystick device corresponds to a connected joystick, facilitating direct interaction with userland applications.

### Event interface
To interact with the different events, one can use the system call poll()
The event interface in Linux allows you to interact with input devices through the `/dev/input/eventX` files. You can perform both blocking and non-blocking reads on these files. The design ensures that you will always read a complete number of input events during each read operation.

To perform non-blocking reads on input events, you can use the `poll()` system call. Here’s how it works:

1. **Set Up the File Descriptor**: Open the desired `/dev/input/eventX` device file.
2. **Configure for Non-Blocking**: Use the `fcntl()` function to set the file descriptor to non-blocking mode, or simply use `poll()` in a loop to check for available events without blocking.
3. **Use `poll()`**: Call the `poll()` function with an array of `pollfd` structures that includes the file descriptor for your input device. This allows you to monitor multiple file descriptors and wait for events to occur.

**Example**:
```c
#include <poll.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    struct pollfd fds[1];
    fds[0].fd = open("/dev/input/event0", O_RDONLY | O_NONBLOCK);
    fds[0].events = POLLIN;

    while (1) {
        int ret = poll(fds, 1, 1000); // Timeout of 1000 ms
        if (ret > 0) {
            // Handle events here
        } else if (ret == 0) {
            // Timeout occurred, no events
        } else {
            // Error occurred
        }
    }

    close(fds[0].fd);
    return 0;
}
```

### Event codes
A single hardware event generates multiple input events. Each input event contains the new value of a single data item. A special event type, **EV_SYN**, is used to separate input events into packets of input data changes occurring at the same moment in time. The input protocol is a stateful protocol, emitting events only when values of event codes have changed. However, the state is maintained within the Linux input subsystem; drivers do not need to maintain the state.

### Event Types
- **EV_SYN**: Used as markers to separate events.
- **EV_KEY**: Describes state changes of keyboards, buttons, or other key-like devices.
- **EV_REL**: Describes relative axis value changes (e.g., moving the mouse).
- **EV_ABS**: Describes absolute axis value changes (e.g., coordinates on a touchscreen).
- **EV_MSC**: Describes miscellaneous input data.
- **EV_SW**: Describes binary state input switches.
- **EV_LED**: Turns LEDs on devices on and off.
- **EV_SND**: Outputs sound to devices.
- **EV_REP**: For autorepeating devices.
- **EV_FF**: Sends force feedback commands.
- **EV_PWR**: For power button and switch input.
- **EV_FF_STATUS**: Receives force feedback device status.

### Event Codes

#### EV_SYN
- **SYN_REPORT**: Synchronizes and separates events into packets.
- **SYN_CONFIG**: TBD.
- **SYN_MT_REPORT**: Synchronizes and separates touch events.
- **SYN_DROPPED**: Indicates buffer overrun in the event queue.

#### EV_KEY
`EV_KEY` events represent keyboard and button actions in the Linux input event framework. These events can take the form of `KEY_<name>` or `BTN_<name>`, where `<name>` corresponds to a specific key or button. For example:

- **`KEY_A`**: Represents the 'A' key on a keyboard.
- **`BTN_LEFT`**: Represents the left mouse button.

The different types of `EV_KEY` events values include:

- **Key Press**: When a key is pressed down, an event is emitted with the corresponding key code and a value of `1`.
- **Key Release**: When the key is released, the event is emitted again with the same key code and a value of `0`.
- **Key Repeat**: Some hardware devices can send repeat events for keys. These events are emitted with a value of `2`.


#### EV_REL
Describes relative changes in a property, e.g., mouse movement. 
Special meanings include:
- **REL_WHEEL, REL_HWHEEL**: Used for scroll wheels.

#### EV_ABS
Describes absolute changes in a property, e.g., touch coordinates. 
Special meanings include:
- **ABS_DISTANCE**: Describes the distance of a tool from an interaction surface.

#### EV_SW
Describes stateful binary switches, e.g., SW_LID for laptop lid closure.

#### EV_MSC
Used for input and output events that do not fall under other categories.
- **MSC_TIMESTAMP**: Reports the number of microseconds since the last reset.

#### EV_LED
Used for setting and querying the state of various LEDs on devices.

#### EV_REP
Specifies autorepeating events.

#### EV_SND
Used for sending sound commands to devices.

#### EV_FF
Initializes force feedback capable devices.

#### EV_PWR
Used specifically for power management events.


### Device Drivers

#### Device drivers
- **Description**: Device drivers are the modules that generate events.

#### hid-generic
- **Description**: Handles all HID devices, supporting a wide variety due to the complexity of the USB HID specification.
- **Functionality**: Currently handles USB mice, joysticks, gamepads, steering wheels, keyboards, trackballs, and digitizers.

#### usbmouse
- **Description**: Handles USB mice only, using a simpler HIDBP protocol. Suitable for embedded systems or devices with broken HID descriptors.

#### usbkbd
- **Description**: Talks to keyboards using a simplified HIDBP protocol. Smaller but does not support special keys.

#### psmouse
- **Description**: Driver for pointing devices using PS/2 protocol, including touchpads and various mouse types.

#### atkbd
- **Description**: Driver for PS/2 (AT) keyboards.

#### iforce
- **Description**: Driver for I-Force joysticks and wheels, supporting both USB and RS232 with Force Feedback support.