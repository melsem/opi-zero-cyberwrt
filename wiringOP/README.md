
# Depends:
-    make menuconfig, select Global Build Settings ---> Kernel Build Options:
-    check /dev/mem virtual device support and
-    check /dev/kmem virtual device support

-    Select your board from the list below and enter it into the file /patches/001-add-board_config.patch:

```
	orangepione-h3
	orangepilite-h3
	orangepipc-h3
	orangepiplus-h3
	orangepipcplus-h3
	orangepiplus2e-h3
	orangepi2giot
	orangepipc2-h5
	orangepiprime-h5
	orangepizeroplus-h5
	orangepiwin-a64
	orangepiwinplus-a64
	orangepizero-h2
	orangepir1-h2
	orangepioneplus-h6
	orangepilite2-h6
	orangepi3-h6
	orangepizero2-h616
	orangepizeroplus2h3
	orangepizeroplus2h5
	orangepirk3399
	orangepi4
	orangepi800
	orangepir1plus-rk3328
```

-    Example for orangepizero-h2:
```
--- a/BOARD
+++ b/BOARD
@@ -0,0 +1,1 @@
+orangepizero-h2
```

