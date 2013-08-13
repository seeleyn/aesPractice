env = Environment()
env.Append(CPPPATH = ['/usr/include/openssl'])
env.Append(LIBS = ['crypto'])

sources = Split("""
aesenc.cpp
base64.cpp
""")
object_list = env.Object(source = sources)
env.Program(target = 'aesenc', source = object_list, CCFLAGS='-g')

