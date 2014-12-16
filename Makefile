CCFLAGS = -fPIC -fopenmp -lmagma \
-fPIC -fopenmp -lopenblas -lcublas -lcudart -lm

CXXFLAGS = -O3  -Wall -fno-strict-aliasing -lmagma \
-L/usr/local/openBLAS/lib -L/usr/local/cuda/lib64 \
-fPIC -fopenmp -lopenblas -lcublas -lcudart -lm

BINARIES = magtest

CC = gcc
CXX = g++

MACROS = -DMAGMA_SETAFFINITY -DHAVE_CUBLAS -DMIN_CUDA_ARCH=200 -DADD_

INCLUDEDIRS = -I/usr/local/cuda/include -Imagmainclude/

main: main.o butterfly.o 
	#(CXX) -O3  -Wall -fno-strict-aliasing -o $@ $<

main.o: main.cpp
	$(CXX) -O3  -Wall -fno-strict-aliasing -o $@ -c $<

butterfly.o: butterfly.cpp
	$(CXX) -O3  -Wall -fno-strict-aliasing -o $@ -c $<

magtest: magtest.o
	$(CC) $(CCFLAGS) -o $@ $<

magtest.o: magtest.cpp
	$(CXX) $(CXXFLAGS) $(MACROS) $(INCLUDEDIRS) -o $@ -c $<

clean:
	rm -f *.o $(BINARIES)

# gcc -fPIC -fopenmp  testing_dgesv_gpu.o -o testing_dgesv_gpu \
    libtest.a lin/liblapacktest.a -L../lib -lmagma \
        -L/usr/local/openBLAS/lib -L/usr/local/cuda/lib64 \
            -lopenblas -lcublas -lcudart -lm
# g++ -fPIC -O3 -DADD_ -Wall -fno-strict-aliasing -fopenmp -DMAGMA_SETAFFINITY -DHAVE_CUBLAS -DMIN_CUDA_ARCH=200 -DHAVE_CUBLAS -DMIN_CUDA_ARCH=200 -I/usr/local/cuda/include -I../include -I../control -I../sparse-iter/include -c testing_dgesv_gpu.cpp -o testing_dgesv_gpu.o

