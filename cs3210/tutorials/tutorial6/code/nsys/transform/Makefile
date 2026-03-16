NVCC = nvcc
TARGET = transform
NVCC_FLAGS = -arch=sm_80 -Xptxas=-v,-warn-spills,-warn-lmem-usage -lineinfo

all:
	$(NVCC) $(NVCC_FLAGS) main.cu transform.cu -o $(TARGET)

clean:
	rm -f $(TARGET)
