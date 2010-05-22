import os

env = Environment(CXXFLAGS = '-I. -Iexample -Ilogstream -O0 -g3 -Wall', ENV = os.environ)
env.Append(LIBS = ['boost_thread-mt','boost_filesystem-mt'])
env.Append(LINKFLAGS = ['-Wl'])

env.Program('logstream_example', ['example/example.cpp'])
