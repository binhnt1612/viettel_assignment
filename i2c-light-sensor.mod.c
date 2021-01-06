#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0xc880b7ba, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0x715e585b, __VMLINUX_SYMBOL_STR(i2c_del_driver) },
	{ 0xee311f03, __VMLINUX_SYMBOL_STR(i2c_register_driver) },
	{ 0x67c2fa54, __VMLINUX_SYMBOL_STR(__copy_to_user) },
	{ 0x91715312, __VMLINUX_SYMBOL_STR(sprintf) },
	{ 0x8277a3aa, __VMLINUX_SYMBOL_STR(mutex_unlock) },
	{ 0x813d591f, __VMLINUX_SYMBOL_STR(i2c_master_recv) },
	{ 0x12a38747, __VMLINUX_SYMBOL_STR(usleep_range) },
	{ 0xe654bfaa, __VMLINUX_SYMBOL_STR(i2c_master_send) },
	{ 0x89295f30, __VMLINUX_SYMBOL_STR(mutex_lock) },
	{ 0xefd6cf06, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr0) },
	{ 0x51784a4d, __VMLINUX_SYMBOL_STR(misc_register) },
	{ 0x6b58674b, __VMLINUX_SYMBOL_STR(__mutex_init) },
	{ 0xa9a9fece, __VMLINUX_SYMBOL_STR(devm_kmalloc) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x7842f5a1, __VMLINUX_SYMBOL_STR(misc_deregister) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("of:N*T*Crohm,bh1750*");
MODULE_ALIAS("i2c:bh1750");

MODULE_INFO(srcversion, "90FB283DACA1466D1BE6186");
