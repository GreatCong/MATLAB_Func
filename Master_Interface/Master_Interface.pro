TEMPLATE = lib
#CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
#CONFIG += mexw64

TARGET = Master_Interface

contains(QT_ARCH, i386) {

TARGET=$$join(TARGET,,,.mexw32)

INCLUDEPATH += \
   $$(MATLAB_ROOT_X86)/extern/include
   $$(MATLAB_ROOT_X86)/extern/include/win32

LIBS += $$(MATLAB_ROOT_X86)/extern/lib/win32/microsoft/libmx.lib \
      $$(MATLAB_ROOT_X86)/extern/lib/win32/microsoft/libmat.lib \
      $$(MATLAB_ROOT_X86)/extern/lib/win32/microsoft/libmex.lib \
      $$(MATLAB_ROOT_X86)/extern/lib/win32/microsoft/libeng.lib

   message($$TARGET "32-bit")

} else {
TARGET=$$join(TARGET,,,.mexw64)

INCLUDEPATH += \
   $$(MATLAB_ROOT)/extern/include
   $$(MATLAB_ROOT)/extern/include/w64

LIBS += $$(MATLAB_ROOT)/extern/lib/win64/microsoft/libmx.lib \
      $$(MATLAB_ROOT)/extern/lib/win64/microsoft/libmat.lib \
      $$(MATLAB_ROOT)/extern/lib/win64/microsoft/libmex.lib \
      $$(MATLAB_ROOT)/extern/lib/win64/microsoft/libeng.lib

   message($$TARGET "64-bit")
}

include(MemShare_EtherCAT/MemShare_EtherCAT.pri)

DEFINES += MATLAB_MEX_FILE \
           _CRT_SECURE_NO_WARNINGS

SOURCES += \
    Master_Interface.cpp \
    MasterWrapper.cpp

# windows
DEF_FILE += \
    MEX_interface.def

HEADERS += \
    MasterWrapper.h
