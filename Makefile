# Alternative GNU Make workspace makefile autogenerated by Premake

ifndef config
  config=debug
endif

ifndef verbose
  SILENT = @
endif

ifeq ($(config),debug)
  GLFW_config = debug
  GLAD_config = debug
  IMGUI_config = debug
  Ullmannite_config = debug

else ifeq ($(config),release)
  GLFW_config = release
  GLAD_config = release
  IMGUI_config = release
  Ullmannite_config = release

else
  $(error "invalid configuration $(config)")
endif

PROJECTS := GLFW GLAD IMGUI Ullmannite

.PHONY: all clean help $(PROJECTS) 

all: $(PROJECTS)

GLFW:
ifneq (,$(GLFW_config))
	@echo "==== Building GLFW ($(GLFW_config)) ===="
	@${MAKE} --no-print-directory -C . -f GLFW.make config=$(GLFW_config)
endif

GLAD:
ifneq (,$(GLAD_config))
	@echo "==== Building GLAD ($(GLAD_config)) ===="
	@${MAKE} --no-print-directory -C . -f GLAD.make config=$(GLAD_config)
endif

IMGUI: GLFW
ifneq (,$(IMGUI_config))
	@echo "==== Building IMGUI ($(IMGUI_config)) ===="
	@${MAKE} --no-print-directory -C . -f IMGUI.make config=$(IMGUI_config)
endif

Ullmannite: GLFW GLAD IMGUI
ifneq (,$(Ullmannite_config))
	@echo "==== Building Ullmannite ($(Ullmannite_config)) ===="
	@${MAKE} --no-print-directory -C Ullmannite -f Makefile config=$(Ullmannite_config)
endif

clean:
	@${MAKE} --no-print-directory -C . -f GLFW.make clean
	@${MAKE} --no-print-directory -C . -f GLAD.make clean
	@${MAKE} --no-print-directory -C . -f IMGUI.make clean
	@${MAKE} --no-print-directory -C Ullmannite -f Makefile clean

help:
	@echo "Usage: make [config=name] [target]"
	@echo ""
	@echo "CONFIGURATIONS:"
	@echo "  debug"
	@echo "  release"
	@echo ""
	@echo "TARGETS:"
	@echo "   all (default)"
	@echo "   clean"
	@echo "   GLFW"
	@echo "   GLAD"
	@echo "   IMGUI"
	@echo "   Ullmannite"
	@echo ""
	@echo "For more information, see https://github.com/premake/premake-core/wiki"