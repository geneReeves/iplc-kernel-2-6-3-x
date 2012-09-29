/*
 * Cm15a USB driver - 0.32
 *
 * Copyright (c) 2012 Florent CARRÉ <colundrum@gmail.com> - aka COLUNDRUM
 * Copyright (c) 2010 Neil Cherry <ncherry@linuxha.com> - aka njc
 * Copyright (c) 2003 Eric Sorton <erics@cfl.rr.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * derived from Lego Tower USB driver - 0.52
 * 	Copyright (c) 2001 Juergen Stuber <stuber@loria.fr>
 * derived from USB Skeleton driver - 0.5
 * 	Copyright (c) 2001 Greg Kroah-Hartman (greg@kroah.com)
 *
 * History:
 *
 * 2012-05-06 - 0.32
 * - Changes made to make and insmod with success on 2.6.36+ and 3.x Kernel - COLUNDRUM
 * 2010-05-07 - 0.31
 * - Changes made to keep up with the USB changes to the 2.6 Kernel - njc
 * 2004-06-15 - 0.3
 * - made changes from the Lego USB driver .95
 * - works only with the 2.6 kernel
 * 2004-07-27 - 0.21
 * - added cm15a_get_feature function
 * - cm15a_get_feature called when 128 bytes (count == 128) want 
 *   to be read from cm15a_read
 * 2004-11-18 - 0.01
 * - OK so I stole the Labjack code!
 */

/*
** One thing this driver really needs is to return error codes
** when someone has disconnected the device from the USB. It
** may already do this but I'm not sure yet.
*/

/* njc
I found this up on one of the X10 boards (OP: scott?)

in the file cm15a.c

change
  #include <linux/config.h>
to
  #include <linux/autoconf.h>

comment out all (MODULE_PARM) functions but leave all (MODULE_PARM_DESC) functions

comment out line 201(mode:         S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH,)

comment out line 206(owner:   THIS_MODULE,)

It now turns the light on and off

*/
/*
Labjack
  T:  Bus=02 Lev=01 Prnt=01 Port=00 Cnt=01 Dev#=  2 Spd=1.5 MxCh= 0
  D:  Ver= 1.10 Cls=00(>ifc ) Sub=00 Prot=00 MxPS= 8 #Cfgs=  1
  P:  Vendor=0cd5 ProdID=0001 Rev= 5.f5
  S:  Manufacturer=LabJack
  S:  Product=Labjack U12
  C:* #Ifs= 1 Cfg#= 1 Atr=80 MxPwr=100mA
  I:  If#= 0 Alt= 0 #EPs= 2 Cls=03(HID  ) Sub=00 Prot=00 Driver=(none)
  E:  Ad=81(I) Atr=03(Int.) MxPS=   8 Ivl=10ms
  E:  Ad=02(O) Atr=03(Int.) MxPS=   8 Ivl=10ms
*/

