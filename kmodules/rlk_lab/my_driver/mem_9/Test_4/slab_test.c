#include <linux/module.h>
#include <linux/mm.h>
#include <linux/slab.h>
#include <linux/init.h>

static char *kbuf;
static int size = 20;
static struct kmem_cache *my_cache;
module_param(size,int,0644);

static int __init my_init(void)
{
    //create a memory cache
    if(size > KMALLOC_MAX_SIZE)
        pr_err("siez = %d is too large;you can't have more than %lu!\n",
            size,KMALLOC_MAX_SIZE);
    
    my_cache = kmem_cache_create("my_cache_test",size,0,SLAB_HWCACHE_ALIGN,NULL);
    if(!my_cache) {
        pr_err("failed to create a cache object\n");
        return -ENOMEM;
    }
    pr_info("create mycache correctly\n");

    //allocate a memory cache object
    kbuf = kmem_cache_alloc(my_cache,GFP_ATOMIC);
    if(!kbuf) {
        pr_err("failed to create a cache object\n");
        (void)kmem_cache_destroy(my_cache);
        return -1;
    }
    pr_info("successfully created a object,kbuf_addr = 0x%p\n",kbuf);
    
    return 0;
}

static void __exit my_exit(void)
{
    //destroy a memory cache object
    kmem_cache_free(my_cache,kbuf);
    pr_info("destroy a cache object\n");

    //destroy the memory cache
    kmem_cache_destroy(my_cache);
    pr_info("destroy mycaache\n");
}


module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("silence");