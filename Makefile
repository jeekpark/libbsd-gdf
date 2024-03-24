SUPER_SUBDIRS = lib/Assert/ lib/Logger/
SUBDIRS = $(filter-out $(SUPER_SUBDIRS) %.dylib, $(wildcard lib/*/))

all :
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
	echo "  Add: [-I$(PWD)/include, -std=c++98]" >> .clangd