/*
CM15A
  T:  Bus=01 Lev=01 Prnt=01 Port=01 Cnt=01 Dev#= 12 Spd=1.5 MxCh= 0
  D:  Ver= 1.10 Cls=00(>ifc ) Sub=00 Prot=00 MxPS= 8 #Cfgs=  1
  P:  Vendor=0bc7 ProdID=0001 Rev= 1.00
  S:  Manufacturer=X10 Wireless Technology Inc
  S:  Product=USB ActiveHome Interface
  C:* #Ifs= 1 Cfg#= 1 Atr=e0 MxPwr=  2mA
  I:  If#= 0 Alt= 0 #EPs= 2 Cls=00(>ifc ) Sub=00 Prot=00 Driver=usbdevfs
  E:  Ad=81(I) Atr=03(Int.) MxPS=   8 Ivl=10ms
  E:  Ad=02(O) Atr=03(Int.) MxPS=   8 Ivl=10ms

*/
/*
IPLC

Bus 001 Device 002: ID 10bf:0004 SmartHome
Device Descriptor:
  bLength                18
  bDescriptorType         1
  bcdUSB               1.10
  bDeviceClass            0 (Defined at Interface level)
  bDeviceSubClass         0
  bDeviceProtocol         0
  bMaxPacketSize0         8
  idVendor           0x10bf SmartHome
  idProduct          0x0004
  bcdDevice            4.00
  iManufacturer           1 SmartHome
  iProduct                2 SmartHome PowerLinc USB E
  iSerial                 0
  bNumConfigurations      1
  Configuration Descriptor:
    bLength                 9
    bDescriptorType         2
    wTotalLength           41
    bNumInterfaces          1
    bConfigurationValue     1
    iConfiguration          4 Cfg1
    bmAttributes         0xa0
      Remote Wakeup
    MaxPower              500mA
    Interface Descriptor:
      bLength                 9
      bDescriptorType         4
      bInterfaceNumber        0
      bAlternateSetting       0
      bNumEndpoints           2
      bInterfaceClass         3 Human Interface Devices
      bInterfaceSubClass      1 Boot Interface Subclass
      bInterfaceProtocol      0 None
      iInterface              5 EP10In
        HID Device Descriptor:
          bLength                 9
          bDescriptorType        33
          bcdHID               1.00
          bCountryCode            0 Not supported
          bNumDescriptors         1
          bDescriptorType        34 Report
          wDescriptorLength      29
          Report Descriptor: (length is 29)
            Item(Global): Usage Page, data= [ 0x00 0xff ] 65280
                            (null)
            Item(Local ): Usage, data= [ 0x01 ] 1
                            (null)
            Item(Main  ): Collection, data= [ 0x01 ] 1
                            Application
            Item(Local ): Usage Minimum, data= [ 0x01 ] 1
                            (null)
            Item(Local ): Usage Maximum, data= [ 0x08 ] 8
                            (null)
            Item(Global): Logical Minimum, data= [ 0x00 ] 0
            Item(Global): Logical Maximum, data= [ 0xff 0x00 ] 255
            Item(Global): Report Size, data= [ 0x08 ] 8
            Item(Global): Report Count, data= [ 0x08 ] 8
            Item(Main  ): Input, data= [ 0x02 ] 2
                            Data Variable Absolute No_Wrap Linear
                            Preferred_State No_Null_Position Non_Volatile Bitfield
            Item(Local ): Usage Minimum, data= [ 0x01 ] 1
                            (null)
            Item(Local ): Usage Maximum, data= [ 0x08 ] 8
                            (null)
            Item(Main  ): Output, data= [ 0x02 ] 2
                            Data Variable Absolute No_Wrap Linear
                            Preferred_State No_Null_Position Non_Volatile Bitfield
            Item(Main  ): End Collection, data=none
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x81  EP 1 IN
        bmAttributes            3
          Transfer Type            Interrupt
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0008  1x 8 bytes
        bInterval              10
      Endpoint Descriptor:
        bLength                 7
        bDescriptorType         5
        bEndpointAddress     0x01  EP 1 OUT
        bmAttributes            3
          Transfer Type            Interrupt
          Synch Type               None
          Usage Type               Data
        wMaxPacketSize     0x0008  1x 8 bytes
        bInterval              10
 */

// I need to set this up in some file (njc)
#define AUTOCONF

#if defined(AUTOCONF)
#include <generated/autoconf.h> // COLUNDRUM - directory changed from linux to generated (change from source kernel)
#else
// #include <linux/config.h> // http://forums.labjack.com/index.php?showtopic=4753 Labjack support
//                           // suggests that the config.h file is deprecated
#endif

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
//#include <linux/smp_lock.h> // COLUNDRUM - deprecated
#include <linux/completion.h>
#include <asm/uaccess.h>
#include <linux/usb.h>
#include <linux/poll.h>
#include <linux/version.h> // COLUNDRUM - added

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,36) // COLUNDRUM - added
#define DECLARE_MUTEX DEFINE_SEMAPHORE // COLUNDRUM - added
#endif // COLUNDRUM - added

#ifdef CONFIG_USB_DEBUG
static int debug = 0;
#else
static int debug = 0;
#endif

#undef dbg
#define dbg(lvl, format, arg...) do { if (debug >= lvl) printk(KERN_DEBUG  __FILE__ ": " format " \n", ## arg); } while (0)

// ===========================================================================
// I think it would be a good idea to allow users to pass the Vendor ID,
// the Product ID and the Description that way this could be a universal
// prototype driver.
// ===========================================================================

#include "device.h"

#define DRIVER_VERSION "v0.32"
#define DRIVER_AUTHOR "Neil Cherry <ncherry@linuxha.com> patched by Florent CARRÉ <colundrum@gmail.com>"

// X10 CM15A
#ifdef CM15A
#define CLASS_NAME	"cm15a%d"
#define DRIVER_NAME	"cm15a"
#define USB_VENDOR_ID	0x0bc7
#define USB_PRODUCT_ID	0x0001
#define DRIVER_DESC	"X10 CM15A USB Driver"
#endif

// X10 CM19A
#ifdef CM19A
#define CLASS_NAME	"cm19a%d"
#define DRIVER_NAME	"cm19A"
#define USB_VENDOR_ID	0x0bc7
#define USB_PRODUCT_ID	0x0001
#define DRIVER_DESC	"X10 CM19A USB Driver"
#endif

// SmartHome Powerlinc
#ifdef POWERLINC
#define CLASS_NAME	"plinc%d"
#define DRIVER_NAME	"plinc"
#define USB_VENDOR_ID	0x10bf
#define USB_PRODUCT_ID	0x0001
#define DRIVER_DESC	"Smarthome USB PowerLinc Driver"
#endif

// SmartHme Insteon Powerlinc
#ifdef INSTEON
#define CLASS_NAME	"iplc%d"
#define DRIVER_NAME	"iplc"
#define USB_NAME	"iplc"
#define USB_VENDOR_ID	0x10bf
#define USB_PRODUCT_ID	0x0004
#define DRIVER_DESC	"Smarthome Insteon USB PowerLinc V2 Driver"
#endif

