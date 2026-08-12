
env = Environment(
    CCFLAGS=['-std=c++17', '-Wall'],  #Компилятор и флаг предупреждений
    CPPPATH=['include', 'external']  #Указывает только на ашники заголовочные для сборки
)
sources = Glob('src/*.cpp')    #Указываемся на функциональные файлв cpp

program = env.Program('zoo_planner', sources)
Default(program)