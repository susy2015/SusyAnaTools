from ctypes import cdll
from ctypes import c_char_p
from ctypes import POINTER 

class SampleCollection:
    def __init__(self):
        self.lib = cdll.LoadLibrary('../obj/samplesModule.so')
        self.obj = self.lib.SC_new()
        self.lib.SC_samples.restype = POINTER(c_char_p)
        self.lib.SC_samples_names.restype = POINTER(c_char_p)

    def nSamples(self, name):
        return self.lib.SC_samples_size(self.obj, name)

    def sampleList(self, name):
        names = self.lib.SC_samples(self.obj, name)
        files = self.lib.SC_samples_names(self.obj, name)
        list = [(names[i],files[i]) for i in xrange(self.lib.SC_samples_size(self.obj, name))]
        return list

