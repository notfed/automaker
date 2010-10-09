test : it
	./automaker-list automaker-list.c
it : automaker-list
automaker-list : load automaker-list.o buffer_0.o buffer_1.o buffer_2.o buffer_get.o buffer_put.o buffer_write.o str_len.o byte_chr.o byte_copy.o byte_copyr.o critbit0_clear.o critbit0_delete.o critbit0_allprefixed.o critbit0_contains.o critbit0_insert.o critbit0_insert.o error.o open_read.o getln.o getln2.o str_start.o str0_free.o str0_copyc.o str0_length.c str0_equalc.c stralloc_catb.o stralloc_copys.o stralloc_cats.o stralloc_copyb.o stralloc_ready.o stralloc_append.o alloc.o alloc_re.o limitmalloc_open.o limitmalloc_open_if2.o limitmalloc_close.o strerr_sys.o strerr_die.o error_str.o 
	./load automaker-list buffer_0.o buffer_1.o buffer_2.o buffer_get.o buffer_put.o buffer_write.o str_len.o byte_chr.o byte_copy.o byte_copyr.o critbit0_delete.o critbit0_clear.o critbit0_allprefixed.o critbit0_contains.o critbit0_insert.o error.o open_read.o getln.o getln2.o str_start.o str0_length.c str0_free.o str0_copyc.o str0_equalc.c stralloc_catb.o stralloc_copys.o stralloc_cats.o stralloc_copyb.o stralloc_ready.o stralloc_append.o alloc.o alloc_re.o limitmalloc_open.o limitmalloc_open_if2.c limitmalloc_close.c strerr_sys.o strerr_die.o error_str.o
error_str.o : error_str.c
	./compile error_str.c
strerr_sys.o : strerr_sys.c
	./compile strerr_sys.c
strerr_die.o : strerr_die.c
	./compile strerr_die.c
automaker-list.o : automaker-list.c
	./compile automaker-list.c
limitmalloc_open.o : limitmalloc_open.c
	./compile limitmalloc_open.c
limitmalloc_close.o : limitmalloc_close.c
	./compile limitmalloc_close.c
limitmalloc_open_if2.o : limitmalloc_open_if2.c
	./compile limitmalloc_open_if2.c
buffer_0.o : buffer_0.c
	./compile buffer_0.c
buffer_1.o : buffer_1.c
	./compile buffer_1.c
buffer_2.o : buffer_2.c
	./compile buffer_2.c
buffer_put.o : buffer_put.c
	./compile buffer_put.c
buffer_get.o : buffer_get.c
	./compile buffer_get.c
buffer_write.o : buffer_write.c
	./compile buffer_write.c
buffer_ready.o : buffer_ready.c
	./compile buffer_ready.c
str_len.o : str_len.c
	./compile str_len.c
byte_copy.o : byte_copy.c
	./compile byte_copy.c
byte_copyr.o : byte_copyr.c
	./compile byte_copyr.c
byte_chr.o : byte_chr.c
	./compile byte_chr.c
open_read.o : open_read.c
	./compile open_read.c
critbit0_allprefixed.o : critbit0_allprefixed.c
	./compile critbit0_allprefixed.c
critbit0_delete.o : critbit0_delete.c
	./compile critbit0_delete.c
critbit0_contains.o : critbit0_contains.c
	./compile critbit0_contains.c
critbit0_insert.o : critbit0_insert.c
	./compile critbit0_insert.c
critbit0_clear.o : critbit0_clear.c
	./compile critbit0_clear.c
error.o : error.c
	./compile error.c
getln.o : getln.c 
	./compile getln.c
getln2.o : getln2.c 
	./compile getln2.c
str_start.o : str_start.c
	./compile str_start.c
str0_free.o : str0_free.c
	./compile str0_free.c
str0_copyc.o : str0_copyc.c
	./compile str0_copyc.c
str0_length.o : str0_length.c
	./compile str0_length.c
str0_equalc.o : str0_equalc.c
	./compile str0_equalc.c
stralloc_copys.o : stralloc_copys.c
	./compile stralloc_copys.c
stralloc_cats.o : stralloc_cats.c
	./compile stralloc_cats.c
stralloc_catb.o : stralloc_catb.c
	./compile stralloc_catb.c
stralloc_copyb.o : stralloc_copyb.c
	./compile stralloc_copyb.c
stralloc_ready.o : stralloc_ready.c
	./compile stralloc_ready.c
stralloc_append.o : stralloc_append.c
	./compile stralloc_append.c
alloc.o : alloc.c
	./compile alloc.c
alloc_re.o : alloc_re.c
	./compile alloc_re.c
clean : 
	rm -f *.o automaker-list
