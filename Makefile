SUPER_SUBDIRS = src/BSD-GDF/Assert/ src/BSD-GDF/Logger/
SUBDIRS = $(filter-out $(SUPER_SUBDIRS) %.dylib, $(wildcard src/BSD-GDF/*/))

all :
	mkdir -p lib
	$(foreach subdir, $(SUPER_SUBDIRS), $(MAKE) all -C $(subdir);)
	$(foreach subdir, $(SUBDIRS), $(MAKE) all -C $(subdir);)

clean :
	$(foreach subdir, $(SUPER_SUBDIRS), $(MAKE) clean -C $(subdir);)
	$(foreach subdir, $(SUBDIRS), $(MAKE) clean -C $(subdir);)

fclean :
	$(foreach subdir, $(SUPER_SUBDIRS), $(MAKE) fclean -C $(subdir);)
	$(foreach subdir, $(SUBDIRS), $(MAKE) fclean -C $(subdir);)

re :
	$(foreach subdir, $(SUPER_SUBDIRS), $(MAKE) re -C $(subdir);)
	$(foreach subdir, $(SUBDIRS), $(MAKE) re -C $(subdir);)

clangd :
	echo "CompileFlags:" > .clangd
	echo "  Add: [-I$(PWD)/include, -I$(PWD)/src, -std=c++98]" >> .clangd
