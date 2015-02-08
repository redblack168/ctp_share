#! /usr/bin/python
# encoding: utf-8

top = '.'
out = '_build'

api_path = '/6.2.0_201312041840_apitraderapi_linux64'

def options(opt):
        opt.load('compiler_cxx')
def configure(cnf):
	cnf.env.CTP_API_PATH = cnf.path.abspath() + api_path
        cnf.load('compiler_cxx')

def build(bld):
  sources = bld.path.ant_glob(['**/*.cpp', '**/*.cc'],  excl=['**/simple'])
  headers = bld.path.ant_glob(['**/*.h'])
  bld.program(
                source       = sources,
                header       = headers,
                target       = 'md_recoder',
                #features     = ['qt5'],
                #use      = ['QT5CORE', 'QT5WIDGETS', 'QT5GUI'],
                includes     = ['.', bld.env.CTP_API_PATH],
                #defines      = ['LINUX=1', 'BIDULE'],

                lib          = ['thostmduserapi', 'thosttraderapi'],
                libpath      = ['/usr/lib', bld.env.CTP_API_PATH],
                linkflags    = ['-g'], 
                rpath        = [bld.env.CTP_API_PATH],
                #vnum         = '1.2.3',

                #install_path = '${SOME_PATH}/bin',
                #cflags       = ['-O2', '-Wall'],
                cxxflags     = ['-O3', '-fPIC','-std=c++0x', '-Wall'],
                dflags       = ['-g'],
  )
 
