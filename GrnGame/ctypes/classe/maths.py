from ..structures import *
from ctypes import CDLL

class _maths:
    def __init__(self, lib: CDLL, utils):
        self.jeu = lib
        self.utils = utils

    def abs_val(self, x):
        return self.jeu.abs_val(c_double(x))
    
    def clamp(self, x, min_, max_):
        return self.jeu.clamp(c_double(x), c_double(min_), c_double(max_))
    
    def pow(self, base, exp):
        return self.jeu.pow_custom(c_double(base), c_double(exp))
    
    def sqrt(self, x):
        return self.jeu.sqrt_custom(c_double(x))
    
    def cbrt(self, x):
        return self.jeu.cbrt_custom(c_double(x))
    
    def exp(self, x):
        return self.jeu.exp_custom(c_double(x))
    
    def log(self, x):
        return self.jeu.log_custom(c_double(x))
    
    def log10(self, x):
        return self.jeu.log10_custom(c_double(x))
    
    def log2(self, x):
        return self.jeu.log2_custom(c_double(x))
    
    def sin(self, x):
        return self.jeu.sin_custom(c_double(x))
    
    def cos(self, x):
        return self.jeu.cos_custom(c_double(x))
    
    def tan(self, x):
        return self.jeu.tan_custom(c_double(x))
    
    def asin(self, x):
        return self.jeu.asin_custom(c_double(x))
    
    def acos(self, x):
        return self.jeu.acos_custom(c_double(x))
    
    def atan(self, x):
        return self.jeu.atan_custom(c_double(x))
    
    def atan2(self, y, x):
        return self.jeu.atan2_custom(c_double(y), c_double(x))
    
    def sinh(self, x):
        return self.jeu.sinh_custom(c_double(x))
    
    def cosh(self, x):
        return self.jeu.cosh_custom(c_double(x))
    
    def tanh(self, x):
        return self.jeu.tanh_custom(c_double(x))
    
    def asinh(self, x):
        return self.jeu.asinh_custom(c_double(x))
    
    def acosh(self, x):
        return self.jeu.acosh_custom(c_double(x))
    
    def atanh(self, x):
        return self.jeu.atanh_custom(c_double(x))
    
    def floor(self, x):
        return self.jeu.floor_custom(c_double(x))
    
    def ceil(self, x):
        return self.jeu.ceil_custom(c_double(x))
    
    def round(self, x):
        return self.jeu.round_custom(c_double(x))
    
    def trunc(self, x):
        return self.jeu.trunc_custom(c_double(x))
    
    def fmod(self, x, y):
        return self.jeu.fmod_custom(c_double(x), c_double(y))
    
    def hypot(self, x, y):
        return self.jeu.hypot_custom(c_double(x), c_double(y))

    def random(self, min_val, max_val):
        return self.jeu.random_jeu(min_val, max_val)