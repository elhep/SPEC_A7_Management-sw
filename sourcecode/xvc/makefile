include xcompile.mk

TARGET := main
OUT_BIN_NAME := hello

all: $(TARGET)

$(TARGET):
	@echo "Compiling C program"
	$(CC) $(CFLAGS) $(TARGET).c -lgpiod -o $(OUT_BIN_NAME) $(LDFLAGS)

clean:
	@rm -rf $(OUT_BIN_NAME)
