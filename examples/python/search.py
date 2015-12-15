from ctypes import *

x  = ctypes . pelopia . LoadLibrary ( _path )


ds = x.createDataset
ds. argtypes =  ctypes.POINTER(c_char) 
ds. restype = ctypes.POINTER(c_void_p)

resp = x.Search_py
resp.argtypes =  (ctypes.POINTER(c_void_p), ctypes.POINTER(c_char), ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_int)   
resp.restype = ctypes.POINTER(c_void_p)

for i in resp:
    result = x.Get_py
    result.argtypes =  (ctypes.POINTER(c_void_p), ctypes.c_int, ctypes.c_double) 
    result.restype = ctypes.c_bool

    ds = x.createDataset
    ds.argtypes =  (ctypes.POINTER(c_void_p), ctypes.c_int) 
    ds.restype = ctypes.POINTER(c_void_p)

