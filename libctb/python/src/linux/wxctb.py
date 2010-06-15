# This file was created automatically by SWIG 1.3.29.
# Don't modify this file, modify the SWIG interface instead.

import _wxctb
import new
new_instancemethod = new.instancemethod
def _swig_setattr_nondynamic(self,class_type,name,value,static=1):
    if (name == "thisown"): return self.this.own(value)
    if (name == "this"):
        if type(value).__name__ == 'PySwigObject':
            self.__dict__[name] = value
            return
    method = class_type.__swig_setmethods__.get(name,None)
    if method: return method(self,value)
    if (not static) or hasattr(self,name):
        self.__dict__[name] = value
    else:
        raise AttributeError("You cannot add attributes to %s" % self)

def _swig_setattr(self,class_type,name,value):
    return _swig_setattr_nondynamic(self,class_type,name,value,0)

def _swig_getattr(self,class_type,name):
    if (name == "thisown"): return self.this.own()
    method = class_type.__swig_getmethods__.get(name,None)
    if method: return method(self)
    raise AttributeError,name

def _swig_repr(self):
    try: strthis = "proxy of " + self.this.__repr__()
    except: strthis = ""
    return "<%s.%s; %s >" % (self.__class__.__module__, self.__class__.__name__, strthis,)

import types
try:
    _object = types.ObjectType
    _newclass = 1
except AttributeError:
    class _object : pass
    _newclass = 0
del types


def _swig_setattr_nondynamic_method(set):
    def set_attr(self,name,value):
        if (name == "thisown"): return self.this.own(value)
        if hasattr(self,name) or (name == "this"):
            set(self,name,value)
        else:
            raise AttributeError("You cannot add attributes to %s" % self)
    return set_attr