// Not complete, need the IDs
#ifdef UPB
#define CLASS_NAME	"upb%d"
#define DRIVER_NAME	"upb"
#define USB_VENDOR_ID	0x0
#define USB_PRODUCT_ID	0x0001
#define DRIVER_DESC	"UPB USB Driver"
#endif

// Labjack U12
#ifdef LABJACK
#define CLASS_NAME	"lj%d"
#define DRIVER_NAME	"lj"
#define USB_VENDOR_ID	0x0cd5
#define USB_PRODUCT_ID	0x0001
#define DRIVER_DESC	"Labjack U12"
#endif

// ===========================================================================

module_param(debug, int, S_IRUGO | S_IWUSR);
MODULE_PARM_DESC(debug, "Debug enabled or not");

static size_t read_buffer_size = 512;
module_param(read_buffer_size, int, 0);
MODULE_PARM_DESC(read_buffer_size, "Read buffer size");

static size_t write_buffer_size = 256;
module_param(write_buffer_size, int, 0);
MODULE_PARM_DESC(write_buffer_size, "Write buffer size");

static int packet_timeout = 0;
module_param(packet_timeout, int, 0);
MODULE_PARM_DESC(packet_timeout, "Packet timeout in ms");

static int read_timeout = 1000;
module_param(read_timeout, int, 0);
MODULE_PARM_DESC(read_timeout, "Read timeout in ms");

static int interrupt_in_interval = 0;
module_param(interrupt_in_interval, int, 0);
MODULE_PARM_DESC(interrupt_in_interval, "Interrupt in interval in ms");

static int interrupt_out_interval = 0;
module_param(interrupt_out_interval, int, 0);
MODULE_PARM_DESC(interrupt_out_interval, "Interrupt out interval in ms");

static struct usb_device_id table [] = {
  { USB_DEVICE(USB_VENDOR_ID, USB_PRODUCT_ID) },
  { }
};

MODULE_DEVICE_TABLE (usb, table);


/* :TODO: Get a minor range for your devices from the usb maintainer */
#define USB_MINOR_BASE	0xfc	// 0xfc will give us 4 devices max
//#define USB_MINOR_BASE	0x00

struct usb {
  struct semaphore	sem;		/* locks this structure */
  struct usb_device* 	udev;		/* save off the usb device pointer */
  unsigned char		minor;		/* the starting minor number for this device */

  int			open_count;	/* number of times this port has been opened */

  char*			read_buffer;
  size_t                read_buffer_length; /* this much came in */
  size_t                read_packet_length; /* this much will be returned on read */
  spinlock_t            read_buffer_lock;
  int                   packet_timeout_jiffies;
  unsigned long         read_last_arrival;

  wait_queue_head_t	read_wait;
  wait_queue_head_t	write_wait;

  char*			interrupt_in_buffer;
  struct usb_endpoint_descriptor* interrupt_in_endpoint;
  struct urb*		interrupt_in_urb;
  int                   interrupt_in_interval;
  int                   interrupt_in_running;
  int                   interrupt_in_done;

  char*			interrupt_out_buffer;
  struct usb_endpoint_descriptor* interrupt_out_endpoint;
  struct urb*		interrupt_out_urb;
  int                   interrupt_out_interval;
  int                   interrupt_out_busy;

};

static ssize_t read(struct file *file, char *buffer, size_t count, loff_t *ppos);
static ssize_t write(struct file *file, const char *buffer, size_t count, loff_t *ppos);
static long unlocked_ioctl(struct file *file, unsigned int cmd, unsigned long arg);  // COLUNDRUM - changed from ioctl to unlocked_ioctl
static inline void delete(struct usb *dev);
static int open(struct inode *inode, struct file *file);
static int release(struct inode *inode, struct file *file);
static unsigned int poll(struct file *file, poll_table *wait);
static loff_t llseek(struct file *file, loff_t off, int whence);

static void abort_transfers (struct usb *dev);
static void check_for_read_packet (struct usb *dev);
static void interrupt_in_callback (struct urb *urb,  struct pt_regs *regs);
static void interrupt_out_callback (struct urb *urb,  struct pt_regs *regs);
static int  probe (struct usb_interface *interface, const struct usb_device_id *id);
static void disconnect    (struct usb_interface *interface);

static DECLARE_MUTEX (disconnect_sem);

static struct file_operations fops = {
  owner:	THIS_MODULE,
  read:		read,
  write:	write,
  unlocked_ioctl:  unlocked_ioctl,  // COLUNDRUM - changed from ioctl to unlocked_ioctl
  open:		open,
  release:	release, 
  poll:         poll,
  llseek:       llseek,
};

static struct usb_class_driver class = {
//name:	        "cm15a%d",
  name:		CLASS_NAME,
  fops:     	&fops,
//mode:         S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH,
  minor_base:   USB_MINOR_BASE,
};

