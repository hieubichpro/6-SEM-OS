#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xdc658e53, "module_layout" },
	{ 0x3bdb9d4, "proc_symlink" },
	{ 0xef2141dc, "proc_create" },
	{ 0xa47006d6, "proc_mkdir" },
	{ 0xd6ee688f, "vmalloc" },
	{ 0x999e8297, "vfree" },
	{ 0x953dead6, "remove_proc_entry" },
	{ 0xd0da656b, "__stack_chk_fail" },
	{ 0xc3268051, "get_pid_task" },
	{ 0x9e4e7c52, "find_get_pid" },
	{ 0x35a4e1df, "seq_printf" },
	{ 0x8c8569cb, "kstrtoint" },
	{ 0x270732c4, "single_open" },
	{ 0x988f5ab6, "seq_read" },
	{ 0x13c49cc2, "_copy_from_user" },
	{ 0x88db9f48, "__check_object_size" },
	{ 0xd6e4b68e, "seq_lseek" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xd059e6e6, "seq_release" },
	{ 0x92997ed8, "_printk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "0AC4C74FA68D02894F02A9E");
