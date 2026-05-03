NVCC = nvcc
TARGET = mat_transpose
NVCC_FLAGS = -arch=sm_80 -Xptxas=-v,-warn-spills,-warn-lmem-usage -lineinfo

all:
	$(NVCC) $(NVCC_FLAGS) main.cu mat_transpose.cu -o $(TARGET)

clean:
	rm -f $(TARGET)