/**
 * struct usb_driver - identifies USB driver to usbcore
 * @name: The driver name should be unique among USB drivers,
 *      and should normally be the same as the module name.
 * @probe: Called to see if the driver is willing to manage a particular
 *      interface on a device.  If it is, probe returns zero and uses
 *      dev_set_drvdata() to associate driver-specific data with the
 *      interface.  It may also use usb_set_interface() to specify the
 *      appropriate altsetting.  If unwilling to manage the interface,
 *      return a negative errno value.
 * @disconnect: Called when the interface is no longer accessible, usually
 *      because its device has been (or is being) disconnected or the
 *      driver module is being unloaded.
 * @ioctl: Used for drivers that want to talk to userspace through
 *      the "usbfs" filesystem.  This lets devices provide ways to
 *      expose information to user space regardless of where they
 *      do (or don't) show up otherwise in the filesystem.
 * @suspend: Called when the device is going to be suspended by the system.
 * @resume: Called when the device is being resumed by the system.
 * @id_table: USB drivers use ID table to support hotplugging.
 *      Export this with MODULE_DEVICE_TABLE(usb,...).  This must be set
 *      or your driver's probe function will never get called.
 * @dynids: used internally to hold the list of dynamically added device
 *      ids for this driver.
 * @driver: the driver model core driver structure.
 * @no_dynamic_id: if set to 1, the USB core will not allow dynamic ids to be
 *      added to this driver by preventing the sysfs file from being created.
 *
 * USB drivers must provide a name, probe() and disconnect() methods,
 * and an id_table.  Other driver fields are optional.
 *
 * The id_table is used in hotplugging.  It holds a set of descriptors,
 * and specialized data may be associated with each entry.  That table
 * is used by both user and kernel mode hotplugging support.
 *
 * The probe() and disconnect() methods are called in a context where
 * they can sleep, but they should avoid abusing the privilege.  Most
 * work to connect to a device should be done when the device is opened,
 * and undone at the last close.  The disconnect code needs to address
 * concurrency issues with respect to open() and close() methods, as
 * well as forcing all pending I/O requests to complete (by unlinking
 * them as necessary, and blocking until the unlinks complete).
 */

static struct usb_driver driver = {
#if 0
  // Looks like they got rid of this somewhere in 2.6.15
  owner:	THIS_MODULE,
#endif
//name:		"cm15a",
  name:		DRIVER_NAME,
  probe:	probe,
  disconnect:	disconnect,
  id_table:	table,
};


static inline void usb_debug_data (int level, const char *function, int size, const unsigned char *data)
{
  int i;

  if (debug < level)
    return; 
	
  printk (KERN_DEBUG __FILE__": %s - length = %d, data = ", function, size);
    
  for (i = 0; i < size; ++i) {
    printk ("%.2x ", data[i]);
  }
  printk ("\n");
}


static inline void delete (struct usb *dev)
{
  dbg(2, "%s enter", __FUNCTION__);

  abort_transfers (dev);

  if (dev->interrupt_in_urb != NULL) {
    usb_free_urb (dev->interrupt_in_urb);
  }
  if (dev->interrupt_out_urb != NULL) {
    usb_free_urb (dev->interrupt_out_urb);
  }
  kfree (dev->read_buffer);
  kfree (dev->interrupt_in_buffer);
  kfree (dev->interrupt_out_buffer);
  kfree (dev);
    
  dbg(2, "%s : leave", __FUNCTION__);
}


static int open (struct inode *inode, struct file *file)
{
  struct usb *dev = NULL;
  int subminor;
  int retval = 0;
  struct usb_interface *interface;
    
  dbg(2,"%s : enter", __FUNCTION__);

  subminor = iminor(inode);

  down(&disconnect_sem);

  interface = usb_find_interface(&driver, subminor);

  if (!interface) {
    err ("%s - error, can't find device for minor %d", __FUNCTION__, subminor);
    retval = -ENODEV;
    goto unlock_disconnect_exit;
  }

  dev = usb_get_intfdata(interface);

  if (!dev) {
    retval = -ENODEV;
    goto unlock_disconnect_exit;
  }
    
  if (down_interruptible (&dev->sem)) {
    retval = -ERESTARTSYS;
    goto unlock_disconnect_exit;
  }
 
  if (dev->open_count) {
    retval = -EBUSY;
    goto unlock_exit;
  }
    
  dev->open_count = 1;

  dev->read_buffer_length = 0;

  dev->read_packet_length = 0;

  // From /usr/src/linux/include/linux/usb.h
  // static inline void usb_fill_int_urb(struct urb *urb,
  //                                     struct usb_device *dev,
  //                                     unsigned int pipe,
  //                                     void *transfer_buffer,
  //                                     int buffer_length,
  //                                     usb_complete_t complete_fn,
  //                                     void *context,
  //                                     int interval)

  usb_fill_int_urb (dev->interrupt_in_urb,
		    dev->udev,
		    usb_rcvintpipe(dev->udev, dev->interrupt_in_endpoint->bEndpointAddress),
		    dev->interrupt_in_buffer,
		    dev->interrupt_in_endpoint->wMaxPacketSize,
		    (usb_complete_t) interrupt_in_callback, // njc - this is a hack until I figure out usb_complet_t
		    dev,
		    dev->interrupt_in_interval);

  dev->interrupt_in_running = 1;
  dev->interrupt_in_done = 0;
  mb();
    
  retval = usb_submit_urb (dev->interrupt_in_urb, GFP_KERNEL);
  if (retval) {
    err("Couldn't submit interrupt_in_urb %d", retval);
    dev->interrupt_in_running = 0;
    dev->open_count = 0;
    goto unlock_exit;
  }

  file->private_data = dev;
 
 unlock_exit:
  up (&dev->sem);
	
 unlock_disconnect_exit:
  up (&disconnect_sem);

  dbg(2,"%s : leave, return value %d ", __FUNCTION__, retval);

  return retval;
}


