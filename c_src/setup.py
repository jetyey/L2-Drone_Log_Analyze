from distutils.core import setup, Extension
from os import listdir
from os.path import isfile, join

module1 = Extension('dranalyse',
                    ['sources/cJSON.c','sources/freearray.c', "sources/errortotxt.c" , 'sources/batteryAnalyze2.c', 'sources/compassAnalyze.c', 'sources/arrange.c', 'sources/compassoffsetAnalyze.c', 'sources/count.c', 'sources/arrayallocation.c', 'sources/array_to_csv.c', 'sources/mechanicalAnalyze.c', 'sources/gpsAnalyze.c', 'sources/dranalyse.c','sources/vibrationAnalyze.c'],
                    include_dirs = ["include"])


setup (name = 'dranalyse',
       version = '1.0',
       description = 'dranalyse',
       ext_modules = [module1])
