LIBS=$(wildcard libs/*)
INCLUDE_PATHS=libs/
EDITOR=kate

clean:
	rm -rf Libraries/
#	rm hex.elf

Libraries:
	mkdir Libraries/
	for dir in $(LIBS); do \
		cd $$dir; \
		arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -c *.c -I../; \
		mv *.o ../../Libraries; \
		cd -; \
	done

hex: Libraries
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -g3 -O0 main.c $(wildcard Libraries/*) -I$(INCLUDE_PATHS) -o hex.elf

edit: 
	for dir in $(LIBS); do \
			cd $$dir; \
			$(EDITOR) *.c;\
			$(EDITOR) *.h;\
			cd -; \
	done