static int release (struct inode *inode, struct file *file)
{
  struct usb *dev;
  int retval = 0;

  dbg(2," %s : enter", __FUNCTION__);

  dev = (struct usb *)file->private_data;

  if (dev == NULL) {
    dbg(1," %s : object is NULL", __FUNCTION__);
    retval = -ENODEV;
    goto exit;
  }

  if (down_interruptible (&dev->sem)) {
    retval = -ERESTARTSYS;
    goto exit;
  }

  if (dev->open_count != 1) {
    dbg(1, "%s: device not opened exactly once", __FUNCTION__);
    retval = -ENODEV;
    goto unlock_exit;
  }
	
  if (dev->udev == NULL) {
    up (&dev->sem); 
    delete (dev);
    goto exit;
  }

  if (dev->interrupt_out_busy) {
    wait_event_interruptible_timeout (dev->write_wait, !dev->interrupt_out_busy, 2 * HZ);
  }
  abort_transfers (dev);
  dev->open_count = 0;

 unlock_exit:
  up (&dev->sem);
  dbg(2," %s : leave, return value %d", __FUNCTION__, retval);

 exit:
  dbg(2," %s : leave, return value %d", __FUNCTION__, retval);
  return retval;
}


static void abort_transfers (struct usb *dev)
{
  dbg(2," %s : enter", __FUNCTION__);

  if (dev == NULL) {
    dbg(1, "%s: dev is null", __FUNCTION__);
    goto exit;
  }

  if (dev->interrupt_in_running) {
    dev->interrupt_in_running = 0;
    mb();
    if (dev->interrupt_in_urb != NULL && dev->udev) {
      usb_unlink_urb (dev->interrupt_in_urb);
    }	
  }
  if (dev->interrupt_out_busy) {
    if (dev->interrupt_out_urb != NULL && dev->udev) {
      usb_unlink_urb (dev->interrupt_out_urb);
    }
  }

 exit:
  dbg(2," %s : leave", __FUNCTION__);
}

static void check_for_read_packet (struct usb *dev)
{
  spin_lock_irq (&dev->read_buffer_lock);
  if (!packet_timeout
      || time_after(jiffies, dev->read_last_arrival + dev->packet_timeout_jiffies)
      || dev->read_buffer_length == read_buffer_size) {
    dev->read_packet_length = dev->read_buffer_length;
  }
  dev->interrupt_in_done = 0;
  spin_unlock_irq (&dev->read_buffer_lock);
}

static unsigned int poll (struct file *file, poll_table *wait)
{
  struct usb *dev;
  unsigned int mask = 0;

  dbg(2, "%s: enter", __FUNCTION__);

  dev = file->private_data;

  poll_wait(file, &dev->read_wait, wait);
  poll_wait(file, &dev->write_wait, wait);
	
  check_for_read_packet(dev);
  if (dev->read_packet_length > 0) {
    mask |= POLLIN | POLLRDNORM;
  }
  if (!dev->interrupt_out_busy) {
    mask |= POLLOUT | POLLWRNORM;
  }
 
  dbg(2, "%s: leave, mask = %d", __FUNCTION__, mask);
  return mask;
}

static loff_t llseek (struct file *file, loff_t off, int whence)
{
  return -ESPIPE;         /* unseekable */
}

// =[ read() ]==========================================================

