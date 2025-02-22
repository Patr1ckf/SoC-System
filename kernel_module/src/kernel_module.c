#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ioport.h>
#include <linux/types.h>
#include <asm/errno.h>
#include <asm/io.h>

MODULE_INFO(intree, "Y");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Patryk Figiel");
MODULE_DESCRIPTION("Simple kernel module for SYKOM project");
MODULE_VERSION("1.0");

#define SYKT_GPIO_BASE_ADDR  (0x00100000)
#define SYKT_GPIO_SIZE       (0x8000)
#define SYKT_EXIT            (031463)
#define SYKT_EXIT_CODE       (0177)

#define SYKT_GPIO_ADDR_SPACE (baseptr)
#define A (SYKT_GPIO_ADDR_SPACE + 0x100)
#define W (SYKT_GPIO_ADDR_SPACE + 0x110)
#define S (SYKT_GPIO_ADDR_SPACE + 0x120)

void __iomem *baseptr;
static struct kobject *kobj_ref;

static int rejAValue;
static int rejWValue;
static int rejSValue;

// Write to A
static ssize_t rejA_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    int ret;
    // Parse the input buffer for an octal integer
    ret = sscanf(buf, "%o", &rejAValue);
    if (ret != 1) {
        printk(KERN_ERR "Failed to parse input. Expected one octal number, got %d\n", ret);
        return -EINVAL; // Error: invalid number of arguments
    }

    // Write the parsed value to the hardware register A
    writel(rejAValue, A);
    return count;
}

// Read from A
static ssize_t rejA_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    // Read the value from the hardware register A
    rejAValue = readl(A);
    return sprintf(buf, "%o", rejAValue);
}

// Read from W
static ssize_t rejW_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    // Read the value from the hardware register W
    rejWValue = readl(W);
    return sprintf(buf, "%o", rejWValue);
}

// Read from S
static ssize_t rejS_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
    // Read the value from the hardware register S
    rejSValue = readl(S);
    return sprintf(buf, "%o", rejSValue);
}

// Sysfs attributes
static struct kobj_attribute rejA_attr = __ATTR_RW(rejA);
static struct kobj_attribute rejW_attr = __ATTR_RO(rejW);
static struct kobj_attribute rejS_attr = __ATTR_RO(rejS);

int my_init_module(void) // Initialization
{
    int ret;

    printk(KERN_INFO "Init my module.\n");

    // Remap the GPIO base address
    baseptr = ioremap_nocache(SYKT_GPIO_BASE_ADDR, SYKT_GPIO_SIZE);
    if (!baseptr) {
        printk(KERN_ERR "Failed to remap GPIO address space\n");
        return -ENXIO;
    }

    // Create a kobject and add it to the sysfs
    kobj_ref = kobject_create_and_add("proj4figpat", kernel_kobj);
    if (!kobj_ref) {
        printk(KERN_ERR "Failed to create sysfs entry\n");
        iounmap(baseptr);
        return -ENOMEM;
    }

    // Create sysfs files
    ret = sysfs_create_file(kobj_ref, &rejA_attr.attr);
    if (ret) {
        printk(KERN_ERR "Cannot create sysfs file rejA\n");
        goto error_cleanup;
    }

    ret = sysfs_create_file(kobj_ref, &rejW_attr.attr);
    if (ret) {
        printk(KERN_ERR "Cannot create sysfs file rejW\n");
        goto error_cleanup;
    }

    ret = sysfs_create_file(kobj_ref, &rejS_attr.attr);
    if (ret) {
        printk(KERN_ERR "Cannot create sysfs file rejS\n");
        goto error_cleanup;
    }

    return 0;

error_cleanup:
    // Cleanup on error
    sysfs_remove_file(kobj_ref, &rejA_attr.attr);
    sysfs_remove_file(kobj_ref, &rejW_attr.attr);
    sysfs_remove_file(kobj_ref, &rejS_attr.attr);
    kobject_put(kobj_ref);
    iounmap(baseptr);
    return ret;
}

void my_cleanup_module(void) // Cleanup
{
    printk(KERN_INFO "Cleanup my module, Bye\n");

    // Write the exit code to the hardware register
    writel(SYKT_EXIT | (SYKT_EXIT_CODE << 16), baseptr);

    // Remove sysfs files and kobject
    sysfs_remove_file(kobj_ref, &rejA_attr.attr);
    sysfs_remove_file(kobj_ref, &rejW_attr.attr);
    sysfs_remove_file(kobj_ref, &rejS_attr.attr);
    kobject_put(kobj_ref);

    // Unmap the GPIO base address
    iounmap(baseptr);
}

module_init(my_init_module)
module_exit(my_cleanup_module)
