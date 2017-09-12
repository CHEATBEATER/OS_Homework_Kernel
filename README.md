## Linux内核模块编程
#### 操作系统课程设计，要求如下：
编写内核模块，并加载到系统，测试加载是否成功。      
（1）编写加载函数init_module()和卸载函数cleanup_module()，加载完成后用printk()输出成功信息；        
（2）用gcc编译目标模块；       
（3）用insmod加载模块，用rmmod卸载模块；        
（4）在proc文件中查看模块加载情况。

#### 操作方法：
```shell
make
sudo insmod k.ko
dmesg
sudo rmmod k.ko
dmesg
make clean
```
#### 或者自己手写insmod和rmmod：
```shell
make
gcc -o mod mod.c
dmesg
sudo ./mod -i k.ko
dmesg
sudo ./mod -r k
dmesg
make clean
```
#### 测试环境：ubuntu 12.04LTS
