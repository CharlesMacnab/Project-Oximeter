cmake_minimum_required(VERSION 3.7)
project(ProjetC C)

set(CMAKE_C_STANDARD 99)

add_executable(ProjetC main.c affichage.c affichage.h define.h fichiers.c fichiers.h fir.c fir.h iir.c iir.h integration.c integration.h lecture.c lecture.h mesure.c mesure.h)