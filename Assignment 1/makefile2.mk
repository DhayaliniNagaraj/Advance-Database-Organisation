storagembr2:test_assign1_2.o dberror.o storage_mgr.o 
	gcc test_assign1_2.o dberror.o storage_mgr.o -o storagembr2
dberror.o:dberror.c dberror.h
	gcc -c dberror.c
storage_mgr.o:storage_mgr.c storage_mgr.h dberror.h
	gcc -c storage_mgr.c
test_assign2.o: test_assign1_2.c test_helper.h dberror.h storage_mgr.h
		gcc -c test_assign1_2.c
clean:
	rm *o storagembr2
