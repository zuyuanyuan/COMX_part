@echo off
y:\comx_sdk\bin\tidy.exe -xml -imq -w 1024 --show-warnings no y:\nxcocadframework/script\script.ksc
y:\comx_sdk\bin\tidy.exe -xml -imq -w 1024 --show-warnings no y:\nxcocadframework/script\package.xml
y:\comx_sdk\bin\tidy.exe -xml -imq -w 1024 --show-warnings no y:\nxcocadframework/distribute\nxcocadframework.krelease
y:\comx_sdk\bin\tidy.exe -xml -imq -w 1024 --show-warnings no y:\nxcocadframework\nxcocadframework.kproduct
y:\comx_sdk\bin\tidy.exe -xml -imq -w 1024 --show-warnings no y:\nxcocadframework/kul\sample.kul
