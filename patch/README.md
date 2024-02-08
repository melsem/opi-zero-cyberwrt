

# An example of using the build system.
- [1.] Download sources for **[old-stable openwrt-v22.03.6](https://github.com/openwrt/openwrt/archive/refs/tags/v22.03.6.tar.gz)**
* And unpack:
```
wget https://github.com/openwrt/openwrt/archive/refs/tags/v22.03.6.tar.gz
tar -xvf v22.03.6.tar.gz
cd openwrt-22.03.6
```
- [2.] Download patches **[1-old-stable openwrt-v22.03.6](https://github.com/melsem/opi-zero-cyberwrt/raw/master/patch/All_openwrt-22.03.6.patch)**
 and **[2-old-stable openwrt-v22.03.6](https://github.com/melsem/opi-zero-cyberwrt/raw/master/patch/036-.config_openwrt-22.03.6.patch)**
* Download two patches to the root folder of openwrt-22.03.6:
```
wget https://github.com/melsem/opi-zero-cyberwrt/raw/master/patch/All_openwrt-22.03.6.patch
wget https://github.com/melsem/opi-zero-cyberwrt/raw/master/patch/036-.config_openwrt-22.03.6.patch
./scripts/feeds update -a
patch -p1 < All_openwrt-22.03.6.patch
./scripts/feeds update opicyberwrt
./scripts/feeds update diskman
./scripts/feeds install -a
patch -p1 < 036-.config_openwrt-22.03.6.patch
make menuconfig
make -j1 V=s
```