static ssize_t read (struct file *file, char *buffer, size_t count, loff_t *ppos)
{
  struct usb *dev;
  size_t bytes_to_read;
  int i;
  int retval = 0;
  unsigned long timeout = 0;
	
  dbg(2," %s : enter, count = %d", __FUNCTION__, count);

  dev = (struct usb *)file->private_data;

  if (down_interruptible (&dev->sem)) {
    retval = -ERESTARTSYS;
    goto exit;
  }

  if (dev->udev == NULL) {
    retval = -ENODEV;
    err("No device or device unplugged %d", retval);
    goto unlock_exit;
  }
	
  if (count == 0) {
    dbg(1," %s : read request of 0 bytes", __FUNCTION__);
    goto unlock_exit;
  }

  if (read_timeout) {
    timeout = jiffies + read_timeout * HZ / 1000;
  }

  check_for_read_packet (dev);
	
  while (dev->read_packet_length == 0) {
    if (file->f_flags & O_NONBLOCK) {
      retval = -EAGAIN;
      goto unlock_exit;
    }
        
    retval = wait_event_interruptible_timeout(dev->read_wait, dev->interrupt_in_done, dev->packet_timeout_jiffies);
    if (retval < 0) {
      goto unlock_exit;
    }

    if (read_timeout && (dev->read_buffer_length || dev->interrupt_out_busy)) {
      timeout = jiffies + read_timeout * HZ / 1000;
    }

    if (read_timeout && time_after (jiffies, timeout)) {
      retval = -ETIMEDOUT;
      goto unlock_exit;
    }
    check_for_read_packet (dev);
  }	// End while(dev->read_packet_length == 0)

  bytes_to_read = min(count, dev->read_packet_length);

  if (copy_to_user (buffer, dev->read_buffer, bytes_to_read)) {
    retval = -EFAULT;
    goto unlock_exit;
  } 

  spin_lock_irq (&dev->read_buffer_lock);
  dev->read_buffer_length -= bytes_to_read;
  dev->read_packet_length -= bytes_to_read;
  for (i=0; i<dev->read_buffer_length; i++) {
    dev->read_buffer[i] = dev->read_buffer[i+bytes_to_read];
  }
  spin_unlock_irq (&dev->read_buffer_lock);
  retval = bytes_to_read;

 unlock_exit:
  up (&dev->sem);

 exit:
  dbg(2," %s : leave, return value %d", __FUNCTION__, retval);
  return retval;
}


// =[ write() ]=========================================================

static ssize_t write (struct file *file, const char *buffer, size_t count, loff_t *ppos)
{
  struct usb *dev;
  size_t bytes_to_write;
  int retval = 0;

  dbg(2," %s : enter, count = %Zd", __FUNCTION__, count);

  dev = (struct usb *)file->private_data;

  if (down_interruptible (&dev->sem)) {
    retval = -ERESTARTSYS;
    goto exit;
  }

  if (dev->udev == NULL) {
    retval = -ENODEV;
    err("No device or device unplugged %d", retval);
    goto unlock_exit;
  }

  if (count == 0) {
    dbg(1," %s : write request of 0 bytes", __FUNCTION__);
    goto unlock_exit;
  }
	
  while (dev->interrupt_out_busy) {
    if (file->f_flags & O_NONBLOCK) {
      retval = -EAGAIN;
      goto unlock_exit;
    }
    retval = wait_event_interruptible (dev->write_wait, !dev->interrupt_out_busy);
    if (retval) {
      goto unlock_exit;
    }
  }
    
  bytes_to_write = min(count, write_buffer_size);
  dbg(4, "%s: count = %Zd, bytes_to_write = %Zd", __FUNCTION__, count, bytes_to_write);

  if (copy_from_user (dev->interrupt_out_buffer, buffer, bytes_to_write)) {
    retval = -EFAULT;
    goto unlock_exit;
  }

  usb_fill_int_urb(dev->interrupt_out_urb,
		   dev->udev,
		   usb_sndintpipe(dev->udev, dev->interrupt_out_endpoint->bEndpointAddress),
		   dev->interrupt_out_buffer,
		   bytes_to_write,
		   (usb_complete_t) interrupt_out_callback, // njc - this is a hack until I figure out usb_complet_t
		   dev,
		   dev->interrupt_out_interval);

  dev->interrupt_out_busy = 1;
  wmb(); // ???
 
  retval = usb_submit_urb (dev->interrupt_out_urb, GFP_KERNEL);
  if (retval) {
    dev->interrupt_out_busy = 0;
    err("Couldn't submit interrupt_out_urb %d", retval);
    goto unlock_exit;
  }
  retval = bytes_to_write;

 unlock_exit:
  up (&dev->sem);

 exit:
  dbg(2," %s : leave, return value %d", __FUNCTION__, retval);
  return retval;
}


// =[ unlocked_ioctl() ]=========================================================  // COLUNDRUM - changed

static long unlocked_ioctl (struct file *file, unsigned int cmd, unsigned long arg) // COLUNDRUM - changed
{
  struct usb *dev;
  int retval =  -ENOTTY;  /* default: we don't understand ioctl */

  dev = (struct usb *)file->private_data;

  down (&dev->sem);

  if (dev->udev == NULL) {
    retval = -ENODEV;
    goto unlock_exit;
  }
        
  switch (cmd) {
  default:
    dbg(2, "Requested ioctl: %d", cmd);

    /* :TODO: add ioctl commands as needed */
  }
	
 unlock_exit:
  up (&dev->sem);

  dbg(2," %s : leave, return value %d", __func__, retval);

  return retval;
}


