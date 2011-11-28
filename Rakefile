require 'rake/clean'

APP_NAME = 'lucas-kanade-opencv'
CFLAGS = `pkg-config --cflags opencv`.strip
LIBS = `pkg-config --libs opencv`.strip

CLEAN.include(APP_NAME, '*.o')

task :default => [APP_NAME]

SRC = FileList['*.c']
OBJ = SRC.ext 'o'

rule '.o' => '.c' do |file|
  `gcc -c #{CFLAGS} -o #{file.name} #{file.source}`
end

file APP_NAME => OBJ do
  `gcc -o #{APP_NAME} #{OBJ} #{LIBS}`
end

file 'lucas-kanade-opencv.o' => 'lucas-kanade-opencv.c'
