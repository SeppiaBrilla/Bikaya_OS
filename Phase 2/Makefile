default:
	@echo "Please specify a target"

umps:
ifneq ("$(wildcard ./kernel)","")
ifeq ("$(wildcard ./kernel.core.umps)","")
	$(MAKE) -f uarmmake clean
	@echo "object files removed"
endif
endif
	$(MAKE) -f umpsmake

umps2:
ifneq ("$(wildcard ./kernel)","")
ifeq ("$(wildcard ./kernel.core.umps)","")
	$(MAKE) -f uarmmake clean
	@echo "object files removed"
endif
endif
	$(MAKE) -f umpsmake

uarm:
ifneq ("$(wildcard ./kernel.core.umps)","")
	$(MAKE) -f umpsmake clean
	@echo "object files removed"
endif
	$(MAKE) -f uarmmake


uarm-clean:
	bash -c	"rm -f kernel.*.umps"
	$(MAKE) -f uarmmake clean-build

umps-clean:
	$(MAKE) -f umpsmake clean-build

clean:
	bash -c	"rm -f *.o kernel.*.umps kernel"

.PHONY: umps umps2 uarm clean default