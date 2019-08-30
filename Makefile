all: src tools test

.PHONY: src tools test check clean distclean 

test: src tools
	$(MAKE) -C $@ $(MAKEFLAGS)

src tools:
	$(MAKE) -C $@ $(MAKEFLAGS)

check: test
	$(MAKE) -C test $(MAKECMDGOALS)

clean:
	$(MAKE) -C src $(MAKECMDGOALS)
	$(MAKE) -C tools $(MAKECMDGOALS)
	$(MAKE) -C test $(MAKECMDGOALS)

distclean:
	$(MAKE) -C src $(MAKECMDGOALS)
	$(MAKE) -C tools $(MAKECMDGOALS)
	$(MAKE) -C test $(MAKECMDGOALS)
