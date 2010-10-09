test : it
	./automaker automaker.c
it : automaker
automaker : load automaker.o alloc.o alloc_re.o buffer_0.o buffer_1.o buffer_2.o buffer_get.o buffer_put.o buffer_write.o byte_chr.o byte_copy.o byte_copyr.o critbit0_allprefixed.o critbit0_clear.o critbit0_contains.o critbit0_insert.o error.o error_str.o getln.o getln2.o limitmalloc_close.o limitmalloc_open.o limitmalloc_open_if2.o open_read.o str0_copyc.o str0_equalc.o str0_free.o str0_length.o str_len.o str_start.o stralloc_append.o stralloc_catb.o stralloc_cats.o stralloc_copyb.o stralloc_copys.o stralloc_ready.o strerr_die.o strerr_sys.o
	./load automaker alloc.o alloc_re.o buffer_0.o buffer_1.o buffer_2.o buffer_get.o buffer_put.o buffer_write.o byte_chr.o byte_copy.o byte_copyr.o critbit0_allprefixed.o critbit0_clear.o critbit0_contains.o critbit0_insert.o error.o error_str.o getln.o getln2.o limitmalloc_close.o limitmalloc_open.o limitmalloc_open_if2.o open_read.o str0_copyc.o str0_equalc.o str0_free.o str0_length.o str_len.o str_start.o stralloc_append.o stralloc_catb.o stralloc_cats.o stralloc_copyb.o stralloc_copys.o stralloc_ready.o strerr_die.o strerr_sys.o
alloc.o : compile alloc.c
	./compile alloc.c
alloc_re.o : compile alloc_re.c
	./compile alloc_re.c
automaker.o : compile automaker.c
	./compile automaker.c
buffer_0.o : compile buffer_0.c
	./compile buffer_0.c
buffer_1.o : compile buffer_1.c
	./compile buffer_1.c
buffer_2.o : compile buffer_2.c
	./compile buffer_2.c
buffer_get.o : compile buffer_get.c
	./compile buffer_get.c
buffer_put.o : compile buffer_put.c
	./compile buffer_put.c
buffer_write.o : compile buffer_write.c
	./compile buffer_write.c
byte_chr.o : compile byte_chr.c
	./compile byte_chr.c
byte_copy.o : compile byte_copy.c
	./compile byte_copy.c
byte_copyr.o : compile byte_copyr.c
	./compile byte_copyr.c
critbit0_allprefixed.o : compile critbit0_allprefixed.c
	./compile critbit0_allprefixed.c
critbit0_clear.o : compile critbit0_clear.c
	./compile critbit0_clear.c
critbit0_contains.o : compile critbit0_contains.c
	./compile critbit0_contains.c
critbit0_insert.o : compile critbit0_insert.c
	./compile critbit0_insert.c
error.o : compile error.c
	./compile error.c
error_str.o : compile error_str.c
	./compile error_str.c
getln.o : compile getln.c
	./compile getln.c
getln2.o : compile getln2.c
	./compile getln2.c
limitmalloc_close.o : compile limitmalloc_close.c
	./compile limitmalloc_close.c
limitmalloc_open.o : compile limitmalloc_open.c
	./compile limitmalloc_open.c
limitmalloc_open_if2.o : compile limitmalloc_open_if2.c
	./compile limitmalloc_open_if2.c
open_read.o : compile open_read.c
	./compile open_read.c
str0_copyc.o : compile str0_copyc.c
	./compile str0_copyc.c
str0_equalc.o : compile str0_equalc.c
	./compile str0_equalc.c
str0_free.o : compile str0_free.c
	./compile str0_free.c
str0_length.o : compile str0_length.c
	./compile str0_length.c
str_len.o : compile str_len.c
	./compile str_len.c
str_start.o : compile str_start.c
	./compile str_start.c
stralloc_append.o : compile stralloc_append.c
	./compile stralloc_append.c
stralloc_catb.o : compile stralloc_catb.c
	./compile stralloc_catb.c
stralloc_cats.o : compile stralloc_cats.c
	./compile stralloc_cats.c
stralloc_copyb.o : compile stralloc_copyb.c
	./compile stralloc_copyb.c
stralloc_copys.o : compile stralloc_copys.c
	./compile stralloc_copys.c
stralloc_ready.o : compile stralloc_ready.c
	./compile stralloc_ready.c
strerr_die.o : compile strerr_die.c
	./compile strerr_die.c
strerr_sys.o : compile strerr_sys.c
	./compile strerr_sys.c
clean :
	rm -f *.o automaker
