## How to use with Arduino IDE:

Add to boards.txt:
```
generic_f303cc.menu.upload_method.CDCMethod=CDC bootloader
generic_f303cc.menu.upload_method.CDCMethod.upload.protocol=cdc_upload
generic_f303cc.menu.upload_method.CDCMethod.upload.tool=cdc_upload
generic_f303cc.menu.upload_method.CDCMethod.build.vect_flags=-DUSER_ADDR_ROM=0x08001800
generic_f303cc.menu.upload_method.CDCMethod.build.ldscript=ld/cdc_upload.ld
```
Add to platform.txt:
```
# cdc upload
tools.cdc_upload.cmd=cdc_upload
tools.cdc_upload.cmd.windows=cdc_upload.bat
tools.cdc_upload.path.windows={runtime.hardware.path}/tools/win
#tools.cdc_upload.path.macosx={runtime.hardware.path}/tools/macosx
#tools.cdc_upload.path.linux={runtime.hardware.path}/tools/linux
#tools.cdc_upload.path.linux64={runtime.hardware.path}/tools/linux64
tools.cdc_upload.upload.params.verbose=
tools.cdc_upload.upload.params.quiet=
tools.cdc_upload.upload.pattern="{path}/{cmd}" "{build.path}/{build.project_name}.bin" {serial.port.file} 2
```
Copy to variant directory the linker file:
- https://github.com/stevstrong/Arduino_STM32/blob/master/STM32F3/variants/generic_f303cc/ld/cdc_upload.ld

Copy to tools/win directory the files:
- https://github.com/stevstrong/Arduino_STM32/blob/master/tools/win/cdc_flasher.exe
- https://github.com/stevstrong/Arduino_STM32/blob/master/tools/win/cdc_upload.bat