// ===========================================================================

static void interrupt_in_callback (struct urb *urb, struct pt_regs *regs)
{
  struct usb *dev = (struct usb *)urb->context;
  int retval;
	
  dbg(4," %s : enter, status %d", __FUNCTION__, urb->status);

  usb_debug_data(5,__FUNCTION__, urb->actual_length, urb->transfer_buffer);   
    
  if (urb->status) {
    if (urb->status == -ENOENT ||
	urb->status == -ECONNRESET ||
	urb->status == -ESHUTDOWN) {
      goto exit;
    } else {
      dbg(1, "%s: nonzero status received: %d", __FUNCTION__, urb->status);
      goto resubmit; /* maybe we can recover */
    }
  }

  if (urb->actual_length > 0) {
    spin_lock (&dev->read_buffer_lock);
    if (dev->read_buffer_length + urb->actual_length < read_buffer_size) {
      memcpy (dev->read_buffer + dev->read_buffer_length,
	      dev->interrupt_in_buffer,
	      urb->actual_length);
      dev->read_buffer_length += urb->actual_length;
      dev->read_last_arrival = jiffies;
      dbg(3, "%s: received %d bytes", __FUNCTION__, urb->actual_length);
    } else {
      printk(KERN_WARNING "%s: read_buffer overflow, %d bytes dropped (%d/%d)\n", __FUNCTION__, urb->actual_length, dev->read_buffer_length , read_buffer_size);
    }
    spin_unlock (&dev->read_buffer_lock);
  }

 resubmit:
  if (dev->interrupt_in_running && dev->udev) {
    retval = usb_submit_urb (dev->interrupt_in_urb, GFP_ATOMIC);
    if (retval) {
      err("%s: usb_submit_urb failed (%d)", __FUNCTION__, retval);
    }
  }

 exit:
  dev->interrupt_in_done = 1;
  wake_up_interruptible (&dev->read_wait);
  usb_debug_data(5, __FUNCTION__, urb->actual_length, urb->transfer_buffer);
  dbg(4, "%s: leave, status %d", __FUNCTION__, urb->status);
}


static void interrupt_out_callback (struct urb *urb, struct pt_regs *regs)
{
  struct usb *dev = (struct usb *)urb->context;

  dbg(4," %s : enter, status %d", __FUNCTION__, urb->status);
  usb_debug_data(5,__FUNCTION__, urb->actual_length, urb->transfer_buffer);

  if (urb->status && !(urb->status == -ENOENT ||
		       urb->status == -ECONNRESET ||
		       urb->status == -ESHUTDOWN)) {
    dbg(1, "%s - nonzero write bulk status received: %d",
	__FUNCTION__, urb->status);
  }
  dev->interrupt_out_busy = 0;
  wake_up_interruptible(&dev->write_wait);

  usb_debug_data(5,__FUNCTION__, urb->actual_length, urb->transfer_buffer);
  dbg(4," %s : leave, status %d", __FUNCTION__, urb->status);
}


