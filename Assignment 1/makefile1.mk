storagembr1:test_assign1_1.o dberror.o storage_mgr.o 
	gcc test_assign1_1.o dberror.o storage_mgr.o -o storagembr1
test_assign1_1.o :test_assign1_1.c test_helper.h dberror.h storage_mgr.h
		gcc -c test_assign1_1.c 
dberror.o:dberror.c dberror.h
	gcc -c dberror.c
storage_mgr.o:storage_mgr.c storage_mgr.h dberror.h
	gcc -c storage_mgr.c
clean:
	rm *o storagembr1

