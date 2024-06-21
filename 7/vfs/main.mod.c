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
	{ 0x468cde92, "simple_lookup" },
	{ 0xf57e96f2, "simple_statfs" },
	{ 0xc041efbd, "generic_delete_inode" },
	{ 0x41d201cf, "unregister_filesystem" },
	{ 0xe25442be, "kmem_cache_free" },
	{ 0x8413ce25, "kmem_cache_alloc" },
	{ 0x32308496, "kmem_cache_destroy" },
	{ 0x42713f8c, "kmem_cache_create_usercopy" },
	{ 0x37a0cba, "kfree" },
	{ 0x5f8ff9eb, "kmem_cache_alloc_trace" },
	{ 0xc1f15dee, "kmalloc_caches" },
	{ 0xfadb228e, "register_filesystem" },
	{ 0x115a3f2b, "mount_nodev" },
	{ 0x7a58bfa0, "dput" },
	{ 0xda01896d, "shrink_dcache_parent" },
	{ 0xa9931e4f, "d_genocide" },
	{ 0xaa361da5, "d_make_root" },
	{ 0x7c07651, "set_nlink" },
	{ 0xdaf121fb, "current_time" },
	{ 0xd1e3a4cb, "new_inode" },
	{ 0xd1d7002b, "kill_anon_super" },
	{ 0x92997ed8, "_printk" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xbdfb6dbb, "__fentry__" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "1EDA847E58F8363925DB7DC");
