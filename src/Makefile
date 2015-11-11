builddir=../build
target=$(builddir)/test
#generator=Ninja


ifeq ($(generator),Ninja)
buildfile=$(builddir)/build.ninja
else
generator=Unix Makefiles
buildfile=$(builddir)/Makefile
endif



all: $(target)

$(builddir):
	mkdir -p $(builddir)

$(target): $(buildfile)
	cmake --build $(builddir)

$(buildfile): $(builddir) CMakeLists.txt
	(srcdir=`pwd`; cd $(builddir) && cmake -G"$(generator)" $$srcdir)

.PHONY: doc
doc: $(buildfile)
	cmake --build $(builddir) -- doc

.PHONY: clean
clean:
	cmake --build $(builddir) -- clean

.PHONY: mrproper
mrproper:
	$(RM) -r $(builddir)