new_intp = _wxctb.new_intp
copy_intp = _wxctb.copy_intp
delete_intp = _wxctb.delete_intp
intp_assign = _wxctb.intp_assign
intp_value = _wxctb.intp_value
class timer_control(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    usecs = property(_wxctb.timer_control_usecs_get, _wxctb.timer_control_usecs_set)
    exitflag = property(_wxctb.timer_control_exitflag_get, _wxctb.timer_control_exitflag_set)
    exitfnc = property(_wxctb.timer_control_exitfnc_get, _wxctb.timer_control_exitfnc_set)
timer_control_swigregister = _wxctb.timer_control_swigregister
timer_control_swigregister(timer_control)

class Timer(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args, **kwargs): 
        this = _wxctb.new_Timer(*args, **kwargs)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _wxctb.delete_Timer
    __del__ = lambda self : None;
    def start(*args, **kwargs): return _wxctb.Timer_start(*args, **kwargs)
    def stop(*args, **kwargs): return _wxctb.Timer_stop(*args, **kwargs)
Timer_swigregister = _wxctb.Timer_swigregister
Timer_swigregister(Timer)

sleepms = _wxctb.sleepms
CTB_RESET = _wxctb.CTB_RESET
class IOBase(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _wxctb.delete_IOBase
    __del__ = lambda self : None;
    def ClassName(*args, **kwargs): return _wxctb.IOBase_ClassName(*args, **kwargs)
    def Close(*args, **kwargs): return _wxctb.IOBase_Close(*args, **kwargs)
    def Ioctl(*args, **kwargs): return _wxctb.IOBase_Ioctl(*args, **kwargs)
    def IsOpen(*args, **kwargs): return _wxctb.IOBase_IsOpen(*args, **kwargs)
    def Open(*args, **kwargs): return _wxctb.IOBase_Open(*args, **kwargs)
    def PutBack(*args, **kwargs): return _wxctb.IOBase_PutBack(*args, **kwargs)
    def Read(*args, **kwargs): return _wxctb.IOBase_Read(*args, **kwargs)
    def ReadUntilEOS(*args, **kwargs): return _wxctb.IOBase_ReadUntilEOS(*args, **kwargs)
    def Readv(*args, **kwargs): return _wxctb.IOBase_Readv(*args, **kwargs)
    def Write(*args, **kwargs): return _wxctb.IOBase_Write(*args, **kwargs)
    def Writev(*args, **kwargs): return _wxctb.IOBase_Writev(*args, **kwargs)
IOBase_swigregister = _wxctb.IOBase_swigregister
IOBase_swigregister(IOBase)

ParityNone = _wxctb.ParityNone
ParityOdd = _wxctb.ParityOdd
ParityEven = _wxctb.ParityEven
ParityMark = _wxctb.ParityMark
ParitySpace = _wxctb.ParitySpace
LinestateDcd = _wxctb.LinestateDcd
LinestateCts = _wxctb.LinestateCts
LinestateDsr = _wxctb.LinestateDsr
LinestateDtr = _wxctb.LinestateDtr
LinestateRing = _wxctb.LinestateRing
LinestateRts = _wxctb.LinestateRts
LinestateNull = _wxctb.LinestateNull
class SerialPort_DCS(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    baud = property(_wxctb.SerialPort_DCS_baud_get, _wxctb.SerialPort_DCS_baud_set)
    parity = property(_wxctb.SerialPort_DCS_parity_get, _wxctb.SerialPort_DCS_parity_set)
    wordlen = property(_wxctb.SerialPort_DCS_wordlen_get, _wxctb.SerialPort_DCS_wordlen_set)
    stopbits = property(_wxctb.SerialPort_DCS_stopbits_get, _wxctb.SerialPort_DCS_stopbits_set)
    rtscts = property(_wxctb.SerialPort_DCS_rtscts_get, _wxctb.SerialPort_DCS_rtscts_set)
    xonxoff = property(_wxctb.SerialPort_DCS_xonxoff_get, _wxctb.SerialPort_DCS_xonxoff_set)
    buf = property(_wxctb.SerialPort_DCS_buf_get, _wxctb.SerialPort_DCS_buf_set)
    def __init__(self, *args, **kwargs): 
        this = _wxctb.new_SerialPort_DCS(*args, **kwargs)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _wxctb.delete_SerialPort_DCS
    __del__ = lambda self : None;
    def GetSettings(*args, **kwargs): return _wxctb.SerialPort_DCS_GetSettings(*args, **kwargs)
SerialPort_DCS_swigregister = _wxctb.SerialPort_DCS_swigregister
SerialPort_DCS_swigregister(SerialPort_DCS)

class SerialPort_EINFO(object):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    brk = property(_wxctb.SerialPort_EINFO_brk_get, _wxctb.SerialPort_EINFO_brk_set)
    frame = property(_wxctb.SerialPort_EINFO_frame_get, _wxctb.SerialPort_EINFO_frame_set)
    overrun = property(_wxctb.SerialPort_EINFO_overrun_get, _wxctb.SerialPort_EINFO_overrun_set)
    parity = property(_wxctb.SerialPort_EINFO_parity_get, _wxctb.SerialPort_EINFO_parity_set)
    def __init__(self, *args, **kwargs): 
        this = _wxctb.new_SerialPort_EINFO(*args, **kwargs)
        try: self.this.append(this)
        except: self.this = this
SerialPort_EINFO_swigregister = _wxctb.SerialPort_EINFO_swigregister
SerialPort_EINFO_swigregister(SerialPort_EINFO)

CTB_SER_GETEINFO = _wxctb.CTB_SER_GETEINFO
CTB_SER_GETBRK = _wxctb.CTB_SER_GETBRK
CTB_SER_GETFRM = _wxctb.CTB_SER_GETFRM
CTB_SER_GETOVR = _wxctb.CTB_SER_GETOVR
CTB_SER_GETPAR = _wxctb.CTB_SER_GETPAR
CTB_SER_GETINQUE = _wxctb.CTB_SER_GETINQUE
CTB_SER_SETPAR = _wxctb.CTB_SER_SETPAR
class SerialPort_x(IOBase):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    def __init__(self): raise AttributeError, "No constructor defined"
    __repr__ = _swig_repr
    __swig_destroy__ = _wxctb.delete_SerialPort_x
    __del__ = lambda self : None;
    def ClassName(*args, **kwargs): return _wxctb.SerialPort_x_ClassName(*args, **kwargs)
    def ChangeLineState(*args, **kwargs): return _wxctb.SerialPort_x_ChangeLineState(*args, **kwargs)
    def ClrLineState(*args, **kwargs): return _wxctb.SerialPort_x_ClrLineState(*args, **kwargs)
    def GetLineState(*args, **kwargs): return _wxctb.SerialPort_x_GetLineState(*args, **kwargs)
    def GetSettingsAsString(*args, **kwargs): return _wxctb.SerialPort_x_GetSettingsAsString(*args, **kwargs)
    def Ioctl(*args, **kwargs): return _wxctb.SerialPort_x_Ioctl(*args, **kwargs)
    def SendBreak(*args, **kwargs): return _wxctb.SerialPort_x_SendBreak(*args, **kwargs)
    def SetBaudrate(*args, **kwargs): return _wxctb.SerialPort_x_SetBaudrate(*args, **kwargs)
    def SetLineState(*args, **kwargs): return _wxctb.SerialPort_x_SetLineState(*args, **kwargs)
    def SetParityBit(*args, **kwargs): return _wxctb.SerialPort_x_SetParityBit(*args, **kwargs)
    IsStandardRate = staticmethod(_wxctb.SerialPort_x_IsStandardRate)
SerialPort_x_swigregister = _wxctb.SerialPort_x_swigregister
SerialPort_x_swigregister(SerialPort_x)
SerialPort_x_IsStandardRate = _wxctb.SerialPort_x_IsStandardRate

COM1 = "/dev/ttyS0"
COM2 = "/dev/ttyS1"
COM3 = "/dev/ttyS2"
COM4 = "/dev/ttyS3"
COM5 = "/dev/ttyS4"
COM6 = "/dev/ttyS5"
COM7 = "/dev/ttyS6"
COM8 = "/dev/ttyS7"
COM9 = "/dev/ttyS8"

class SerialPort(SerialPort_x):
    thisown = property(lambda x: x.this.own(), lambda x, v: x.this.own(v), doc='The membership flag')
    __repr__ = _swig_repr
    def __init__(self, *args, **kwargs): 
        this = _wxctb.new_SerialPort(*args, **kwargs)
        try: self.this.append(this)
        except: self.this = this
    __swig_destroy__ = _wxctb.delete_SerialPort
    __del__ = lambda self : None;
    def ChangeLineState(*args, **kwargs): return _wxctb.SerialPort_ChangeLineState(*args, **kwargs)
    def ClrLineState(*args, **kwargs): return _wxctb.SerialPort_ClrLineState(*args, **kwargs)
    def GetLineState(*args, **kwargs): return _wxctb.SerialPort_GetLineState(*args, **kwargs)
    def Ioctl(*args, **kwargs): return _wxctb.SerialPort_Ioctl(*args, **kwargs)
    def IsOpen(*args, **kwargs): return _wxctb.SerialPort_IsOpen(*args, **kwargs)
    def Read(*args, **kwargs): return _wxctb.SerialPort_Read(*args, **kwargs)
    def SendBreak(*args, **kwargs): return _wxctb.SerialPort_SendBreak(*args, **kwargs)
    def SetBaudrate(*args, **kwargs): return _wxctb.SerialPort_SetBaudrate(*args, **kwargs)
    def SetLineState(*args, **kwargs): return _wxctb.SerialPort_SetLineState(*args, **kwargs)
    def SetParityBit(*args, **kwargs): return _wxctb.SerialPort_SetParityBit(*args, **kwargs)
    def Write(*args, **kwargs): return _wxctb.SerialPort_Write(*args, **kwargs)
SerialPort_swigregister = _wxctb.SerialPort_swigregister
SerialPort_swigregister(SerialPort)

GetKey = _wxctb.GetKey


