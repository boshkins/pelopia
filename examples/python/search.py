import ctypes
import os

#try to locate the .so file in the same directory as this file
_file = '/home/sergey/Documents/pelopia/linux-x86_64-Debug/lib/libpelopia.so'
#_file = "/home/boshkins/github/pelopia/linux-x86_64-Debug/lib/libpelopia.so"
_path = os.path.join(*(os.path.split(__file__)[:-1] + (_file,)))
x  = ctypes . cdll . LoadLibrary ( _path )


dataset = x.createDataset
dataset.argtypes = [ctypes.POINTER(ctypes.c_char)]
dataset.restype = ctypes.POINTER(ctypes.c_void_p)

search_1 = x.Search_py
search_1.argtypes =  (ctypes.POINTER(ctypes.c_void_p), ctypes.POINTER(ctypes.c_char), ctypes.c_double, ctypes.c_double, ctypes.c_double, ctypes.c_int)   
search_1.restype = ctypes.POINTER(ctypes.c_void_p)

#for i in resp:
result = x.Get_py
result.argtypes =  (ctypes.POINTER(ctypes.c_void_p), ctypes.c_int, ctypes.c_int, ctypes.c_double) 
result.restype = ctypes.c_bool

place = x.Place_py
place.argtypes =  (ctypes.POINTER(ctypes.c_void_p), ctypes.c_int) 
place.restype = ctypes.POINTER(ctypes.c_void_p)


def main():
	ds = dataset('/home/sergey/Documents/pelopia/src/test/input/one-term.json')
        resp  = search_1(ds, "Mapzen", 40.7507, 73.9939, 0, 10) # the required numbers of arguments is six
        #result(resp, i, id, score)
        #place(ds, id)

if __name__ == '__main__':
	main()
	