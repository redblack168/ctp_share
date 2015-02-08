#! /usr/bin/python
# encoding: utf-8

top = '.'
out = '_build'

api_path = '/6.2.0_201312041840_apitraderapi_linux64'

snap_gcc_root = '/usr/local/snap_gcc49'
snap_gcc_bin = snap_gcc_root + '/bin'
snap_gcc_lib64 = snap_gcc_root + '/lib64'

def LoadSnapGcc(conf):
    cc = conf.find_program('gcc', path_list=[snap_gcc_bin, ], var='CC')
    cxx = conf.find_program('g++', path_list=[snap_gcc_bin, ], var='CXX')
#    conf.find_program('gcc-ar', path_list=[snap_gcc_bin, ], var='AR')
    try:
      distcc = conf.find_program('distcc')
      cc = distcc + cc
      cxx = distcc + cxx
    except conf.errors.ConfigurationError:
      conf.to_log("distcc not found complie local instead")
    try:
      cache = conf.find_program('ccache')
      cc = cache + cc
      cxx = cache + cxx
    except conf.errors.ConfigurationError:
      conf.to_log("cache not found complie local instead")

    conf.get_cc_version(cc, gcc=True)
    conf.get_cc_version(cxx, gcc=True)
    conf.env.CC_NAME = 'gcc'
    conf.env.CC = cc
    conf.env.CXX_NAME = 'gcc'
    conf.env.CXX = cxx
    conf.env.append_unique('RPATH', snap_gcc_lib64)

def options(opt):
        opt.load('compiler_cxx')
def configure(cnf):
	LoadSnapGcc(cnf)
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
 