static int probe (struct usb_interface *interface, const struct usb_device_id *id)
{
  struct usb_device *udev = interface_to_usbdev(interface);
  struct usb *dev = NULL;
  struct usb_host_interface *iface_desc;
  struct usb_endpoint_descriptor* endpoint;
  int i;
  int retval = -ENOMEM;

  dbg(2," %s : enter", __FUNCTION__);

  if (udev == NULL) {
    //info ("udev is NULL."); // njc
    //dev_info ("udev is NULL."); // njc
  }
	
  if ((udev->descriptor.idVendor != USB_VENDOR_ID) ||
      (udev->descriptor.idProduct != USB_PRODUCT_ID)) {
    goto exit;
  }
  dev = kmalloc (sizeof(struct usb), GFP_KERNEL);
	
  if (dev == NULL) {
    err ("Out of memory");
    goto exit;
  }
	
  #ifndef init_MUTEX // COLUNDRUM - added
    sema_init (&dev->sem,1); // COLUNDRUM - added
  #else // COLUNDRUM - added
    init_MUTEX (&dev->sem); // COLUNDRUM - original line
  #endif // COLUNDRUM - added
	
  dev->udev = udev;
  dev->open_count = 0;

  dev->read_buffer = NULL;
  dev->read_buffer_length = 0;
  dev->read_packet_length = 0;
  spin_lock_init (&dev->read_buffer_lock);
  dev->packet_timeout_jiffies = packet_timeout * HZ / 1000;
  dev->read_last_arrival = jiffies;

  init_waitqueue_head (&dev->read_wait);
  init_waitqueue_head (&dev->write_wait);

  dev->interrupt_in_buffer = NULL;
  dev->interrupt_in_endpoint = NULL;
  dev->interrupt_in_urb = NULL;
  dev->interrupt_in_running = 0;
  dev->interrupt_in_done = 0;

  dev->interrupt_out_buffer = NULL;
  dev->interrupt_out_endpoint = NULL;
  dev->interrupt_out_urb = NULL;
  dev->interrupt_out_busy = 0;

  iface_desc = &interface->altsetting[0];
    
  for (i = 0; i < iface_desc->desc.bNumEndpoints; ++i) {
    endpoint = &iface_desc->endpoint[i].desc;

    if (((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_IN) &&
	((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT)) {
      dev->interrupt_in_endpoint = endpoint;
    }
		
    if (((endpoint->bEndpointAddress & USB_ENDPOINT_DIR_MASK) == USB_DIR_OUT) &&
	((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT)) {
      dev->interrupt_out_endpoint = endpoint;
    }
  }        
	
  if(dev->interrupt_in_endpoint == NULL) {
    err("interrupt in endpoint not found");
    goto error;
  }
  if (dev->interrupt_out_endpoint == NULL) {
    err("interrupt out endpoint not found");
    goto error;  
  }

  dev->read_buffer = kmalloc (read_buffer_size, GFP_KERNEL);
  if (!dev->read_buffer) {
    err("Couldn't allocate read_buffer");
    goto error;
  }
  dev->interrupt_in_buffer = kmalloc (dev->interrupt_in_endpoint->wMaxPacketSize, GFP_KERNEL);
  if (!dev->interrupt_in_buffer) {
    err("Couldn't allocate interrupt_in_buffer");
    goto error;
  }
  dev->interrupt_in_urb = usb_alloc_urb(0, GFP_KERNEL);
  if (!dev->interrupt_in_urb) {
    err("Couldn't allocate interrupt_in_urb");
    goto error;    
  }
  dev->interrupt_out_buffer = kmalloc (write_buffer_size, GFP_KERNEL);
  if (!dev->interrupt_out_buffer) {
    err("Couldn't allocate interrupt_out_buffer");
    goto error;
  }
  dev->interrupt_out_urb = usb_alloc_urb(0, GFP_KERNEL);
  if (!dev->interrupt_out_urb) {
    err("Couldn't allocate interrupt_out_urb");
    goto error;
  }                
  dev->interrupt_in_interval = interrupt_in_interval ? interrupt_in_interval : dev->interrupt_in_endpoint->bInterval;
  dbg(2, "In Int interval = %d", dev->interrupt_in_interval);
  dev->interrupt_out_interval = interrupt_out_interval ? interrupt_out_interval : dev->interrupt_out_endpoint->bInterval;
  dbg(2, "Out Int interval = %d", dev->interrupt_out_interval);
   	
  usb_set_intfdata (interface, dev);

  retval = usb_register_dev (interface, &class);
	
  if (retval) {
    err ("Not able to get a minor for this device.");
    usb_set_intfdata (interface, NULL);
    goto error;
  }

  dev->minor = interface->minor;
  //info ("%s USB #%d now attached to major %d minor %d", DRIVER_NAME, (dev->minor), USB_MAJOR, dev->minor); // njc
  //dev_info ("%s USB #%d now attached to major %d minor %d", DRIVER_NAME, (dev->minor), USB_MAJOR, dev->minor); // njc

 exit:
  dbg(2," %s : leave, return value 0x%.8lx (dev)", __FUNCTION__, (long) dev);

  return retval;

 error:
  delete(dev);
  return retval;
}


static void disconnect (struct usb_interface *interface)
{
  struct usb *dev;
  int minor;

  dbg(2," %s : enter", __FUNCTION__);

  down (&disconnect_sem);

  dev = usb_get_intfdata (interface);
  usb_set_intfdata (interface, NULL);
	
  down (&dev->sem);

  minor = dev->minor;

  usb_deregister_dev (interface, &class);
	
  if (!dev->open_count) {
    up (&dev->sem);
    delete (dev);
  } else {
    dev->udev = NULL;
    up (&dev->sem);
  }

  up(&disconnect_sem);
	
  //dev_info("%s USB #%d now disconnected", DRIVER_NAME, minor);

  dbg(2," %s : leave", __FUNCTION__);
}


static int __init usb_init(void)
{
  int result;
  int retval = 0;

  dbg(2," %s : enter", __FUNCTION__);

  result = usb_register(&driver);
  if (result < 0) {
    err("usb_register failed for the "__FILE__" driver. Error number %d", result);
    retval = -1;
    goto exit;
  }

  //dev_info(DRIVER_DESC " " DRIVER_VERSION);

 exit:
  dbg(2," %s : leave, return value %d", __FUNCTION__, retval);

  return retval;
}


static void __exit usb_exit(void)
{
  dbg(2," %s : enter", __FUNCTION__);

  usb_deregister (&driver);

  dbg(2," %s : leave", __FUNCTION__);
}

module_init (usb_init);
module_exit (usb_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
#ifdef MODULE_LICENSE
MODULE_LICENSE("GPL");
#endif
